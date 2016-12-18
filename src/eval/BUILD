load("//third_party:cppenum.bzl", "gen_cppenum")

gen_cppenum(
    enum_file="InstructionEnum.yaml",
    visibility = [
        "//eval/util:__pkg__",
    ],
)

cc_library(
    name = "code_generator",
    hdrs = [
        "CodeGenerator.h",
        "CodeGenerator-Impl.h"
    ],
    deps = [
        "//context:interp_context",
        "//context:printer",
        "//eval/util:data_allocator",
        "//eval/util:instruction_printer",
    ],
    visibility = [
        "//interp:__pkg__",
        "//run:__pkg__",
    ],
)

cc_library(
    name = "evaluator",
    hdrs = [
        "Evaluator.h"
    ],
    deps = [
        ":instruction",
        "//eval/util:code_annotation",
        "//math/field/util",
    ],
    visibility = [
        "//operation/eval:__pkg__",
    ],
)

cc_library(
    name = "instruction",
    hdrs = [
        "Instruction.h"
    ],
    deps = [
        ":InstructionEnum",
    ],
    visibility = [
        "//eval/util:__pkg__",
    ],
)

cc_library(
    name = "univ_evaluator",
    hdrs = [
        "UnivEvaluator.h"
    ],
    deps = [
       ":evaluator",
    ],
    visibility = [
        "//interp:__pkg__",
    ],
)