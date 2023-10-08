load("//third_party:cppenum.bzl", "gen_cppenum")

gen_cppenum(
    enum_file = "LevelEnum.yaml",
)

gen_cppenum(
    enum_file = "LogLevelEnum.yaml",
    visibility = ["//runprofile:__pkg__"],
)

gen_cppenum(
    enum_file = "LogModuleEnum.yaml",
    visibility = ["//runprofile:__pkg__"],
)

gen_cppenum(
    enum_file = "LogTagEnum.yaml",
    visibility = ["//runprofile:__pkg__"],
)

cc_library(
    name = "log",
    srcs = ["log.cpp"],
    hdrs = ["log.h"],
    visibility = [
        "//:__pkg__",
        "//complexity:__pkg__",
        "//context:__pkg__",
        "//context/maple:__pkg__",
        "//eval/util:__pkg__",
        "//interp:__pkg__",
        "//math/fft:__pkg__",
        "//operation/field:__pkg__",
        "//run:__pkg__",
        "//runprofile:__pkg__",
    ],
    deps = [
        ":LogLevelEnum",
        ":LogModuleEnum",
        ":LogTagEnum",
        "@boost//:core",
        "@boost//:date_time",
        "@boost//:filesystem",
        "@boost//:log",
    ],
)

cc_library(
    name = "multi_index_map",
    srcs = ["MultiIndexMap.cpp"],
    hdrs = ["MultiIndexMap.h"],
    visibility = [
        "//:__pkg__",
        "//run:__pkg__",
        "//runprofile:__pkg__",
    ],
    deps = [
        "@boost//:variant",
    ],
)

cc_test(
    name = "multi_index_map_test",
    srcs = ["MultiIndexMap.t.cpp"],
    defines = ["BOOST_TEST_MAIN", "BOOST_TEST_DYN_LINK"],
    deps = [
        ":multi_index_map",
        "@boost//:test.so",
    ],
)

cc_library(
    name = "smart_assert",
    srcs = [
        "SmartAssert.cpp",
    ],
    hdrs = [
        "SmartAssert.h",
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
    ],
    deps = [
        ":LevelEnum",
        "@boost//:lexical_cast",
    ],
)

cc_library(
    name = "timer",
    srcs = [
        "Timer.cpp",
    ],
    hdrs = [
        "Timer.h",
    ],
    visibility = [
        "//:__pkg__",
        "//interp:__pkg__",
    ],
)
