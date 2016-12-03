
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
    ],
)