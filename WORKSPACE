workspace(name = "com_github_pmbethe09_dtac")

PROTO_VERS = "3.8.0"

PROTO_SHA = "1e622ce4b84b88b6d2cdf1db38d1a634fe2392d74f0b7b74ff98f3a51838ee53"

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")

http_archive(
    name = "com_google_protobuf",
    sha256 = PROTO_SHA,
    strip_prefix = "protobuf-" + PROTO_VERS,
    urls = ["https://github.com/google/protobuf/archive/v" + PROTO_VERS + ".zip"],
)

load("@com_google_protobuf//:protobuf_deps.bzl", "protobuf_deps")

# Load common dependencies.
protobuf_deps()

http_archive(
    name = "com_github_google_protobuf",
    sha256 = PROTO_SHA,
    strip_prefix = "protobuf-" + PROTO_VERS,
    urls = ["https://github.com/google/protobuf/archive/v" + PROTO_VERS + ".zip"],
)

new_git_repository(
    name = "com_github_google_googletest",
    build_file = "//:third_party/BUILD.gtest",
    remote = "https://github.com/google/googletest",
    tag = "release-1.8.0",
)

git_repository(
    name = "io_bazel_rules_go",
    remote = "https://github.com/bazelbuild/rules_go.git",
    tag = "0.18.5",
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
