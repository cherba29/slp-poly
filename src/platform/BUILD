cc_library(
    name = "platform",
    srcs = [
        "Platform.cpp",
    ],
    hdrs = [
        "Platform.h",
    ],
    visibility = ["//:__pkg__"],
    deps = [
        ":build_info",
        "//external:boost-date_time",
    ],
)

cc_library(
    name = "build_info",
    hdrs = [
        "BuildInfo.h",
    ],
    linkstamp = "BuildInfo.cpp",
)
