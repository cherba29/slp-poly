load("//third_party:cppenum.bzl", "gen_cppenum")

gen_cppenum(
    enum_file="PrunningEnum.yaml",
    visibility = ["//runprofile:__pkg__",],
)

gen_cppenum(
    enum_file="UnivInterpEnum.yaml",
    visibility = ["//runprofile:__pkg__",],
)

gen_cppenum(
    enum_file="VandSolveTypeEnum.yaml",
    visibility = [
        "//run:__pkg__",
        "//runprofile:__pkg__",
    ],
)

gen_cppenum(
    enum_file="VarOrderEnum.yaml",
    visibility = ["//runprofile:__pkg__",],
)

cc_library(
    name = "interpolator",
    hdrs = [
        "Interpolator.h",
        "Interpolator-impl.h",
    ],
    srcs = [
        "Interpolator.cpp",
    ],
    deps = [
        ":PrunningEnum",
        ":VarOrderEnum",
        ":newton",
        ":vand_solver_factory",
        "//algebra:polynomial",
        "//algebra:partial_polynomial",
        "//complexity:perf",
        "//eval:code_generator",
        "//eval:univ_evaluator",
        "//eval/util:instruction_max_degree",
        "//external:boost-smart_ptr",
        "//math/util:prime",
        "//util:timer",
    ],
    visibility = [
        "//run:__pkg__",
    ],
)

cc_library(
    name = "newton",
    hdrs = [
        "Newton.h",
    ],
    deps = [
        "//algebra:polynomial",
        "//util:log",
    ],
    visibility = [
        "//run:__pkg__",
    ],
)

cc_test(
    name = "newton_test",
    srcs = ["Newton.t.cpp"],
    deps = [":newton",],
)

cc_library(
    name = "vand_solver_factory",
    hdrs = [
        "VandSolverFactory.h",
        "VandSolverFactory-impl.h",
    ],
    srcs = [
        "VandSolverFactory.cpp",
    ],
    deps = [
        ":VandSolveTypeEnum",
        "//complexity:AlgEnum",
        "//complexity:perf_alg",
        "//math/vandermonde:fft_solver",
        "//math/vandermonde:quad_solver",
        "//math/vandermonde:solver",
    ],
    visibility = [
    ],
)