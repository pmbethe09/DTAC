load("@rules_shell//shell:sh_test.bzl", "sh_test")

def build_test(name, targets, size = "small", **kwargs):
    genrulename = name + "_gen_empty_test"
    native.genrule(
        name = genrulename,
        outs = ["empty_test.sh"],
        cmd = "touch $@",
    )
    sh_test(
        name = name,
        size = size,
        srcs = [genrulename],
        data = targets,
        **kwargs
    )
