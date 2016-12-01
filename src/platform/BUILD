cc_library(
    name = "platform",
    hdrs = [
        "Platform.h"
    ],
    srcs = [
        "Platform.cpp",
    ],
    deps = [
        ":build_info",
        "//external:boost-date_time",
    ],
    visibility = ["//:__pkg__",]
)

cc_library(
    name = "build_info",
    hdrs = [
        "BuildInfo.h"
    ],
    linkstamp = "BuildInfo.cpp",
)