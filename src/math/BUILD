cc_library(
    name = "base",
    hdrs = [
        "Base.h",
    ],
    visibility = [
        "//complexity:__pkg__",
        "//math/fft:__pkg__",
        "//math/field:__pkg__",
        "//math/field/util:__pkg__",
        "//math/util:__pkg__",
        "//operation/field:__pkg__",
        "//operation/vandermonde:__pkg__",
        "//run:__pkg__",
        "//runprofile:__pkg__",
    ],
    deps = [
        "@boost//:static_assert",
    ],
)

cc_test(
    name = "base_test",
    srcs = [
        "Base.t.cpp",
    ],
    defines = ["BOOST_TEST_MAIN", "BOOST_TEST_DYN_LINK"],
    deps = [
        ":base",
        "@boost//:test.so",
        "//operation/field:setup",
    ],
)

cc_test(
    name = "field_test",
    srcs = [
        "Field.t.cpp",
    ],
    defines = ["BOOST_TIMER_ENABLE_DEPRECATED", "BOOST_TEST_MAIN", "BOOST_TEST_DYN_LINK"],
    deps = [
        "@boost//:test.so",
        "@boost//:timer",
        "//operation/field:setup",
    ],
)

cc_test(
    name = "modular-x86_test",
    srcs = [
        "modular-x86.t.cpp",
    ],
    defines = ["BOOST_TEST_MAIN", "BOOST_TEST_DYN_LINK"],
    deps = [
        "@boost//:test.so",
        "//math/util:modular-x86",
        "//math/util:prime",
    ],
)

cc_test(
    name = "prime_test",
    srcs = [
        "Prime.t.cpp",
    ],
    defines = ["BOOST_TEST_MAIN", "BOOST_TEST_DYN_LINK"],
    deps = [
        "@boost//:test.so",
        "//math/util:prime",
    ],
)

cc_test(
    name = "vandermonde_test",
    srcs = [
        "Vandermonde.t.cpp",
    ],
    defines = ["BOOST_TEST_MAIN", "BOOST_TEST_DYN_LINK"],
    deps = [
        "@boost//:test.so",
        "//math/vandermonde:fft_solver",
        "//math/vandermonde:quad_solver",
        "//operation/field:setup",
    ],
)
