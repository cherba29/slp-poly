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
        "A32.cpp",
        "B32.cpp",
        "C32.cpp",
        "AB32.cpp",
        "AC32.cpp",
        "BC32.cpp",
        "A64.cpp",
        "B64.cpp",
        "C64.cpp",
        "AB64.cpp",
        "AC64.cpp",
        "BC64.cpp",
        "Dynamic32.cpp",
        "Dynamic64.cpp",
        "Interpolation.cpp",
    ],
    deps = [
        ":field_benchmarks",
        "//math/field:field",
        "//math/field:field_uint",
        "//math/field:field_uint2x",
        "//math/util:prime",
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