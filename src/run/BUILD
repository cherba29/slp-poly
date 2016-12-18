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
    name = "interpolation_def",
    hdrs = [
        "Interpolation.h",
    ],
    deps = [
        "//complexity:AlgEnum",
        "//complexity:perf",
        "//eval:code_generator",
        "//eval/util:instruction_counter",
        "//interp:VandSolveTypeEnum",
        "//interp:interpolator",
        "//operation/eval:benchmark",
        "//operation/vandermonde:benchmark",
        "//math/util:prime",
        "//math/util:random",
        "//runprofile:interp_profile",
        "//util:multi_index_map",
        "//external:boost-smart_ptr",
    ],
)

cc_library(
    name = "interpolation",
    srcs = [
        "Interpolation.cpp",
    ],
    deps = [
        ":interpolation_def",
        ":A32",
        ":B32",
        ":C32",
        ":AB32",
        ":AC32",
        ":BC32",
        ":CRT32",
        ":Dynamic32",
        ":A64",
        ":B64",
        ":C64",
        ":AB64",
        ":AC64",
        ":BC64",
        ":CRT64",
        ":Dynamic64",
    ],
    visibility = [
        "//:__pkg__",
    ],
)

cc_library(
    name = "A32",
    srcs = ["A32.cpp"],
    deps = [
        ":field_benchmarks",
        ":interpolation_def",
        "//math/field:field_uint",
        "//math/util:prime",
        "//util:log",
        "//util:smart_assert",
    ],
)    

cc_library(
    name = "B32",
    srcs = ["B32.cpp"],
    deps = [
        ":field_benchmarks",
        ":interpolation_def",
        "//math/field:field_uint",
        "//math/util:prime",
        "//util:log",
        "//util:smart_assert",
    ],
)    

cc_library(
    name = "C32",
    srcs = ["C32.cpp"],
    deps = [
        ":field_benchmarks",
        ":interpolation_def",
        "//math/field:field_uint",
        "//math/util:prime",
        "//util:log",
        "//util:smart_assert",
    ],
)    

cc_library(
    name = "AB32",
    srcs = ["AB32.cpp"],
    deps = [
        ":field_benchmarks",
        ":interpolation_def",
        "//math/field:field_uint2x",
        "//math/util:prime",
        "//util:log",
        "//util:smart_assert",
    ],
)

cc_library(
    name = "AC32",
    srcs = ["AC32.cpp"],
    deps = [
        ":field_benchmarks",
        ":interpolation_def",
        "//math/field:field_uint2x",
        "//math/util:prime",
        "//util:log",
        "//util:smart_assert",
    ],
)

cc_library(
    name = "BC32",
    srcs = ["BC32.cpp"],
    deps = [
        ":field_benchmarks",
        ":interpolation_def",
        "//math/field:field_uint2x",
        "//math/util:prime",
        "//util:log",
        "//util:smart_assert",
    ],
)

cc_library(
    name = "CRT32",
    srcs = ["CRT32.cpp"],
    deps = [
        ":field_benchmarks",
        ":interpolation_def",
        "//math/field:crt",
        "//math/util:prime",
        "//util:log",
        "//util:smart_assert",
    ],
)

cc_library(
    name = "Dynamic32",
    srcs = ["Dynamic32.cpp"],
    deps = [
        ":field_benchmarks",
        ":interpolation_def",
        "//math/field:field",
        "//math/util:prime",
        "//util:log",
        "//util:smart_assert",
    ],
)

cc_library(
    name = "A64",
    srcs = ["A64.cpp"],
    deps = [
        ":field_benchmarks",
        ":interpolation_def",
        "//math/field:field_uint",
        "//math/util:prime",
        "//util:log",
        "//util:smart_assert",
    ],
)    

cc_library(
    name = "B64",
    srcs = ["B64.cpp"],
    deps = [
        ":field_benchmarks",
        ":interpolation_def",
        "//math/field:field_uint",
        "//math/util:prime",
        "//util:log",
        "//util:smart_assert",
    ],
)    

cc_library(
    name = "C64",
    srcs = ["C64.cpp"],
    deps = [
        ":field_benchmarks",
        ":interpolation_def",
        "//math/field:field_uint",
        "//math/util:prime",
        "//util:log",
        "//util:smart_assert",
    ],
)    

cc_library(
    name = "AB64",
    srcs = ["AB64.cpp"],
    deps = [
        ":field_benchmarks",
        ":interpolation_def",
        "//math/field:field_uint2x",
        "//math/util:prime",
        "//util:log",
        "//util:smart_assert",
    ],
)

cc_library(
    name = "AC64",
    srcs = ["AC64.cpp"],
    deps = [
        ":field_benchmarks",
        ":interpolation_def",
        "//math/field:field_uint2x",
        "//math/util:prime",
        "//util:log",
        "//util:smart_assert",
    ],
)

cc_library(
    name = "BC64",
    srcs = ["BC64.cpp"],
    deps = [
        ":field_benchmarks",
        ":interpolation_def",
        "//math/field:field_uint2x",
        "//math/util:prime",
        "//util:log",
        "//util:smart_assert",
    ],
)

cc_library(
    name = "CRT64",
    srcs = ["CRT64.cpp"],
    deps = [
        ":field_benchmarks",
        ":interpolation_def",
        "//math/field:crt",
        "//math/util:prime",
        "//util:log",
        "//util:smart_assert",
    ],
)

cc_library(
    name = "Dynamic64",
    srcs = ["Dynamic64.cpp"],
    deps = [
        ":field_benchmarks",
        ":interpolation_def",
        "//math/field:field",
        "//math/util:prime",
        "//util:log",
        "//util:smart_assert",
    ],
)