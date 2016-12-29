def build_test(name, targets, size="small", **kwargs):
  genrulename = name + "_gen_empty_test"
  native.genrule(
    name = genrulename,
    outs = ["empty_test.sh"],
    cmd = "touch $@",
  )
  native.sh_test(
    name = name,
    size = size,
    srcs = [genrulename],
    data = targets,
    **kwargs
  )
