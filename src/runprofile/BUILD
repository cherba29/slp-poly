load("//third_party:cppenum.bzl", "gen_cppenum")

gen_cppenum(enum_file="ActionEnum.yaml")

gen_cppenum(enum_file="FieldOptionEnum.yaml")

cc_library(
    name = "interp_profile",
    hdrs = [
        "InterpProfile.h",
    ],
    srcs = [
        "InterpProfile.cpp",
    ],
    deps = [
        ":FieldOptionEnum",
        "//interp:PrunningEnum",
        "//interp:UnivInterpEnum",
        "//interp:VandSolveTypeEnum",
        "//interp:VarOrderEnum",
        "//math:base",
        "//util:smart_assert",
        "//external:boost-algorithm",
        "//external:boost-optional",
    ],
    visibility = [
        "//run:__pkg__",
    ],
)

cc_test(
    name = "interp_profile_test",
    srcs = [
        "InterpProfile.t.cpp",
    ],
    deps = [
        ":interp_profile",
        "//external:boost-test",
    ],
)

cc_library(
    name = "runprofile",
    hdrs = [
        "RunProfile.h",
    ],
    srcs = [
        "RunProfile.cpp",
    ],
    deps = [
        ":ActionEnum",
        ":interp_profile",
        "//external:boost-optional",
        "//util:log",
    ],
    visibility = [
        "//:__pkg__",
        "//runprofile/factory:__pkg__",
    ],
)

cc_test(
    name = "run_profile_test",
    srcs = [
        "RunProfile.t.cpp",
    ],
    deps = [
        ":runprofile",
        "//runprofile/factory:command_line",
        "//external:boost-test",
    ],
)

cc_library(
    name = "util",
    hdrs = [
        "util.h",
    ],
    deps = [
        ":runprofile",
        "//util:log",
        "//util:multi_index_map",
    ],
    visibility = [
        "//:__pkg__",
    ],
)