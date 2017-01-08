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
        "//external:boost-static_assert",
    ],
)

cc_test(
    name = "base_test",
    srcs = [
        "Base.t.cpp",
    ],
    deps = [
        ":base",
        "//external:boost-test",
        "//operation/field:setup",
    ],
)

cc_test(
    name = "field_test",
    srcs = [
        "Field.t.cpp",
    ],
    deps = [
        "//external:boost-test",
        "//external:boost-timer",
        "//operation/field:setup",
    ],
)

cc_test(
    name = "modular-x86_test",
    srcs = [
        "modular-x86.t.cpp",
    ],
    deps = [
        "//external:boost-test",
        "//math/util:modular-x86",
        "//math/util:prime",
    ],
)

cc_test(
    name = "prime_test",
    srcs = [
        "Prime.t.cpp",
    ],
    deps = [
        "//external:boost-test",
        "//math/util:prime",
    ],
)

cc_test(
    name = "vandermonde_test",
    srcs = [
        "Vandermonde.t.cpp",
    ],
    deps = [
        "//external:boost-test",
        "//math/vandermonde:fft_solver",
        "//math/vandermonde:quad_solver",
        "//operation/field:setup",
    ],
)
