"""A basic java_proto_library.

Takes .proto as srcs and java_proto_library as deps

A java_proto_library can then be a dependency of java_library
or another java_proto_library.

Does:
Generate protos using the open-source protoc and protoc-gen-grpc-java.
Handles transitive dependencies.
gRPC for service generation

Usage:
In WORKSPACE
load("@com_github_pmbethe09_dtac//proto:java_proto_library.bzl", "java_proto_repositories")

java_proto_repositories()

Then in the BUILD file where protos are

load("@com_github_pmbethe09_dtac//proto:java_proto_library.bzl", "java_proto_library")

java_proto_library(
  name = "my_proto",
  srcs = ["my.proto"],
  deps = [
    ":other_proto",
  ],
)
"""

_GEN_SUFFIX = "_srcjar"

def _external_dirs(files):
  """Compute any needed -I options to protoc from external filegroups."""
  return set(["/".join(f.dirname.split("/")[:2])
              for f in files if f.dirname[:9] == "external/"])
    
def _include_dirs(protos):
  dirs = _external_dirs(protos)
  if not dirs:
	return ""
  return " " + " ".join(["-I" + d for d in dirs])

def _java_proto_library_gen_impl(ctx):
  """generates java srcs from .proto."""
  protos = []
  for d in ctx.attr.deps:
    protos += d._protos
  tmpdir = ctx.outputs.srcjar.path + ".dir"
  grpc_cmd = ""
  grpc_inputs = []
  if ctx.attr.grpc:
    grpc_cmd = (" --grpc-java_out=" + tmpdir
                + " --plugin=protoc-gen-grpc-java="
                + ctx.executable._protoc_gen_grpc_java.path + " ")
    grpc_inputs = [ctx.executable._protoc_gen_grpc_java]
  cmds = ["set -e", "rm -rf " + tmpdir, "mkdir -p " + tmpdir,
          ctx.executable.protoc.path + grpc_cmd + " --java_out=" + tmpdir 
          + " -I." + _include_dirs(protos)
          + " " + " ".join([f.path for f in ctx.files.srcs]),
          ctx.executable._jar.path + " cf " + ctx.outputs.srcjar.path + " -C " + tmpdir + " ."]
  run = ctx.new_file(ctx.configuration.bin_dir, ctx.outputs.srcjar.basename + ".run")
  ctx.file_action(
      output = run,
      content = "\n".join(cmds),
      executable = True)
  ctx.action(
      inputs=(grpc_inputs + protos + ctx.files.srcs + ctx.files._jdk
      	+ [ctx.executable.protoc, ctx.executable._jar, run]),
      outputs=[ctx.outputs.srcjar],
      executable=run,
      mnemonic="GenJavaProto")
  return struct(_protos=protos+ctx.files.srcs)

_java_proto_library_gen = rule(
    attrs = {
        "deps": attr.label_list(providers = ["_protos"]),
        "srcs": attr.label_list(
            mandatory = True,
            allow_files = [".proto"],
        ),
        "protoc": attr.label(
            default = Label("@com_github_google_protobuf//:protoc"),
            executable = True,
            single_file = True,
            allow_files = True,
            cfg = "host",
        ),
        "grpc": attr.int(default = 0),
        "_protoc_gen_grpc_java": attr.label(
            default = Label("@com_github_grpc_java//:protoc-gen-grpc-java"),
            executable = True,
            single_file = True,
            allow_files = True,
            cfg = "host",
        ),
        "_protos": attr.label_list(),
        "_jar": attr.label(
            default = Label("@bazel_tools//tools/jdk:jar"),
            allow_files = True,
            executable = True,
            single_file = True,
            cfg = "host",
        ),
        # Needed for _jar, see bazel issue #938
        "_jdk": attr.label(
            default = Label("@bazel_tools//tools/jdk:jdk"),
            allow_files = True,
        ),
    },
    outputs = {"srcjar": "lib%{name}.srcjar"},
    implementation = _java_proto_library_gen_impl,
    output_to_genfiles = True,
)

def _add_target_suffix(target, suffix):
  idx = target.find(":")
  if idx != -1:
    return target + suffix
  return target + ":" + target.split("/")[-1] + suffix

def java_proto_library(name, srcs, deps = None, visibility = None,
                       testonly = 0, has_services = 0):
  """Creates a java_library after generating a java files from the input protos."""
  if not deps:
    deps = []
  _java_proto_library_gen(
      name = name + _GEN_SUFFIX,
      srcs = srcs,
      grpc = has_services,
      deps = [_add_target_suffix(d, _GEN_SUFFIX) for d in deps],
      visibility = visibility,
      testonly = testonly,
  )

  grpc_deps = []
  if has_services:
    grpc_deps = [
        "@com_github_grpc_java//:jar",
        "@guava_maven//jar",
    ]
  native.java_library(
      name = name,
      srcs = [":" + name + _GEN_SUFFIX],
      deps = ["@protobuf_java_maven//jar"] + deps + grpc_deps,
      visibility = visibility,
      testonly = testonly,
  )

def common_proto_repositories():
  """Common things to all langauge proto deps."""
  native.git_repository(
      name = "com_github_google_protobuf",
      remote = "https://github.com/google/protobuf",
      tag = "v3.0.0",
  )
  native.git_repository(
      name = "com_github_grpc_grpc",
      remote = "https://github.com/grpc/grpc",
      tag = "v1.0.1",
  )

grpc_build_file = """
cc_binary(
  name = "protoc-gen-grpc-java",
  srcs = glob(["compiler/src/java_plugin/cpp/*"]),
  visibility = ["//visibility:public"],
  deps = ["@com_github_google_protobuf//:protoc_lib"],
)

java_library(
  name = "jar",
  srcs = glob([
      "context/src/main/java/**/*.java",
      "core/src/main/java/**/*.java",
      "protobuf/src/main/java/**/*.java",
      "protobuf-lite/src/main/java/**/*.java",
      "stub/src/main/java/**/*.java",
    ],
  ),
  javacopts = ["-extra_checks:off"],
  visibility = ["//visibility:public"],
  deps = [
      "@guava_maven//jar",
      "@jsr305_maven//jar",
      "@protobuf_java_util_maven//jar",
      "@protobuf_java_maven//jar",
   ],
)
"""

_zlib_build_file = """
cc_library(
  name = "zlib",
  srcs = glob(["*.c"]),
  hdrs = glob(["*.h"]),
  visibility = ["//visibility:public"],
  copts = [
    "-Wno-unused-variable",
    "-Wno-implicit-function-declaration",
    "-Wno-shift-negative-value",
  ],
)
"""

_nanopb_build_file = """
cc_library(
  name = "nanopb",
  visibility = ["//visibility:public"],
  hdrs = [
    "third_party/nanopb/pb.h",
    "third_party/nanopb/pb_common.h",
    "third_party/nanopb/pb_decode.h",
    "third_party/nanopb/pb_encode.h",
  ],
  srcs = [
    "third_party/nanopb/pb_common.c",
    "third_party/nanopb/pb_decode.c",
    "third_party/nanopb/pb_encode.c",
  ],
)
"""

def cc_proto_repositories():
  native.bind(
    name = "protobuf_clib",
    actual = "@com_github_google_protobuf//:protobuf",
  )
  native.bind(
    name = "protobuf_compiler",
    actual = "@com_github_google_protobuf//:protoc_lib",
  )
  native.bind(
    name = "grpc_cpp_plugin",
    actual = "@com_github_grpc_grpc//:grpc_cpp_plugin",
  )
  native.bind(
    name = "grpc_lib",
    actual = "@com_github_grpc_grpc//:grpc++_reflection",
  )
  native.new_git_repository(
    name = "com_github_nanopb_nanopb",
    # Hack until a grpc tag has the third_party/nanopb/BUILD file 
    remote = "https://github.com/grpc/grpc",
    tag = "v1.0.1",
    build_file_content = _nanopb_build_file,
  )
  native.bind(
    name="nanopb",
    actual="@com_github_nanopb_nanopb//:nanopb",
  )
  native.git_repository(
    name = "boringssl",
    remote = "https://boringssl.googlesource.com/boringssl",
    commit = "36b3ab3e5d3a4892444a698f7989f2150824d804",
  )
  native.bind(
    name="libssl",
    actual="@boringssl//:ssl",
  )
  native.new_git_repository(
    name = "com_github_madler_zlib",
    remote = "https://github.com/madler/zlib",
    tag = "v1.2.8",
    build_file_content = _zlib_build_file,
  )
  native.bind(
    name="zlib",
    actual="@com_github_madler_zlib//:zlib",
  )

def java_proto_repositories(cc = 0, shared = 1):
  """use this to get all needed dependencies for java_proto_library."""
  if shared:
    common_proto_repositories()  # provides com_github_google_protobuf

  if cc:
     cc_proto_repositories()

  native.new_git_repository(
      name = "com_github_grpc_java",
      remote = "https://github.com/grpc/grpc-java",
      tag = "v1.0.1",
      build_file_content = grpc_build_file,
  )
  native.maven_jar(
      name = "protobuf_java_maven",
      artifact = "com.google.protobuf:protobuf-java:3.0.0",
      sha1 = "6d325aa7c921661d84577c0a93d82da4df9fa4c8",
  )
  native.maven_jar(
      name = "protobuf_java_util_maven",
      artifact = "com.google.protobuf:protobuf-java-util:jar:3.0.0",
      sha1 = "5085a47f398f229ef2f07fb496099461e8f4c56c",
  )
  native.maven_jar(
      name = "guava_maven",
      artifact = "com.google.guava:guava:19.0",
      sha1 = "6ce200f6b23222af3d8abb6b6459e6c44f4bb0e9",
  )
  native.maven_jar(
      name = "jsr305_maven",
      artifact = "com.google.code.findbugs:jsr305:3.0.1",
      sha1 = "f7be08ec23c21485b9b5a1cf1654c2ec8c58168d",
  )

