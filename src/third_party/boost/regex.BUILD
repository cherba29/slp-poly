cc_library(
    name = "regex",
    srcs = glob([
        "src/**/*.cpp",
        "src/**/*.hpp",
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
        "//external:boost-functional",
        "//external:boost-integer",
        "//external:boost-mpl",
        "//external:boost-predef",
        "//external:boost-smart_ptr",
        "//external:boost-throw_exception",
        "//external:boost-type_traits",
    ],
)
