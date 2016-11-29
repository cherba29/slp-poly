load("//third_party:cppenum.bzl", "gen_cppenum")

gen_cppenum(enum_file="ReturnValue.yaml")

cc_library(
    name="return_value",
    hdrs=[
        "ReturnValue.h",
    ],
    srcs=[
        "ReturnValue.cpp",
    ],
)

cc_binary(
    name = "slp-poly",
    srcs = [
        "main.cpp",
    ],
    deps = [
        ":ReturnValue",
        "//platform",
        "//runprofile/factory:command_line",
        "//util:log",
    ],
    stamp = True,
)
