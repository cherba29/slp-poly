cc_library(
    name = "exception",
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
        "//external:boost-config",
    ],
)
