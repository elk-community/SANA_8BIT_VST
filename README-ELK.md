# SANA 8bit

Modified for headless plugin build for [Elk Audio OS](https://elk.audio)

## Building Instructions

1. Open `SANA_8BIT_VST.jucer` with the projucer.
2. Enter the path to the VST2 SDK in the field "VST (LEGACY) SDK Folder" in the exporters menu and save to generate the Linux Makefile.
3. Navigate to the folder `OB-Xd/Builds/LinuxMakefile/`.
4. Set up the cross-compilation toolchain with:

   ```bash
   $ unset LD_LIBRARY_PATH
   $ source /path/to/environment-setup-cortexa7t2hf-neon-vfpv4-elk-linux-gnueabi
   ```

5. (optional) Add flags for more aggressive optimizations than the default ones from the toolchain with:  

   ```bash
   $ export CXXFLAGS="-O3 -pipe -ffast-math -feliminate-unused-debug-types -funroll-loops -mvectorize-with-neon-quad"
   ```

6. Finally cross compile the plugin using:

   ```bash
   $ AR=arm-elk-linux-gnueabi-ar make -j`nproc` CONFIG=Release CFLAGS="-DJUCE_HEADLESS_PLUGIN_CLIENT=1" TARGET_ARCH="-mcpu=cortex-a53 -mtune=cortex-a53 -mfpu=neon-vfpv4 -mfloat-abi=hard"
   ```

## Additional notes

* Make sure to use our [JUCE fork](https://github.com/elk-audio/JUCE/tree/mind/headless_plugin_client) with the branch `mind/headless_plugin_client`.
* For further compilation help. Look at our [documentation](https://github.com/elk-audio/elk-docs/blob/master/documents/building_plugins_for_elk.md).
