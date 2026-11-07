# Lichii for Android 0.3.0

Tested on a Samsung Galaxy S26 (Exynos 2600, Xclipse 960, 12 GB, Android 17) with a custom Mesa RADV driver.

## Highlights

- **The Legend of Zelda: Ocarina of Time** now reaches gameplay. Kokiri Forest runs at about **15–20 FPS** at 0.5× resolution, up from about 2 FPS.
  - Dark's Dynamic FPS patch is built in, so the game runs at full speed at any frame rate. It used to crash the game while compiling its first shaders under the NCE CPU backend.
  - The freeze after the first scene loaded is fixed. The game's lighting passes had been sized with garbage counts and ran for seconds each.
  - The GPU's command processor no longer stops both GPU threads for every read of GPU-written memory (macro DMA read snapshots, after EvilOOT's fix as ported by Frizti).
  - The few memory pages the game's CPU reads or writes after the GPU wrote them are now copied back at each fence. The CPU no longer waits for the GPU on each access.
  - **Recommended:** a phone with **16 GB of RAM**. On 12 GB, Android may close the game for low memory while a scene loads, especially with other apps open.
  - **Known issues:**
    - Some scenes sometimes show white glare and magenta patches.
    - The phone heats up during long sessions and slows down until it cools.
- **Lower memory use while scenes load** (all games):
  - ASTC textures are converted to BC a few layers at a time instead of whole.
  - Large upload buffers are freed once the GPU is done with them, and capped while a scene loads.
  - On Android, converted ASTC textures keep their mip levels up to 1024 px.
  - The game-file decompression cache is capped at 128 MB.
- **Pragmata** no longer quits at startup with error 2153-0513. It needed an audio command we refused. It does not render yet.
- **Pokémon Pokopia** keeps its copy-back with downloads in flight capped at 512 MB, and runs slightly faster.
- Includes the latest Lichii (Zortos) fixes, among them the split of misaligned 64/128-bit storage buffer accesses and a fix for a lost worker wake-up.

## For testers

- Install the APK, add your games folder, and set up keys and firmware as usual. None are included.
- Recommended: Handheld mode, NCE CPU backend, 0.5× resolution, and a custom RADV driver on Xclipse GPUs.
- Diagnostic switches can be set in `Android/data/dev.lichii.android/files/env.txt` (see the README).
- Report problems at https://github.com/pavelgajdo/Lichii-Android/issues with your phone, driver and `eden_log.txt`.
