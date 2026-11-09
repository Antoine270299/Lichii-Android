// SPDX-FileCopyrightText: Copyright 2026 Eden Emulator Project
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <array>
#include <algorithm>
#include <atomic>
#include <mutex>
#include "common/common_types.h"

// Ocarina of Time shows its pre-rendered movies through a compute chain: one pass turns the
// decoded NV12 frame into a 1920x1080 RGBA8 image, a tensor network (HMMA) refines it, and a last
// pass upscales the result to the 2560x1440 RGBA8 image the scene composites. The network only
// sharpens the frame, and emulating its matrix multiplies costs a phone GPU ~50 ms a frame, so the
// RGB frame is scaled into the output image instead.
namespace VideoCommon::OcarinaMovie {

constexpr u64 ProgramId = 0x0400486024DEC000ULL;

// Native program keys.
constexpr u64 ConvertKey = 0xf9e5b02b5fabff78ULL;
constexpr std::array NetworkKeys{
    0x2f5791cea2a85397ULL, 0x313d724f3e214203ULL, 0xdf2c83cf2ad70790ULL,
    0x1fdb95ea2565f49cULL, 0x0be2f024aea21a6cULL, 0x0618cc4a3ac8f33aULL,
    0xe27336d38f1cc8f4ULL, 0xecebb02dec6faa17ULL, 0x2a366b38546a4d5eULL,
};
constexpr u64 OutputKey = 0x6093c9143ba0904dULL;

// Constant bank 0 offsets of the image handles the passes write.
constexpr u32 ConvertImageOffset = 0x68;
constexpr u32 OutputImageOffset = 0xc8;

// Gameplay frames: one tensor pass filters the 1920x1080 scene colour (E5B9G9R9, read through
// the texture at constant buffer 0 offset 0xc0) into the image the frame continues with
// (B10G11R11, written through the image at 0xd0). Emulating its matrix multiplies costs a phone GPU
// ~220 ms a frame; copying the scene colour in its place keeps the frame without the filter.
constexpr u64 GameplayFilterKey = 0xda6df3e54c716df2ULL;
constexpr u32 GameplayFilterInputOffset = 0xc0;
constexpr u32 GameplayFilterOutputOffset = 0xd0;

constexpr bool IsNetwork(u64 native_key) {
    return std::ranges::find(NetworkKeys, native_key) != NetworkKeys.end();
}


// Gameplay anti-aliasing (after zortos293/Lichii docs/nx2/oot-network-bypass.md): the game
// tonemaps the scene into a B10G11R11 render target, then runs a prepass, a tensor network and an
// output pass (GameplayFilterKey) that writes the image the rest of the frame uses. The output
// pass is replaced by a point copy of the tonemapped image, and the prepass and the network are
// skipped while it is.
constexpr u64 GameplayPrepassKey = 0x8475d671e6967428ULL;
constexpr u64 GameplayNetworkKey = 0x42593194bb0b2b32ULL;
constexpr bool IsTonemapShader(u64 hash) {
    return hash == 0xbded5ce917183214ULL || // outdoors
           hash == 0x6cb06edf14be1d79ULL || // indoors
           hash == 0x3ac011bd2d772994ULL;   // outdoors at another time of day
}
/// Render target 0 of the latest tonemap draw, written by the rasterizer, read by the compute
/// engine (address 0 until one is seen).
struct TonemappedImage {
    u64 address{};
    u32 width{};
    u32 height{};
    u32 format{}; ///< Tegra::RenderTargetFormat
    u32 block_width{};
    u32 block_height{};
    u32 block_depth{};
    bool pitch_linear{};
};
inline std::mutex tonemapped_mutex;
inline TonemappedImage tonemapped_image;
inline std::atomic<bool> gameplay_output_replaced{};

} // namespace VideoCommon::OcarinaMovie
