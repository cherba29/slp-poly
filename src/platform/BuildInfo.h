#ifndef SLP_POLY_PLATFORM_BUILD_INFO_H
#define SLP_POLY_PLATFORM_BUILD_INFO_H

#include <time.h>

namespace platform {

// Value of --embed_label when running bazel build.
extern const char* kBuildEmbedLabel;

// Host where the build was performed.
extern const char* kBuildHost;

// User who performed the build.
extern const char* kBuildUser;

// SCM revision used to build the binary.
extern const char* kBuildScmRevision;

// SCM status(clean or modified) when the build is performed.
extern const char* kBuildScmStatus;

// Epoch time when the build was performed.
extern const time_t kBuildTimestamp;

// Build target for this binary, like "//base:some_binary"
extern const char* kBazelTargetName;

// Cpp platform used for this build process, like "local_linux"
extern const char* kBazelPlatform;

}  // namespace platform

#endif  // SLP_POLY_PLATFORM_BUILD_INFO_H