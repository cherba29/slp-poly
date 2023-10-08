load("//third_party:cppenum.bzl", "gen_cppenum")

gen_cppenum(
    enum_file = "AlgEnum.yaml",
    visibility = [
        "//interp:__pkg__",
        "//run:__pkg__",
    ],
)

cc_library(
    name = "equation",
    srcs = [
        "Equation.cpp",
    ],
    hdrs = [
        "Equation.h",
    ],
    visibility = [
        "//operation/eval:__pkg__",
        "//operation/field:__pkg__",
        "//operation/vandermonde:__pkg__",
        "//run:__pkg__",
    ],
    deps = [
        "//util:log",
    ],
)

cc_library(
    name = "estimator",
    srcs = [
        "Estimator.cpp",
    ],
    hdrs = [
        "Estimator.h",
    ],
    visibility = [
        "//run:__pkg__",
    ],
    deps = [
        ":tracker",
        "@boost//:function",
        "@boost//:smart_ptr",
        "//math:base",
        "//util:log",
    ],
)

cc_library(
    name = "perf",
    srcs = [
        "Perf.cpp",
    ],
    hdrs = [
        "Perf.h",
    ],
    visibility = [
        "//interp:__pkg__",
        "//run:__pkg__",
    ],
    defines = ["BOOST_TIMER_ENABLE_DEPRECATED"],
    deps = [
        ":estimator",
        ":tracker",
        "@boost//:timer",
        "//util:log",
    ],
)

cc_test(
    name = "perf_test",
    srcs = [
        "Perf.t.cpp",
    ],
    defines = ["BOOST_TIMER_ENABLE_DEPRECATED", "BOOST_TEST_MAIN", "BOOST_TEST_DYN_LINK"],
    deps = [
        ":perf",
        "//complexity:AlgEnum",
        "@boost//:test.so",
    ],
)

cc_library(
    name = "perf_alg",
    srcs = [
    ],
    hdrs = [
        "PerfAlg.h",
    ],
    visibility = [
        "//interp:__pkg__",
    ],
    deps = [
        ":perf",
        "//util:log",
    ],
)

cc_library(
    name = "tracker",
    srcs = [
        "Tracker.cpp",
    ],
    hdrs = [
        "Tracker.h",
    ],
    visibility = [
        "//operation/eval:__pkg__",
        "//operation/field:__pkg__",
        "//operation/vandermonde:__pkg__",
        "//run:__pkg__",
    ],
    deps = [
        ":equation",
        "@boost//:smart_ptr",
        "//math/numeric:svd",
        "//util:log",
    ],
)
