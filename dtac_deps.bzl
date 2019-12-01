load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:jvm.bzl", "jvm_maven_import_external")

# 1.25
GRPC_VERS = "06a61758461284d210b1481ad5592d3fb6f05002"
GRPC_JAVA_VERS = "1.24.2"
RULES_GO_VERS = "v0.20.2/rules_go-v0.20.2.tar.gz"

def maven_jar(name, artifact, sha256):
    jvm_maven_import_external(
        name = name,
        artifact = artifact,
        artifact_sha256 = sha256,
        server_urls = ["http://central.maven.org/maven2"],
    )

def dtac_deps():
    http_archive(
        name = "com_github_grpc_grpc",
        strip_prefix = "grpc-" + GRPC_VERS,
        urls = [
            "https://github.com/grpc/grpc/archive/%s.tar.gz" % GRPC_VERS,
        ],
    )

    http_archive(
        name = "io_bazel_rules_go",
        sha256 = "b9aa86ec08a292b97ec4591cf578e020b35f98e12173bbd4a921f84f583aebd9",
        urls = [
            "https://storage.googleapis.com/bazel-mirror/github.com/bazelbuild/rules_go/releases/download/" + RULES_GO_VERS,
            "https://github.com/bazelbuild/rules_go/releases/download/" + RULES_GO_VERS,
        ],
    )

    http_archive(
        name = "io_grpc_grpc_java",
        sha256 = "4fddae88f00f73c0771957ce07e07128c9ac33ca41486aa07364a1218b6abdb5",
        strip_prefix = "grpc-java-" + GRPC_JAVA_VERS,
        urls = ["https://github.com/grpc/grpc-java/archive/v%s.zip" % GRPC_JAVA_VERS],
    )

    maven_jar(
        name = "com_google_guava_guava",
        artifact = "com.google.guava:guava:24.0-jre",
        sha256 = "e0274470b16ba1154e926b5f54ef8ae159197fbc356406bda9b261ba67e3e599",
    )

    maven_jar(
        name = "auto_value_repo",
        artifact = "com.google.auto.value:auto-value:1.5.3",
        sha256 = "238d3b7535096d782d08576d1e42f79480713ff0794f511ff2cc147363ec072d",
    )

    maven_jar(
        name = "jsr330_repo",
        artifact = "javax.inject:javax.inject:1",
        sha256 = "91c77044a50c481636c32d916fd89c9118a72195390452c81065080f957de7ff",
    )

    maven_jar(
        name = "com_google_truth_truth",
        artifact = "com.google.truth:truth:0.39",
        sha256 = "25ce04464511d4a7c05e1034477900a897228cba2f86110d2ed49c956d9a82af",
    )
