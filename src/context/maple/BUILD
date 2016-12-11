load("//third_party:flex_bison.bzl", "cc_flex_bison_library")


cc_flex_bison_library(
    name = "parser",
    hdrs = [
        "Driver.h",
        "Scanner.h",
    ],
    srcs = [
        "Driver.cpp",
    ],
    flex_file = "Scanner.ll",
    bison_file = "Parser.yy",
    deps = [
        "//context",
        "//context:interp_context",
        "//external:boost-smart_ptr",
        "//util:log",
    ],
    visibility = [
        "//:__pkg__",
    ],
)

