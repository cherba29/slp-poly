cc_library(
    name = "log",
    srcs = glob(
        [
            "src/**/*.cpp",
            "src/**/*.h",
        ],
        exclude = ["src/windows/**/*"],
    ),
    hdrs = glob([
        "include/boost/**/*",
        "src/**/*.hpp",
    ]),
    copts = [
        "-mavx2",
        "-Iinclude/boost",
    ],
    includes = [
        "include/",
        "src/",
    ],
    linkopts = ["-pthread"],
    visibility = ["//visibility:public"],
    deps = [
        "//external:boost-align",
        "//external:boost-array",
        "//external:boost-asio",
        "//external:boost-atomic",
        "//external:boost-bind",
        "//external:boost-config",
        "//external:boost-detail",
        "//external:boost-exception",
        "//external:boost-filesystem",
        "//external:boost-interprocess",
        "//external:boost-intrusive",
        "//external:boost-lexical_cast",
        "//external:boost-locale",
        "//external:boost-move",
        "//external:boost-optional",
        "//external:boost-parameter",
        "//external:boost-predef",
        "//external:boost-property_tree",
        "//external:boost-random",
        "//external:boost-regex",
        "//external:boost-spirit",
        "//external:boost-system",
        "//external:boost-thread",
        "//external:boost-type_index",
    ],
)
