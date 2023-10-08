load("//third_party:cppenum.bzl", "gen_cppenum")

cc_library(
    name = "context",
    hdrs = [
        "context.h",
    ],
    visibility = [
        "//context/maple:__pkg__",
    ],
    deps = [
        ":FunctionEnum",
        ":assignment",
        ":determinant",
        ":function",
        ":id",
        ":integer",
        ":matrix",
        ":named_value",
        ":negation",
        ":power",
        ":processor",
        ":product",
        ":real_number",
        ":statement",
        ":string",
        ":sum",
        ":table",
        ":value",
        ":value_list",
    ],
)

cc_library(
    name = "assignment",
    hdrs = [
        "Assignment.h",
    ],
    deps = [
        ":statement",
        ":value",
        "@boost//:smart_ptr",
    ],
)

cc_library(
    name = "determinant",
    hdrs = [
        "Determinant.h",
    ],
    deps = [
        ":value",
        "@boost//:smart_ptr",
    ],
)

cc_library(
    name = "function",
    hdrs = [
        "Function.h",
    ],
    deps = [
        ":FunctionEnum",
        ":value_list",
        "@boost//:smart_ptr",
    ],
)

gen_cppenum(
    enum_file = "FunctionEnum.yaml",
)

cc_library(
    name = "id",
    hdrs = [
        "Id.h",
    ],
    deps = [
        ":value",
    ],
)

cc_library(
    name = "integer",
    hdrs = [
        "Integer.h",
    ],
    deps = [
        ":value",
    ],
)

cc_library(
    name = "interp_context",
    hdrs = [
        "InterpContext.h",
    ],
    visibility = [
        "//context/maple:__pkg__",
        "//eval:__pkg__",
    ],
    deps = [
        ":context",
        "//util:log",
    ],
)

cc_library(
    name = "matrix",
    hdrs = [
        "Matrix.h",
    ],
    deps = [
        ":integer",
        ":value",
    ],
)

cc_library(
    name = "named_value",
    hdrs = [
        "NamedValue.h",
    ],
    deps = [
        ":value",
    ],
)

cc_library(
    name = "negation",
    hdrs = [
        "Negation.h",
    ],
    deps = [
        ":value",
        "@boost//:smart_ptr",
        "//util:smart_assert",
    ],
)

cc_library(
    name = "power",
    hdrs = [
        "Power.h",
    ],
    deps = [
        ":value",
        "@boost//:smart_ptr",
        "//util:smart_assert",
    ],
)

cc_library(
    name = "printer",
    srcs = [
        "Printer.cpp",
    ],
    hdrs = [
        "Printer.h",
    ],
    visibility = [
        "//eval:__pkg__",
    ],
    deps = [
        ":context",
    ],
)

cc_library(
    name = "processor",
    hdrs = [
        "Processor.h",
    ],
)

cc_library(
    name = "product",
    hdrs = [
        "Product.h",
    ],
    deps = [
        ":value",
        "@boost//:smart_ptr",
        "//util:smart_assert",
    ],
)

cc_library(
    name = "real_number",
    hdrs = [
        "RealNumber.h",
    ],
    deps = [
        ":value",
    ],
)

cc_library(
    name = "statement",
    hdrs = [
        "Statement.h",
    ],
)

cc_library(
    name = "string",
    hdrs = [
        "String.h",
    ],
    deps = [
        ":processor",
        ":value",
    ],
)

cc_library(
    name = "sum",
    hdrs = [
        "Sum.h",
    ],
    deps = [
        ":value",
        "@boost//:smart_ptr",
        "//util:smart_assert",
    ],
)

cc_library(
    name = "table",
    hdrs = [
        "Table.h",
    ],
)

cc_library(
    name = "value",
    srcs = [
        "Value.cpp",
    ],
    hdrs = [
        "Value.h",
    ],
)

cc_library(
    name = "value_list",
    srcs = [
        "ValueList.cpp",
    ],
    hdrs = [
        "ValueList.h",
    ],
    deps = [
        ":value",
    ],
)
