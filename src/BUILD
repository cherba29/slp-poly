load("//third_party/clang_format:clang_format.bzl", "clang_format")
load("//third_party:cppenum.bzl", "gen_cppenum")

gen_cppenum(enum_file = "ReturnValue.yaml")

cc_binary(
    name = "slp-poly",
    srcs = [
        "main.cpp",
    ],
    # -1 -- Embedding of build information is controlled by the --[no]stamp flag.
    stamp = -1, 
    visibility = [
        "//try_out:__pkg__",
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
)

# Note! Needs needs to be the last declaration in the file
# in order to correctly pick up earlier declared cc_* rules.
clang_format()