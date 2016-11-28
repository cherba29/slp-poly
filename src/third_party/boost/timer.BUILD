cc_library(
    name = "timer",
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
        "//external:boost-chrono",
        "//external:boost-config",
        "//external:boost-io",
        "//external:boost-system",
        "//external:boost-throw_exception",
    ],
)
