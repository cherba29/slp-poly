def _impl(ctx):
  input_file = ctx.files.enum_file[0]
  enum_name = input_file.basename[:-len(input_file.extension)] 
  ctx.actions.run(
      inputs=[input_file],
      #outputs=[enum_name + '.cpp', enum_name + '.h', enum_name + 't.cpp'],
      outputs=[ctx.outputs.header, ctx.outputs.cpp, ctx.outputs.test],
      arguments=[
          "--config", input_file.path,
          "--output_dir", ctx.outputs.cpp.dirname
      ],
      progress_message="Generating from %s" % input_file.short_path,
      executable=ctx.executable._gen_tool)


_gen_cppenum = rule(
    implementation=_impl,
    attrs={
        "enum_name": attr.string(),
        "enum_file": attr.label(
            mandatory=True,
            # allow_files=True,
            allow_single_file=True
        ),
        "_gen_tool": attr.label(
            executable=True,
            cfg="host",
            allow_files=True,
            default=Label("//external:gen_cppenum")
        )
    },
    output_to_genfiles=True,
    outputs={
        "header": "%{enum_name}.h",
        "cpp": "%{enum_name}.cpp",
        "test": "%{enum_name}.t.cpp",
    },
)

def gen_cppenum(enum_file, visibility=None):
  name = enum_file[:enum_file.rindex('.')]
  _gen_cppenum(
      name=name + '_genfiles',
      enum_name=name,
      enum_file=enum_file
  )

  native.cc_library(
      name=name,
      hdrs=[
          name + ".h",
      ],
      srcs=[
          name + ".cpp",
      ],
      visibility=visibility,
  )

  native.cc_test(
      name=name + '_test',
      srcs=[
          name + ".t.cpp",
      ],
      defines = ["BOOST_TEST_MAIN", "BOOST_TEST_DYN_LINK"],
      deps=[
          ":" + name,
          "@boost//:test.so",
      ], 
  )
