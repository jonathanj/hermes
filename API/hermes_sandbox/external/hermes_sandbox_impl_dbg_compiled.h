/* Automatically generated by wasm2c */
#ifndef HERMES_SANDBOX_IMPL_DBG_COMPILED_H_GENERATED_
#define HERMES_SANDBOX_IMPL_DBG_COMPILED_H_GENERATED_

#include "wasm-rt.h"

#include <stdint.h>

#ifndef WASM_RT_CORE_TYPES_DEFINED
#define WASM_RT_CORE_TYPES_DEFINED
typedef uint8_t u8;
typedef int8_t s8;
typedef uint16_t u16;
typedef int16_t s16;
typedef uint32_t u32;
typedef int32_t s32;
typedef uint64_t u64;
typedef int64_t s64;
typedef float f32;
typedef double f64;
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct w2c_env;
struct w2c_hermes__import;
struct w2c_wasi__snapshot__preview1;

typedef struct w2c_hermes {
  struct w2c_env* w2c_env_instance;
  struct w2c_hermes__import* w2c_hermes__import_instance;
  struct w2c_wasi__snapshot__preview1* w2c_wasi__snapshot__preview1_instance;
  u32 w2c_0x5F_stack_pointer;
  u32 w2c_0x5F_stack_end;
  u32 w2c_0x5F_stack_base;
  wasm_rt_memory_t w2c_memory;
  wasm_rt_funcref_table_t w2c_0x5F_indirect_function_table;
} w2c_hermes;

void wasm2c_hermes_instantiate(w2c_hermes*, struct w2c_env*, struct w2c_hermes__import*, struct w2c_wasi__snapshot__preview1*);
void wasm2c_hermes_free(w2c_hermes*);
wasm_rt_func_type_t wasm2c_hermes_get_func_type(uint32_t param_count, uint32_t result_count, ...);

/* import: 'env' '__syscall_lstat64' */
u32 w2c_env_0x5F_syscall_lstat64(struct w2c_env*, u32, u32);

/* import: 'env' '__syscall_newfstatat' */
u32 w2c_env_0x5F_syscall_newfstatat(struct w2c_env*, u32, u32, u32, u32);

/* import: 'env' '__syscall_stat64' */
u32 w2c_env_0x5F_syscall_stat64(struct w2c_env*, u32, u32);

/* import: 'env' '__syscall_unlinkat' */
u32 w2c_env_0x5F_syscall_unlinkat(struct w2c_env*, u32, u32, u32);

/* import: 'env' 'emscripten_notify_memory_growth' */
void w2c_env_emscripten_notify_memory_growth(struct w2c_env*, u32);

/* import: 'hermes_import' 'getentropy' */
u32 w2c_hermes__import_getentropy(struct w2c_hermes__import*, u32, u32);

/* import: 'wasi_snapshot_preview1' 'clock_time_get' */
u32 w2c_wasi__snapshot__preview1_clock_time_get(struct w2c_wasi__snapshot__preview1*, u32, u64, u32);

/* import: 'wasi_snapshot_preview1' 'environ_get' */
u32 w2c_wasi__snapshot__preview1_environ_get(struct w2c_wasi__snapshot__preview1*, u32, u32);

/* import: 'wasi_snapshot_preview1' 'environ_sizes_get' */
u32 w2c_wasi__snapshot__preview1_environ_sizes_get(struct w2c_wasi__snapshot__preview1*, u32, u32);

/* import: 'wasi_snapshot_preview1' 'fd_close' */
u32 w2c_wasi__snapshot__preview1_fd_close(struct w2c_wasi__snapshot__preview1*, u32);

/* import: 'wasi_snapshot_preview1' 'fd_fdstat_get' */
u32 w2c_wasi__snapshot__preview1_fd_fdstat_get(struct w2c_wasi__snapshot__preview1*, u32, u32);

/* import: 'wasi_snapshot_preview1' 'fd_seek' */
u32 w2c_wasi__snapshot__preview1_fd_seek(struct w2c_wasi__snapshot__preview1*, u32, u64, u32, u32);

/* import: 'wasi_snapshot_preview1' 'fd_write' */
u32 w2c_wasi__snapshot__preview1_fd_write(struct w2c_wasi__snapshot__preview1*, u32, u32, u32, u32);

/* import: 'wasi_snapshot_preview1' 'proc_exit' */
void w2c_wasi__snapshot__preview1_proc_exit(struct w2c_wasi__snapshot__preview1*, u32);

/* export: 'memory' */
wasm_rt_memory_t* w2c_hermes_memory(w2c_hermes* instance);

/* export: 'get_hermes_abi_vtable' */
u32 w2c_hermes_get_hermes_abi_vtable(w2c_hermes*);

/* export: '_ZNK4llvh3cfg6UpdateIPN6hermes10BasicBlockEE4dumpEv' */
void w2c_hermes_0x5FZNK4llvh3cfg6UpdateIPN6hermes10BasicBlockEE4dumpEv(w2c_hermes*, u32);

/* export: '_ZNK4llvh7APFloat4dumpEv' */
void w2c_hermes_0x5FZNK4llvh7APFloat4dumpEv(w2c_hermes*, u32);

/* export: '_ZNK4llvh5APInt4dumpEv' */
void w2c_hermes_0x5FZNK4llvh5APInt4dumpEv(w2c_hermes*, u32);

/* export: '_ZNK4llvh5Twine4dumpEv' */
void w2c_hermes_0x5FZNK4llvh5Twine4dumpEv(w2c_hermes*, u32);

/* export: '_ZNK4llvh5Twine8dumpReprEv' */
void w2c_hermes_0x5FZNK4llvh5Twine8dumpReprEv(w2c_hermes*, u32);

/* export: '_initialize' */
void w2c_hermes_0x5Finitialize(w2c_hermes*);

/* export: '__indirect_function_table' */
wasm_rt_funcref_table_t* w2c_hermes_0x5F_indirect_function_table(w2c_hermes* instance);

/* export: '__errno_location' */
u32 w2c_hermes_0x5F_errno_location(w2c_hermes*);

/* export: 'malloc' */
u32 w2c_hermes_malloc(w2c_hermes*, u32);

/* export: 'free' */
void w2c_hermes_free(w2c_hermes*, u32);

/* export: 'realloc' */
u32 w2c_hermes_realloc(w2c_hermes*, u32, u32);

/* export: 'stackSave' */
u32 w2c_hermes_stackSave(w2c_hermes*);

/* export: 'stackRestore' */
void w2c_hermes_stackRestore(w2c_hermes*, u32);

/* export: 'stackAlloc' */
u32 w2c_hermes_stackAlloc(w2c_hermes*, u32);

#ifdef __cplusplus
}
#endif

#endif  /* HERMES_SANDBOX_IMPL_DBG_COMPILED_H_GENERATED_ */
