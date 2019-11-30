workspace(name = "com_github_pmbethe09_dtac")

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# 1.25
GRPC_VERS = "06a61758461284d210b1481ad5592d3fb6f05002"

http_archive(
    name = "com_github_grpc_grpc",
    strip_prefix = "grpc-" + GRPC_VERS,
    urls = [
        "https://github.com/grpc/grpc/archive/%s.tar.gz" % GRPC_VERS,
    ],
)

load("@com_github_grpc_grpc//bazel:grpc_deps.bzl", "grpc_deps")

grpc_deps()

RULES_GO_VERS = "v0.20.2/rules_go-v0.20.2.tar.gz"

http_archive(
    name = "io_bazel_rules_go",
    sha256 = "b9aa86ec08a292b97ec4591cf578e020b35f98e12173bbd4a921f84f583aebd9",
    urls = [
        "https://storage.googleapis.com/bazel-mirror/github.com/bazelbuild/rules_go/releases/download/" + RULES_GO_VERS,
        "https://github.com/bazelbuild/rules_go/releases/download/" + RULES_GO_VERS,
    ],
)

load("@io_bazel_rules_go//go:deps.bzl", "go_register_toolchains", "go_rules_dependencies")

go_rules_dependencies()

go_register_toolchains()

maven_jar(
    name = "aopalliance_repo",
    artifact = "aopalliance:aopalliance:1.0",
    sha1 = "0235ba8b489512805ac13a8f9ea77a1ca5ebe3e8",
)

maven_jar(
    name = "auto_value_repo",
    artifact = "com.google.auto.value:auto-value:1.5.3",
    sha1 = "514df6a7c7938de35c7f68dc8b8f22df86037f38",
)

maven_jar(
    name = "errorprone_repo",
    artifact = "com.google.errorprone:error_prone_annotations:jar:2.3.2",
    sha1 = "d1a0c5032570e0f64be6b4d9c90cdeb103129029",
)

maven_jar(
    name = "com_google_guava_guava",
    artifact = "com.google.guava:guava:24.0-jre",
    sha1 = "041ac1e74d6b4e1ea1f027139cffeb536c732a81",
)

maven_jar(
    name = "guice_repo",
    artifact = "com.google.inject:guice:4.2.0",
    sha1 = "25e1f4c1d528a1cffabcca0d432f634f3132f6c8",
)

maven_jar(
    name = "guice_assistedinject_repo",
    artifact = "com.google.inject.extensions:guice-assistedinject:4.2.0",
    sha1 = "e7270305960ad7db56f7e30cb9df6be9ff1cfb45",
)

maven_jar(
    name = "com_google_code_findbugs_jsr305",
    artifact = "com.google.code.findbugs:jsr305:3.0.1",
    sha1 = "f7be08ec23c21485b9b5a1cf1654c2ec8c58168d",
)

maven_jar(
    name = "jsr330_repo",
    artifact = "javax.inject:javax.inject:1",
    sha1 = "6975da39a7040257bd51d21a231b76c915872d38",
)

maven_jar(
    name = "com_google_truth_truth",
    artifact = "com.google.truth:truth:0.39",
    sha1 = "bd1bf5706ff34eb7ff80fef8b0c4320f112ef899",
)
