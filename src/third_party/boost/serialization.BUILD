cc_library(
    name = "serialization",
    srcs = glob([
        "src/**/*.cpp",
    ]),
    #  copts = ["-Iinclude/boost"],
    hdrs = glob([
        "include/boost/**/*",
        "src/**/*.ipp",
    ]),
    includes = [
        "include/",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//external:boost-array",
        "//external:boost-assert",
        "//external:boost-config",
        "//external:boost-core",
        "//external:boost-function",
        "//external:boost-integer",
        "//external:boost-io",
        "//external:boost-iterator",
        "//external:boost-mpl",
        "//external:boost-smart_ptr",
        "//external:boost-spirit",
        "//external:boost-static_assert",
        "//external:boost-utility",
    ],
)
