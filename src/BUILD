load("//third_party:cppenum.bzl", "gen_cppenum")

gen_cppenum(enum_file="ReturnValue.yaml")

cc_binary(
    name = "slp-poly",
    srcs = [
        "main.cpp",
    ],
    deps = [
        ":ReturnValue",
        "//context/maple:parser",
        "//platform",
        "//run:field_benchmarks",
        "//run:interpolation",
        "//runprofile:util",
        "//runprofile/factory:command_line",
        "//util:log",
        "//util:multi_index_map",
        "//util:timer",
    ],
    stamp = True,
    visibility = [
        "//try_out:__pkg__",
    ],
)
