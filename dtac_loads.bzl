# This file must come after dtac_deps() run, as it installs these repos
load("@com_github_grpc_grpc//bazel:grpc_deps.bzl", "grpc_deps")
load("@io_bazel_rules_go//go:deps.bzl", "go_register_toolchains", "go_rules_dependencies")
load("@io_grpc_grpc_java//:repositories.bzl", "grpc_java_repositories")
load("@rules_python//python:repositories.bzl", "py_repositories")
load("@rules_jvm_external//:repositories.bzl", "rules_jvm_external_deps")
load("@rules_jvm_external//:setup.bzl", "rules_jvm_external_setup")

def dtac_loads():
    rules_jvm_external_deps()
    rules_jvm_external_setup()

    py_repositories()
    grpc_deps()

    go_rules_dependencies()
    go_register_toolchains(version = "1.19.3")

    # grpc uses android java, we want jre java
    grpc_java_repositories()
