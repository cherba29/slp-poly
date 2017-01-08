load("//third_party:cppenum.bzl", "gen_cppenum")

gen_cppenum(enum_file = "ActionEnum.yaml")

gen_cppenum(enum_file = "FieldOptionEnum.yaml")

cc_library(
    name = "interp_profile",
    srcs = [
        "InterpProfile.cpp",
    ],
    hdrs = [
        "InterpProfile.h",
    ],
    visibility = [
        "//run:__pkg__",
    ],
    deps = [
        ":FieldOptionEnum",
        "//external:boost-algorithm",
        "//external:boost-optional",
        "//interp:PrunningEnum",
        "//interp:UnivInterpEnum",
        "//interp:VandSolveTypeEnum",
        "//interp:VarOrderEnum",
        "//math:base",
        "//util:smart_assert",
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
    srcs = [
        "RunProfile.cpp",
    ],
    hdrs = [
        "RunProfile.h",
    ],
    visibility = [
        "//:__pkg__",
        "//runprofile/factory:__pkg__",
    ],
    deps = [
        ":ActionEnum",
        ":interp_profile",
        "//external:boost-optional",
        "//util:log",
    ],
)

cc_test(
    name = "run_profile_test",
    srcs = [
        "RunProfile.t.cpp",
    ],
    deps = [
        ":runprofile",
        "//external:boost-test",
        "//runprofile/factory:command_line",
    ],
)

cc_library(
    name = "util",
    hdrs = [
        "util.h",
    ],
    visibility = [
        "//:__pkg__",
    ],
    deps = [
        ":runprofile",
        "//util:log",
        "//util:multi_index_map",
    ],
)
