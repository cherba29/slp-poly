licenses(["notice"])

cc_library(
    name = "optional",
    srcs = [
    ],
    hdrs = glob([
        "include/boost/**/*",
    ]),
    includes = [
        "include/",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//external:boost-assert",
        "//external:boost-core",
        "//external:boost-move",
        "//external:boost-static_assert",
        "//external:boost-throw_exception",
        "//external:boost-type_traits",
        "//external:boost-utility",
    ],
)

cc_library(
    name = "optional_test_lib",
    hdrs = [
        "test/optional_test_common.cpp",
    ],
)

cc_test(
    name = "optional_test",
    srcs = [
        "test/optional_test.cpp",
    ],
    deps = [
        ":optional",
        ":optional_test_lib",
        "//external:boost-bind",
        "//external:boost-mpl",
    ],
)
