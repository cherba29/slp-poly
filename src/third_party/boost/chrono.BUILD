cc_library(
    name = "chrono",
    srcs = glob([
        "src/**/*.cpp",
    ]),
    hdrs = glob([
        "include/boost/**/*",
    ]),
    includes = [
        "include/",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//external:boost-config",
        "//external:boost-mpl",
        "//external:boost-predef",
        "//external:boost-ratio",
        "//external:boost-system",
        "//external:boost-throw_exception",
        "//external:boost-utility",
    ],
)
