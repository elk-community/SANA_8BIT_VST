# SANA 8Bit VST Plugin for Elk Audio OS

## Build Instructions

Standard JUCE Cross-compilation build:

```bash
$ source [path-to-cross-compiling-sdk-environment-activation-script]
$ cmake -DCMAKE_BUILD_TYPE=Release -B build -S .
$ cmake --build build --parallel --config Release
```

Copyright 2017-2026 Elk Audio AB, Stockholm, Sweden.
