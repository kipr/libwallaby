# `module`

Modules are components of the KIPR library. Most modules (e.g., C/C++) conform to the following directory structure:

  - `/`
    - `public` - Public includes. These includes are visible to end users of the KIPR library.
    - `protected` - "Protected" includes. These includes are visible to other modules that depend on this one.
    - `src` - Source code.


