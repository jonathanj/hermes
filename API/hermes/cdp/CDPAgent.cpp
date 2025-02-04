/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "CDPAgent.h"

#include <hermes/inspector/chrome/MessageConverters.h>
#include <hermes/inspector/chrome/MessageTypes.h>

namespace facebook {
namespace hermes {
namespace cdp {

using namespace facebook::hermes::debugger;
using namespace facebook::hermes::inspector_modern::chrome;

/// Implementation of the CDP Agent. This class accepts CDP commands from
/// arbitrary threads and delivers them to the appropriate, domain-specific
/// message handler on the runtime thread. This class also manages the lifetime
/// of the domain-specific message handlers. Methods on this class can be
/// called from arbitrary threads.
class CDPAgentImpl : public std::enable_shared_from_this<CDPAgentImpl> {
 public:
  CDPAgentImpl(
      HermesRuntime &runtime,
      AsyncDebuggerAPI &asyncDebuggerAPI,
      EnqueueRuntimeTaskFunc enqueueRuntimeTaskCallback,
      OutboundMessageFunc messageCallback);
  ~CDPAgentImpl();

  /// Schedule initialization of handlers for each message domain.
  void initializeDomainAgents();

  /// Process a CDP command encoded in \p json.
  void handleCommand(std::string json);

 private:
  /// Collection of domain-specific message handlers. These handlers require
  /// exclusive access to the runtime (whereas the CDP Agent can be used from)
  /// arbitrary threads), so all methods on this struct are expected to be
  /// called with exclusive access to the runtime.
  struct DomainAgents {
    // Create a new collection of domain agents.
    DomainAgents(OutboundMessageFunc messageCallback);

    /// Create the domain handlers and subscribing to any external events.
    void initialize(HermesRuntime &runtime, AsyncDebuggerAPI &asyncDebuggerAPI);

    /// Releasing any domain handlers and event subscriptions.
    void dispose();

    /// Process a CDP \p command encoded in JSON using the appropriate domain
    /// handler.
    void handleCommand(
        HermesRuntime &runtime,
        AsyncDebuggerAPI &asyncDebuggerAPI,
        std::shared_ptr<message::Request> command);

    OutboundMessageFunc messageCallback_;

    // TODO: Add storage for domain-specific handlers
  };

  HermesRuntime &runtime_;
  debugger::AsyncDebuggerAPI &asyncDebuggerAPI_;

  EnqueueRuntimeTaskFunc enqueueRuntimeTaskCallback_;
  OutboundMessageFunc messageCallback_;

  RuntimeTaskRunner runtimeTaskRunner_;
  std::shared_ptr<DomainAgents> domainAgents_;
};

CDPAgentImpl::CDPAgentImpl(
    HermesRuntime &runtime,
    debugger::AsyncDebuggerAPI &asyncDebuggerAPI,
    EnqueueRuntimeTaskFunc enqueueRuntimeTaskCallback,
    OutboundMessageFunc messageCallback)
    : runtime_(runtime),
      asyncDebuggerAPI_(asyncDebuggerAPI),
      enqueueRuntimeTaskCallback_(enqueueRuntimeTaskCallback),
      messageCallback_(messageCallback),
      runtimeTaskRunner_(asyncDebuggerAPI, enqueueRuntimeTaskCallback),
      domainAgents_(std::make_shared<DomainAgents>(messageCallback)) {}

CDPAgentImpl::~CDPAgentImpl() {
  // Call DomainAgents::dispose on the runtime thread, only keeping a copy of
  // the domainAgents shared pointer (as opposed to the usual shared "this"),
  // so the remainder of the CDP Agent is free to be released.
  // AsyncDebuggerAPI::triggerInterrupt (which is used by the
  // runtimeTaskRunner) guarantees the callback will run even if
  // AsyncDebuggerAPI is being destructed.
  // The runtimeTaskRunner is still alive at this point, so it can be used
  // to enqueue the task. enqueueTask will add the task to the relevant
  // execution queues before returning, with no lingering references to the
  // runtimeTaskRunner itself.
  runtimeTaskRunner_.enqueueTask(
      [domainAgents = domainAgents_](HermesRuntime &runtime) {
        domainAgents->dispose();
      });
}

void CDPAgentImpl::initializeDomainAgents() {
  // Call DomainAgents::initialize on the runtime thread.
  auto sharedThis = shared_from_this();
  auto weakThis = std::weak_ptr<CDPAgentImpl>(shared_from_this());
  runtimeTaskRunner_.enqueueTask(
      [weakThis = std::move(weakThis)](HermesRuntime &) {
        if (auto strongThis = weakThis.lock()) {
          strongThis->domainAgents_->initialize(
              strongThis->runtime_, strongThis->asyncDebuggerAPI_);
        }
      });
}

void CDPAgentImpl::handleCommand(std::string json) {
  std::shared_ptr<message::Request> command = message::Request::fromJson(json);
  if (!command) {
    // Can't even parse the command to get the command ID, so there's no ID
    // to respond to with an error message.
    return;
  }

  // Call DomainAgents::handleCommand on the runtime thread.
  auto weakThis = std::weak_ptr<CDPAgentImpl>(shared_from_this());
  runtimeTaskRunner_.enqueueTask(
      [weakThis = std::move(weakThis),
       command = std::move(command)](HermesRuntime &) {
        if (auto strongThis = weakThis.lock()) {
          strongThis->domainAgents_->handleCommand(
              strongThis->runtime_,
              strongThis->asyncDebuggerAPI_,
              std::move(command));
        }
      });
}

CDPAgentImpl::DomainAgents::DomainAgents(OutboundMessageFunc messageCallback)
    : messageCallback_(messageCallback) {}

void CDPAgentImpl::DomainAgents::initialize(
    HermesRuntime &runtime,
    AsyncDebuggerAPI &asyncDebuggerAPI) {
  // TODO: create domain-specific handlers
}

void CDPAgentImpl::DomainAgents::dispose() {
  // TODO: dispose domain-specific handlers
}

void CDPAgentImpl::DomainAgents::handleCommand(
    HermesRuntime &runtime,
    AsyncDebuggerAPI &asyncDebuggerAPI,
    std::shared_ptr<message::Request> command) {
  size_t domainLength = command->method.find('.');
  if (domainLength == std::string::npos) {
    messageCallback_(message::makeErrorResponse(
                         command->id,
                         message::ErrorCode::ParseError,
                         "Malformed domain '" + command->method + "'")
                         .toJsonStr());
    return;
  }
  std::string domain = command->method.substr(0, domainLength);

  // TODO: dispatch to the appropriate domain handler

  messageCallback_(message::makeErrorResponse(
                       command->id,
                       message::ErrorCode::MethodNotFound,
                       "Unsupported domain '" + command->method + "'")
                       .toJsonStr());
}

std::unique_ptr<CDPAgent> CDPAgent::create(
    HermesRuntime &runtime,
    debugger::AsyncDebuggerAPI &asyncDebuggerAPI,
    EnqueueRuntimeTaskFunc enqueueRuntimeTaskCallback,
    OutboundMessageFunc messageCallback) {
  return std::unique_ptr<CDPAgent>(new CDPAgent(
      runtime, asyncDebuggerAPI, enqueueRuntimeTaskCallback, messageCallback));
}

CDPAgent::CDPAgent(
    HermesRuntime &runtime,
    debugger::AsyncDebuggerAPI &asyncDebuggerAPI,
    EnqueueRuntimeTaskFunc enqueueRuntimeTaskCallback,
    OutboundMessageFunc messageCallback)
    : impl_(std::make_shared<CDPAgentImpl>(
          runtime,
          asyncDebuggerAPI,
          enqueueRuntimeTaskCallback,
          messageCallback)) {
  impl_->initializeDomainAgents();
}

CDPAgent::~CDPAgent() {}

void CDPAgent::handleCommand(std::string json) {
  impl_->handleCommand(json);
}

} // namespace cdp
} // namespace hermes
} // namespace facebook
