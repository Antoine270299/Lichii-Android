# Lichii for Android 0.2.0

Tested on a Samsung Galaxy S26 (Exynos 2600, Xclipse 960, 12 GB, Android 17) with a custom Mesa RADV driver.

## Highlights

- **Pokémon Pokopia** is playable at about **40 FPS** at 0.5× resolution, even in the busy outdoor scenes.
  - The multi-second drops to about 5 FPS are gone. Buffers the GPU writes are now copied back at fences, instead of making the GPU thread wait for the GPU hundreds of times a second.
  - Pokopia presents a frame ahead, so the CPU prepares the next frame while the GPU finishes the current one.
  - **Known issue:** the sky is black, with only the clouds drawn. No other emulator draws it on this phone either. This is being worked on.
- **New setting: Present a frame ahead** (Graphics → Advanced, also per game). It can be Auto, Off or On.
  - Auto picks per game.
  - On usually raises the frame rate in GPU-heavy games, at the cost of one frame of input latency.
- **Game folders:** extracted games can now be listed and launched from a folder, not just from a single file. A folder needs `exefs/main` and can include numbered update folders. These games need the "All files access" permission.
- **The Legend of Zelda: Ocarina of Time** gets past the hang after the intro. Gameplay needs about 3.7 GB of GPU memory, so on a 12 GB phone Android stops the game for low memory shortly after gameplay starts. Phones with more RAM may run it.
- Fixed a crash when the shader-build progress dialog outlived the game screen.
- Includes the latest Lichii and Eglantine rendering, shader and performance fixes since 0.1.0.

## For testers

- Install the APK, add your games folder, and set up keys and firmware as usual. None are included.
- Recommended: Handheld mode, NCE CPU backend, 0.5× to 1× resolution, and a custom RADV driver on Xclipse GPUs.
- Diagnostic switches can be set in `Android/data/dev.lichii.android/files/env.txt` (see the README).
- Report problems at https://github.com/pavelgajdo/Lichii-Android/issues with your phone, driver and `eden_log.txt`.
