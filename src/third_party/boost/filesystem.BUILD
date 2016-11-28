cc_library(
    name = "filesystem",
    srcs = glob([
        "src/*.cpp",
    ]),
    hdrs = glob([
        "include/boost/**/*.hpp",
    ]),
    includes = [
        "include/",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//external:boost-config",
        "//external:boost-detail",
        "//external:boost-functional",
        "//external:boost-io",
        "//external:boost-iterator",
        "//external:boost-range",
        "//external:boost-smart_ptr",
        "//external:boost-system",
        "//external:boost-type_traits",
    ],
)
