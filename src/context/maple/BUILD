load("//third_party:flex_bison.bzl", "cc_flex_bison_library")

cc_flex_bison_library(
    name = "parser",
    srcs = [
        "Driver.cpp",
    ],
    hdrs = [
        "Driver.h",
        "Scanner.h",
    ],
    bison_file = "Parser.yy",
    flex_file = "Scanner.ll",
    visibility = [
        "//:__pkg__",
    ],
    deps = [
        "//context",
        "//context:interp_context",
        "@boost//:smart_ptr",
        "//util:log",
    ],
)
