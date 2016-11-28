cc_library(
    name = "system",
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
        "//external:boost-assert",
        "//external:boost-config",
        "//external:boost-core",
        "//external:boost-predef",
    ],
)
