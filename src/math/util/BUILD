cc_library(
    name = "prime",
    hdrs = [
        "prime.h",
    ],
    srcs = [
        "prime.cpp",
    ],
    deps = [
        ":modular-x86",
        ":random",
        "//math:base",
        "//external:boost-config",
    ],
    visibility = [
        "//interp:__pkg__",
        "//math:__pkg__",
        "//math/field:__pkg__",
        "//run:__pkg__",
    ],
)    

cc_library(
    name = "random",
    hdrs = [
        "random.h",
    ],
    deps = [
        "//external:boost-random",
    ],
    visibility = [
        "//math/field:__pkg__",
        "//run:__pkg__",
    ],
)    

cc_library(
    name = "modular-x86",
    hdrs = [
        "modular-x86.h",
    ],
    deps = [
        "//math:base",
        "//external:boost-smart_ptr",
        "//util:smart_assert",
    ],
    visibility = [
        "//math:__pkg__",
        "//math/field:__pkg__",
    ],
)

cc_library(
    name = "static",
    hdrs = [
        "static.h",
    ],
    deps = [
        "//external:boost-static_assert",
    ],
    visibility = [
        "//math/field:__pkg__",
    ],
)    
