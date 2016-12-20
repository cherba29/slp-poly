
cc_library(
    name = "base",
    hdrs = [
        "Base.h",
    ],
    deps = [
        "//external:boost-static_assert",
    ],
    visibility = [
        "//complexity:__pkg__",
        "//math/fft:__pkg__",
        "//math/field:__pkg__",
        "//math/field/util:__pkg__",
        "//math/util:__pkg__",
        "//run:__pkg__",
        "//runprofile:__pkg__",
        "//operation/field:__pkg__",
        "//operation/vandermonde:__pkg__",
    ]
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
        "//operation/field:setup",
        "//external:boost-test",
        "//external:boost-timer",
    ],
)

cc_test(
    name = "modular-x86_test",
    srcs = [
        "modular-x86.t.cpp",
    ],
    deps = [
        "//math/util:modular-x86",
        "//math/util:prime",
        "//external:boost-test",
    ],
)

cc_test(
    name = "prime_test",
    srcs = [
        "Prime.t.cpp",
    ],
    deps = [
        "//math/util:prime",
        "//external:boost-test",
    ],
)

cc_test(
    name = "vandermonde_test",
    srcs = [
        "Vandermonde.t.cpp",
    ],
    deps = [
        "//math/vandermonde:fft_solver",
        "//math/vandermonde:quad_solver",
        "//operation/field:setup",
        "//external:boost-test",
    ],
)