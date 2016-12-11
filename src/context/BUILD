load("//third_party:cppenum.bzl", "gen_cppenum")

cc_library(
    name = "context",
    hdrs = [
        "context.h",
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
    visibility = [
        "//context/maple:__pkg__",
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
        "//external:boost-smart_ptr",
    ],
)

cc_library(
    name = "determinant",
    hdrs = [
        "Determinant.h",
    ],
    deps = [
        ":value",
        "//external:boost-smart_ptr",
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
        "//external:boost-smart_ptr",
    ],
)

gen_cppenum(
    enum_file="FunctionEnum.yaml",
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
    deps = [
        ":context",
        "//util:log",
    ],
    visibility = [
        "//eval:__pkg__",
        "//context/maple:__pkg__",
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
        "//external:boost-smart_ptr",
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
        "//external:boost-smart_ptr",
        "//util:smart_assert",
    ],
)

cc_library(
    name = "printer",
    hdrs = [
        "Printer.h",
    ],
    srcs = [
        "Printer.cpp",
    ],
    deps = [
        ":context",
    ],
    visibility = [
        "//eval:__pkg__",
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
        "//external:boost-smart_ptr",
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
        ":value",
        ":processor",
    ],
)

cc_library(
    name = "sum",
    hdrs = [
        "Sum.h",
    ],
    deps = [
        ":value",
        "//external:boost-smart_ptr",
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
    hdrs = [
        "Value.h",
    ],
    srcs = [
        "Value.cpp",
    ],
)

cc_library(
    name = "value_list",
    hdrs = [
        "ValueList.h",
    ],
    srcs = [
        "ValueList.cpp",
    ],
    deps = [
        ":value",
    ],
)