load("//third_party:cppenum.bzl", "gen_cppenum")

gen_cppenum(
    enum_file="LevelEnum.yaml",
)

gen_cppenum(
    enum_file="LogLevelEnum.yaml",
    visibility = ["//runprofile:__pkg__",],
)

gen_cppenum(
    enum_file="LogModuleEnum.yaml",
    visibility = ["//runprofile:__pkg__",],
)

gen_cppenum(
    enum_file="LogTagEnum.yaml",
    visibility = ["//runprofile:__pkg__",],
)

cc_library(
    name = "log",
    srcs = ["log.cpp"],
    hdrs = ["log.h"],
    deps = [
        ":LogLevelEnum",
        ":LogModuleEnum",
        ":LogTagEnum",
        "//external:boost-core",
        "//external:boost-date_time",
        "//external:boost-filesystem",
        "//external:boost-log",
    ],
    visibility = [
        "//:__pkg__",
        "//complexity:__pkg__",
        "//context:__pkg__",
        "//context/maple:__pkg__",
        "//eval/util:__pkg__",
        "//interp:__pkg__",
        "//math/fft:__pkg__",
        "//runprofile:__pkg__",
        "//run:__pkg__",
        "//operation/field:__pkg__",
    ]
)

cc_library(
    name = "multi_index_map",
    srcs = ["MultiIndexMap.cpp"],
    hdrs = ["MultiIndexMap.h"],
    deps = [
        "//external:boost-variant",
    ],
    visibility = [
        "//:__pkg__",
        "//run:__pkg__",
        "//runprofile:__pkg__",
    ]
)

cc_test(
    name = "multi_index_map_test",
    srcs = ["MultiIndexMap.t.cpp",],
    deps = [
        ":multi_index_map",
        "//external:boost-test",
    ],
)

cc_library(
    name = "smart_assert",
    hdrs = [
        "SmartAssert.h",
    ],
    srcs = [
        "SmartAssert.cpp",
    ],
    deps = [
        ":LevelEnum",
        "//external:boost-lexical_cast",
    ],
    visibility = [
        "//algebra:__pkg__",
        "//context:__pkg__",
        "//eval/util:__pkg__",
        "//math/fft:__pkg__",
        "//math/util:__pkg__",
        "//math/vandermonde:__pkg__",
        "//run:__pkg__",
        "//runprofile:__pkg__",
    ]
)

cc_library(
    name = "timer",
    hdrs = [
        "Timer.h",
    ],
    srcs = [
        "Timer.cpp",
    ],
    visibility = [
        "//:__pkg__",
        "//interp:__pkg__",
    ]
)