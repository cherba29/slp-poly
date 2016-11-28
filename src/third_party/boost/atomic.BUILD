cc_library(
    name = "atomic",
    srcs = glob([
        "src/**/*.cpp",
    ]),
    hdrs = glob([
        "include/boost/**/*.hpp",
    ]),
    includes = [
        "include/",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//external:boost-assert",
        "//external:boost-config",
    ],
)
