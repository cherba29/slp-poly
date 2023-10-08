load("//third_party:cppenum.bzl", "gen_cppenum")

gen_cppenum(
    enum_file = "PrunningEnum.yaml",
    visibility = ["//runprofile:__pkg__"],
)

gen_cppenum(
    enum_file = "UnivInterpEnum.yaml",
    visibility = ["//runprofile:__pkg__"],
)

gen_cppenum(
    enum_file = "VandSolveTypeEnum.yaml",
    visibility = [
        "//run:__pkg__",
        "//runprofile:__pkg__",
    ],
)

gen_cppenum(
    enum_file = "VarOrderEnum.yaml",
    visibility = ["//runprofile:__pkg__"],
)

cc_library(
    name = "interpolator",
    srcs = [
        "Interpolator.cpp",
    ],
    hdrs = [
        "Interpolator.h",
        "Interpolator-impl.h",
    ],
    visibility = [
        "//run:__pkg__",
    ],
    deps = [
        ":PrunningEnum",
        ":VarOrderEnum",
        ":newton",
        ":vand_solver_factory",
        "//algebra:partial_polynomial",
        "//algebra:polynomial",
        "//complexity:perf",
        "//eval:code_generator",
        "//eval:univ_evaluator",
        "//eval/util:instruction_max_degree",
        "@boost//:smart_ptr",
        "//math/util:prime",
        "//util:timer",
    ],
)

cc_library(
    name = "newton",
    hdrs = [
        "Newton.h",
    ],
    visibility = [
        "//run:__pkg__",
    ],
    deps = [
        "//algebra:polynomial",
        "//util:log",
    ],
)

cc_test(
    name = "newton_test",
    srcs = ["Newton.t.cpp"],
    defines = ["BOOST_TEST_MAIN", "BOOST_TEST_DYN_LINK"],
    deps = [
        ":newton",
        "@boost//:test.so",
        "//operation/field:setup",
    ],
)

cc_library(
    name = "vand_solver_factory",
    srcs = [
        "VandSolverFactory.cpp",
    ],
    hdrs = [
        "VandSolverFactory.h",
        "VandSolverFactory-impl.h",
    ],
    visibility = [
    ],
    deps = [
        ":VandSolveTypeEnum",
        "//complexity:AlgEnum",
        "//complexity:perf_alg",
        "//math/vandermonde:fft_solver",
        "//math/vandermonde:quad_solver",
        "//math/vandermonde:solver",
    ],
)
