cc_library(
    name = "platform",
    srcs = [
        "Platform.cpp",
    ],
    hdrs = [
        "Platform.h",
    ],
    visibility = ["//:__pkg__"],
    defines = ["BOOST_BIND_GLOBAL_PLACEHOLDERS"],
    deps = [
        ":build_info",
        "@boost//:date_time",
    ],
)

cc_library(
    name = "build_info",
    hdrs = [
        "BuildInfo.h",
    ],
    linkstamp = "BuildInfo.cpp",
)
