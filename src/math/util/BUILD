cc_library(
    name = "prime",
    srcs = [
        "prime.cpp",
    ],
    hdrs = [
        "prime.h",
    ],
    visibility = [
        "//interp:__pkg__",
        "//math:__pkg__",
        "//math/field:__pkg__",
        "//run:__pkg__",
    ],
    deps = [
        ":modular-x86",
        ":random",
        "//external:boost-config",
        "//math:base",
    ],
)

cc_library(
    name = "random",
    hdrs = [
        "random.h",
    ],
    visibility = [
        "//math/field:__pkg__",
        "//run:__pkg__",
    ],
    deps = [
        "//external:boost-random",
    ],
)

cc_library(
    name = "modular-x86",
    hdrs = [
        "modular-x86.h",
    ],
    visibility = [
        "//math:__pkg__",
        "//math/field:__pkg__",
    ],
    deps = [
        "//external:boost-smart_ptr",
        "//math:base",
        "//util:smart_assert",
    ],
)

cc_library(
    name = "static",
    hdrs = [
        "static.h",
    ],
    visibility = [
        "//math/field:__pkg__",
    ],
    deps = [
        "//external:boost-static_assert",
    ],
)
