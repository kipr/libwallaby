A `Device` implementation for Emscripten that uses Emscripten's `EM_JS` feature. The host is expected to provide a `onRegisterChange` function and `registers` buffer to the `Module`'s `context`. 

`EM_JS` is not supported when using Emscripten's `SIDE_MODULE` feature, which is required for libkipr's usage with `cpython` in the browser. Please see `emscripten_fs` for an alternate implementation used with `cpython`.
