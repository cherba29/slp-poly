
cc_library(
    name = "base",
    hdrs = [
        "Base.h",
    ],
    deps = [
        "//external:boost-static_assert",
    ],
    visibility = ["//runprofile:__pkg__",]
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