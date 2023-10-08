cc_library(
    name = "benchmark",
    hdrs = [
        "FieldOperations.h",
        "Operation.h",
        "benchmark.h",
    ],
    visibility = [
        "//run:__pkg__",
    ],
    deps = [
        "//complexity:equation",
        "//complexity:tracker",
        "@boost//:timer",
        "//math:base",
        "//util:log",
    ],
)

cc_library(
    name = "setup",
    hdrs = [
        "setup.h",
    ],
    visibility = [
        "//interp:__pkg__",
        "//math:__pkg__",
    ],
    deps = [
        "@boost//:mpl",
        "//math/field",
        "//math/field:crt",
        "//math/field:crt2",
        "//math/field:field_uint",
        "//math/field:field_uint2x",
        "//math/field/util",
    ],
)
