// SPDX-FileCopyrightText: Copyright 2026 Eden Emulator Project
// SPDX-License-Identifier: GPL-3.0-or-later

// Android has no way to pass environment variables to an app, so the EDEN_* diagnostic switches
// are read from <external files dir>/env.txt, one KEY=VALUE per line ('#' starts a comment).
// Several switches are read by static initializers, so this runs before them.

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

#include <android/log.h>

namespace {

std::string PackageName() {
    char name[256]{};
    if (FILE* file = std::fopen("/proc/self/cmdline", "rb")) {
        const size_t n = std::fread(name, 1, sizeof(name) - 1, file);
        name[n] = '\0';
        std::fclose(file);
    }
    // A process of the app may carry a ":service" suffix.
    if (char* colon = std::strchr(name, ':')) {
        *colon = '\0';
    }
    return name;
}

std::string Trim(const std::string& s) {
    const size_t begin = s.find_first_not_of(" \t\r\n");
    if (begin == std::string::npos) {
        return {};
    }
    const size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(begin, end - begin + 1);
}

void LoadFile(const std::string& path, const char* label) {
    FILE* file = std::fopen(path.c_str(), "r");
    if (file == nullptr) {
        return;
    }
    char buffer[1024];
    while (std::fgets(buffer, sizeof(buffer), file) != nullptr) {
        std::string line = Trim(buffer);
        if (line.empty() || line[0] == '#') {
            continue;
        }
        const size_t eq = line.find('=');
        if (eq == std::string::npos || eq == 0) {
            continue;
        }
        const std::string key = Trim(line.substr(0, eq));
        const std::string value = Trim(line.substr(eq + 1));
        setenv(key.c_str(), value.c_str(), 1);
        __android_log_print(ANDROID_LOG_WARN, "YuzuNative", "%s: %s=%s", label, key.c_str(),
                            value.c_str());
    }
    std::fclose(file);
}

__attribute__((constructor(101))) void LoadEnvFile() {
    const std::string package = PackageName();
    if (package.empty()) {
        return;
    }
    // The variables saved on the Freedreno settings page (same KEY=VALUE lines) are applied by
    // the app only after the static initializers have run: read them here as well, so switches
    // set from the phone work like the ones in env.txt, which comes last and wins.
    LoadFile("/data/data/" + package + "/cache/.freedreno.conf", "freedreno.conf");
    LoadFile("/storage/emulated/0/Android/data/" + package + "/files/env.txt", "env.txt");
}

} // namespace
