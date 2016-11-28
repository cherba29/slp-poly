cc_library(
    name = "program_options",
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
        "//external:boost-any",
        "//external:boost-throw_exception",
        "//external:boost-bind",
        "//external:boost-detail",
        "//external:boost-config",
        "//external:boost-type_traits",
        "//external:boost-iterator",
        "//external:boost-function",
        #    "//external:boost-lexical_cast",
        "//external:boost-smart_ptr",
        "//external:boost-tokenizer",
    ],
)
