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
        "@boost//:algorithm",
        "@boost//:optional",
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
    defines = ["BOOST_TEST_MAIN", "BOOST_TEST_DYN_LINK"],
    deps = [
        ":interp_profile",
        "@boost//:test.so",
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
        "@boost//:optional",
        "//util:log",
    ],
)

cc_test(
    name = "run_profile_test",
    srcs = [
        "RunProfile.t.cpp",
    ],
    defines = ["BOOST_TEST_MAIN", "BOOST_TEST_DYN_LINK"],
    deps = [
        ":runprofile",
        "@boost//:test.so",
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
