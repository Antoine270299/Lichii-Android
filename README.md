<!--
# SPDX-FileCopyrightText: Copyright 2025 Eden Emulator Project
# SPDX-License-Identifier: GPL-3.0-or-later

# SPDX-FileCopyrightText: 2018 yuzu Emulator Project
# SPDX-License-Identifier: GPL-2.0-or-later
-->

<p align="center">
  <img src="dist/branding/lichii_icon_with_logo.png" alt="Lichii" width="600">
</p>

<p align="center"><img src="docs/brand/hexnostalgia.png" alt="HexNostalgia" width="180"></p>

# Lichii for Android — Snapdragon branch

**HexNostalgia's fork of [Lichii for Android](https://github.com/pavelgajdo/Lichii-Android), tuned for Snapdragon phones with Adreno 8xx GPUs.** It follows one goal: getting Switch 2 games, starting with *The Legend of Zelda: Ocarina of Time*, playable on a Snapdragon 8 Elite phone.

This is an independent, experimental fork. It is not affiliated with Nintendo, Qualcomm or the upstream projects it builds on. Games, keys and firmware are not included: only use games and system files you have legally dumped from your own console.

## Download

- **Test builds:** the [`android-latest`](https://github.com/HexNostalgia/Lichii-Android/releases/tag/android-latest) pre-release is rebuilt automatically from every change on `nx2`. Download `app-mainline-release.apk` and install it.
- **Merge test builds:** [`android-sync`](https://github.com/HexNostalgia/Lichii-Android/releases/tag/android-sync) is built from `nx2-sync`, a merge with Zortos' latest Lichii. It is not ready for play (see below).

### Required: the patched Turnip driver

Use **HN Turnip** ([releases](https://github.com/HexNostalgia/AdrenoToolsDrivers/releases); the first build is [`linear_v1`](https://github.com/HexNostalgia/AdrenoToolsDrivers/releases/tag/linear_v1)) (Mesa Turnip from whitebelyash's Adreno 8xx branch, plus one display fix), installed through the app's GPU driver manager.

Why: on Android 17 with Qualcomm's new SnapAlloc allocator, Turnip can't ask Android how screen buffers are laid out and falls back to guessing. It guesses "linear", Android picks a compressed tiled layout, and every emulator using Turnip shows red bands or striped images. The patch makes Turnip ask for CPU-readable screen buffers in that fallback case, which forces the linear layout it assumes. The phone's own Qualcomm driver is not supported here (it can't run the 64-bit integer shaders Ocarina of Time needs).

## Tested on

| | |
|---|---|
| Phone | Samsung Galaxy Z Fold 8 (SM-F971B) |
| SoC | Snapdragon 8 Elite Gen 5 (SM8850) |
| GPU | Adreno 840 |
| RAM | 16 GB |
| OS | Android 17 (One UI) |
| GPU driver | Turnip linear swapchain fix (Mesa 26.2.99, whitebelyash branch + patch) |

## Ocarina of Time (Switch 2, v2.0.0) — status

| | |
|---|---|
| Boot, intro movies, title screen | Yes, 60 FPS |
| Kokiri Forest / Deku Tree | Playable at about **17 FPS** (it started at about 3 FPS) |
| Foliage drawn with mesh shaders | Off by default on the phone (`EDEN_NX2_NO_MESH=1`): with it on, the Deku Tree scene runs out of memory |
| Known issues | The picture slowly turns white (auto-exposure climbs too high); the intro movie can lag behind its sound |

Recommended for Ocarina of Time: keep the **Max Resolution** add-on enabled (the network bypass below expects it), and put `EDEN_NX2_NO_MESH=1` in `env.txt`.

## What this branch changes

- **Ocarina of Time's anti-aliasing network is skipped.** The game runs a tensor-core neural network on every frame; emulated on a phone it costs hundreds of milliseconds. This branch copies the tonemapped image straight to the network's output instead.
- **Faster global memory access in shaders.** Shaders that read the same storage buffers many times now look up each buffer's address once. On the Deku Tree scene this took the game from about 3 to about 17 FPS.
- **Fewer render-pass breaks** on tiled Adreno GPUs (`EDEN_NX2_DEFER_WFI`).
- **Smoother GPU thread** (short sleeps with a 1 µs timer slack), ported from Zortos' Lichii (#446 by ne1i).
- **Mesh shader emulation** from Zortos' Lichii, with memory caps for phones.
- **Microphone fix (on `nx2-sync`).** Ocarina of Time opens a Switch 2 microphone stream before its first frame; on Android this waited forever for a permission and left a black screen. The emulator now gives the game a silent microphone.
- **Diagnostic tools:** per-pass GPU timings, render-pass and finish reports, a logcat stream to a file, and `env.txt` / Freedreno-page switches (listed in [AGENTS.md](AGENTS.md#debugging-tools)).

## Reporting problems

Open an issue with your phone, Android version, GPU driver, the game and what happened (screenshots or a short screen recording help), and the session's `Android/data/dev.lichii.android/files/log/eden_log.txt`. Check logs for personal information before uploading. Never upload games, keys, firmware or account data.

## Credits

This branch stands on the work of [pavelgajdo](https://github.com/pavelgajdo) (Lichii for Android), [Zortos](https://github.com/zortos293) (Lichii), whitebelyash (Turnip builds), ne1i, Dark (Dynamic FPS), and the Eglantine, Eden, yuzu and Sudachi projects and their contributors. Original copyright notices and third-party licenses are retained.

---

*The upstream README of Lichii for Android follows.*

## Upstream: Lichii for Android

An Android port of the [Lichii](https://github.com/zortos293/Eglantine) Nintendo Switch / Switch 2 emulator, maintained by [pavelgajdo](https://github.com/pavelgajdo). It focuses on getting Switch 2 games running on Android phones, starting with Samsung's Exynos phones and their AMD-based Xclipse GPUs.

This is an independent, experimental fork. It is not affiliated with Nintendo or with the upstream projects it builds on.

### Download

- **Releases:** [github.com/pavelgajdo/Lichii-Android/releases](https://github.com/pavelgajdo/Lichii-Android/releases). Download the `.apk` from the latest `android-*` release and install it on your phone. You may need to allow installs from your browser or file manager.
- **Test builds:** the `android-latest` pre-release is rebuilt automatically from every change on `nx2`. It has the newest fixes and the newest bugs.

Requirements: an arm64 phone with Android 13 or newer and Vulkan 1.3. Games, keys and firmware are not included. Only use games and system files you have legally obtained.

### Tested on

| | |
|---|---|
| Phone | Samsung Galaxy S26 (SM-S942B) |
| SoC | Exynos 2600 |
| GPU | Samsung Xclipse 960 (AMD RDNA3, GFX11) |
| RAM | 12 GB |
| OS | Android 17 (One UI) |
| GPU driver | Custom Mesa RADV build for Xclipse ("RADV Xclipse", Mesa 27.0-devel), installed through the app's GPU driver manager |

Samsung's own driver on the S26 is also RADV-based and works, but on this phone it measured about 1.8× slower on the same scene than the custom RADV build.

Other phones may work, but this is the only device the fork is tested on so far.

> 📺 **Game Testing & Video Footage:** Check out gameplay performance tests and video showcases on the [Exynos Gaming YouTube Channel](https://www.youtube.com/@ExynosGaming-t7k).

### Game status

| Game | Status |
|---|---|
| The Legend of Zelda: Ocarina of Time (Switch 2) | Boots. The title screen and the pre-rendered intro movies run at 60 FPS. **It currently hangs while loading the first gameplay scene after the intro** (being worked on). |

Only results tested on the phone above are listed here.

#### What's different from upstream

- AV1 movies are decoded with Android's hardware decoder (MediaCodec).
- Ocarina of Time's movies skip the game's movie-enhancement neural network (which needs NVIDIA tensor cores), so they play at full speed instead of about 8 FPS.
- Emulated tensor-core math (HMMA) is about twice as fast on RADV.
- `EDEN_*` diagnostic switches can be set on Android with an `env.txt` file (see below).

### Recommended settings

- **System:** Handheld mode.
- **CPU:** NCE backend.
- **Graphics:** 1× resolution. Install a custom RADV driver if your phone has an Xclipse GPU.

### Reporting problems

[Open an issue](https://github.com/pavelgajdo/Lichii-Android/issues) with:

- your phone model, Android version and GPU driver;
- the game and what happened (screenshots help);
- the log from the affected session: `Android/data/dev.lichii.android/files/log/eden_log.txt` (`dev.lichii.android.relWithDebInfo` for test builds).

Check logs for personal information before uploading. Never upload games, keys, firmware or account data.

#### Diagnostic switches

Android can't pass environment variables to an app, so Lichii reads them from `Android/data/<app id>/files/env.txt`, one `KEY=VALUE` per line (`#` starts a comment). They take effect the next time the app starts. The available switches are listed in [AGENTS.md](AGENTS.md#debugging-tools). Leave the file empty for normal play: most switches slow the emulator down.

### Building

```bash
cd src/android
./gradlew assembleMainlineRelWithDebInfo
```

You need the Android SDK, NDK 28.2 and CMake 3.22. The APK is written to `src/android/app/build/outputs/apk/mainline/relWithDebInfo/`. CI builds every push (see [.github/workflows/android.yml](.github/workflows/android.yml)). Pushing a tag named `android-<version>` publishes a release with the APK.

More documentation, inherited from upstream: [build guide](docs/Build.md), [coding guidelines](docs/Coding.md).

### Credits and license

Lichii for Android builds on [Lichii](https://github.com/zortos293/Eglantine) by Zortos and on Eglantine, Eden, yuzu and Sudachi, and their contributors, including Eglantine's original developer Camille LaVey. Lichii artwork by YOB (yobslab). Original copyright notices and third-party licenses are retained.

See [LICENSE.txt](LICENSE.txt) and the source files for the applicable license terms.
