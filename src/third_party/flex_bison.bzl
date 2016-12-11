
def cc_flex_bison_library(
    name, flex_file, bison_file,
    hdrs=[], srcs=[], deps=[], visibility=None):

  flex_out_file = flex_file + ".cpp"
  native.genrule(
      name = name + "_flex",
      outs = [flex_out_file],
      srcs = [flex_file],
      cmd = "flex -o $(@D)/%s $(location %s)" % (flex_out_file, flex_file)
  )

  arg_adjust = "$$(bison --version | grep -qE '^bison .* 3\..*' && echo -Wno-deprecated)"
  bison_name = bison_file[:bison_file.rindex('.')] 
  h_out_file =  bison_name + ".hpp"
  cpp_out_file =  bison_name + ".cpp"

  native.genrule(
      name = name + "_bison",
      outs = [h_out_file, cpp_out_file, "stack.hh", "position.hh", "location.hh"],
      srcs = [bison_file],
      cmd = "bison %s -o $(@D)/%s $(location %s)" % (arg_adjust, cpp_out_file, bison_file)
  )

  native.cc_library(
      name = name,
      hdrs = hdrs + [h_out_file, "stack.hh", "position.hh", "location.hh"],
      srcs = srcs + [cpp_out_file, flex_out_file],
      deps = deps,
      visibility = visibility,
  )
