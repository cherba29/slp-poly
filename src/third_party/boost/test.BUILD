cc_library(
    name = "test",
    srcs = [
        "src/compiler_log_formatter.cpp",
        "src/debug.cpp",
        "src/decorator.cpp",
        "src/execution_monitor.cpp",
        "src/framework.cpp",
        "src/junit_log_formatter.cpp",
        "src/plain_report_formatter.cpp",
        "src/progress_monitor.cpp",
        "src/results_collector.cpp",
        "src/results_reporter.cpp",
        "src/test_tools.cpp",
        "src/test_tree.cpp",
        "src/unit_test_log.cpp",
        "src/unit_test_main.cpp",
        "src/unit_test_monitor.cpp",
        "src/unit_test_parameters.cpp",
        "src/xml_log_formatter.cpp",
        "src/xml_report_formatter.cpp",
    ],
    hdrs = glob([
        "include/boost/**/*",
    ]),
    copts = [
        "-DBOOST_TEST_ALTERNATIVE_INIT_API",
    ],
    includes = [
        "include/",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//external:boost-algorithm",
        "//external:boost-config",
        "//external:boost-detail",
        "//external:boost-exception",
        "//external:boost-function",
        "//external:boost-io",
        "//external:boost-iterator",
        "//external:boost-mpl",
        "//external:boost-numeric_conversion",
        "//external:boost-optional",
        "//external:boost-preprocessor",
        "//external:boost-smart_ptr",
        "//external:boost-timer",
        "//external:boost-type_traits",
        "//external:boost-utility",
    ],
)
