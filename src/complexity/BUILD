load("//third_party:cppenum.bzl", "gen_cppenum")

gen_cppenum(
    enum_file="AlgEnum.yaml",
    visibility = [
        "//run:__pkg__",
    ],
)

cc_library(
    name = "equation",
    hdrs = [
        "Equation.h",
    ],
    srcs = [
        "Equation.cpp",
    ],
    deps = [
        "//util:log",
    ],
    visibility = [
        "//run:__pkg__",
        "//operation/field:__pkg__",
        "//operation/vandermonde:__pkg__",
    ],
)

cc_library(
    name = "estimator",
    hdrs = [
        "Estimator.h",
    ],
    srcs = [
        "Estimator.cpp",
    ],
    deps = [
        ":tracker",
        "//math:base",
        "//util:log",
        "//external:boost-function",
        "//external:boost-smart_ptr",
    ],
    visibility = [
        "//run:__pkg__",
    ],
)

cc_library(
    name = "perf",
    hdrs = [
        "Perf.h",
    ],
    srcs = [
        "Perf.cpp",
    ],
    deps = [
        ":estimator",
        ":tracker",
        "//util:log",
        "//external:boost-timer",
    ],
    visibility = [
        "//run:__pkg__",
    ],
)

cc_test(
    name = "perf_test",
    srcs = [
        "Perf.t.cpp",
    ],
    deps = [
        ":perf",
    ],
)

cc_library(
    name = "tracker",
    hdrs = [
        "Tracker.h",
    ],
    srcs = [
        "Tracker.cpp",
    ],
    deps = [
        ":equation",
        "//math/numeric:svd",
        "//external:boost-smart_ptr",
        "//util:log",
    ],
    visibility = [
        "//run:__pkg__",
        "//operation/field:__pkg__",
        "//operation/vandermonde:__pkg__",
    ],
)
