cc_library(
    name = "thread",
    srcs = glob(
        [
            "src/**/*.cpp",
        ],
        exclude = [
            "src/win32/**",
            "src/pthread/once.cpp",
        ],
    ),
    hdrs = glob([
        "include/boost/**/*",
    ]),
    copts = ["-Iexternal/com_github_boost_thread/src/pthread"],
    includes = [
        "include/",
    ],
    linkopts = ["-pthread"],
    visibility = ["//visibility:public"],
    deps = [
        "//external:boost-atomic",
        "//external:boost-bind",
        "//external:boost-chrono",
        "//external:boost-config",
        "//external:boost-core",
        "//external:boost-date_time",
        "//external:boost-exception",
        "//external:boost-function",
        "//external:boost-functional",
        "//external:boost-io",
        "//external:boost-lexical_cast",
        "//external:boost-move",
        "//external:boost-optional",
        "//external:boost-system",
        "//external:boost-tuple",
        "//external:boost-type_traits",
    ],
)
