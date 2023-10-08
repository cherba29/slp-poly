cc_library(
    name = "command_line",
    srcs = [
        "CommandLine.cpp",
    ],
    hdrs = [
        "CommandLine.h",
    ],
    visibility = [
        "//:__pkg__",
        "//runprofile:__pkg__",
    ],
    deps = [
        "@boost//:algorithm",
        "@boost//:bind",
        "@boost//:program_options",
        "@boost//:static_assert",
        "@boost//:type_traits",
        "//runprofile",
    ],
)
