cc_library(
    name = "date_time",
    srcs = glob([
        "src/*.cpp",
    ]),
    hdrs = glob([
        "include/boost/**/*",
    ]),
    includes = [
        "include/",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//external:boost-algorithm",
        "//external:boost-io",
        "//external:boost-lexical_cast",
        "//external:boost-smart_ptr",
        "//external:boost-throw_exception",
        "//external:boost-tokenizer",
        "//external:boost-utility",
    ],
)
