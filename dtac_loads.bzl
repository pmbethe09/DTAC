# This file must come after dtac_deps() run, as it installs these repos
load("@com_github_grpc_grpc//bazel:grpc_deps.bzl", "grpc_deps")
load("@io_bazel_rules_go//go:deps.bzl", "go_register_toolchains", "go_rules_dependencies")
load("@io_grpc_grpc_java//:repositories.bzl", "grpc_java_repositories")

def dtac_loads():
    grpc_deps()

    go_rules_dependencies()
    go_register_toolchains()

    # grpc uses android java, we want jre java
    grpc_java_repositories()
