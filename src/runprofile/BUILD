load("//third_party:cppenum.bzl", "gen_cppenum")

gen_cppenum(enum_file="ActionEnum.yaml")

gen_cppenum(enum_file="FieldOptionEnum.yaml")

cc_library(
    name = "interpprofile",
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
    ]
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
        ":interpprofile",
        "//external:boost-optional",
        "//util:log",
    ],
    visibility = [
        "//:__pkg__",
        "//runprofile/factory:__pkg__",
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