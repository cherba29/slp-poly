cc_library(
    name="field_benchmarks",
    hdrs=[
        "FieldBenchmarks.h",
    ],
    srcs=[
        "FieldBenchmarks.cpp",
    ],
    deps = [
        "//complexity:AlgEnum",
        "//complexity:equation",
        "//complexity:estimator",
        "//complexity:perf",
        "//math:base",
        "//operation/field:benchmark",
        "//operation/vandermonde:benchmark",
        "//util:multi_index_map",
    ],
    visibility = [
        "//:__pkg__",
    ]
)

cc_library(
    name = "interpolation",
    hdrs = [
        "Interpolation.h",
    ],
    srcs = [
        "Interpolation.cpp",
    ],
    deps = [
        "//eval/util:instruction_counter",
        "//interp:interpolator",
        "//runprofile",
        "//operation/eval:benchmark",
        "//operation/vandermonde:benchmark",
        "//util:multi_index_map",
    ],
    visibility = [
        "//:__pkg__",
    ],
)