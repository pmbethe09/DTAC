load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:jvm.bzl", "jvm_maven_import_external")

GRPC_VERS = "1.46.6"
GRPC_JAVA_VERS = "1.52.1"

RULES_JVM_EXTERNAL_TAG = "4.5"
RULES_JVM_EXTERNAL_SHA = "b17d7388feb9bfa7f2fa09031b32707df529f26c91ab9e5d909eb1676badd9a6"

def maven_jar(name, artifact, sha256):
    jvm_maven_import_external(
        name = name,
        artifact = artifact,
        artifact_sha256 = sha256,
        server_urls = ["https://repo.maven.apache.org/maven2"],
    )

def dtac_deps():
    http_archive(
        name = "rules_jvm_external",
        strip_prefix = "rules_jvm_external-%s" % RULES_JVM_EXTERNAL_TAG,
        sha256 = RULES_JVM_EXTERNAL_SHA,
        url = "https://github.com/bazelbuild/rules_jvm_external/archive/%s.zip" % RULES_JVM_EXTERNAL_TAG,
    )
    http_archive(
        name = "rules_python",
        sha256 = "8c15896f6686beb5c631a4459a3aa8392daccaab805ea899c9d14215074b60ef",
        strip_prefix = "rules_python-0.17.3",
        url = "https://github.com/bazelbuild/rules_python/archive/refs/tags/0.17.3.tar.gz",
    )
    http_archive(
        name = "com_github_grpc_grpc",
        strip_prefix = "grpc-" + GRPC_VERS,
        urls = [
            "https://github.com/grpc/grpc/archive/refs/tags/v%s.tar.gz" % GRPC_VERS,
        ],
    )

    http_archive(
        name = "io_bazel_rules_go",
        sha256 = "56d8c5a5c91e1af73eca71a6fab2ced959b67c86d12ba37feedb0a2dfea441a6",
        urls = [
            "https://mirror.bazel.build/github.com/bazelbuild/rules_go/releases/download/v0.37.0/rules_go-v0.37.0.zip",
            "https://github.com/bazelbuild/rules_go/releases/download/v0.37.0/rules_go-v0.37.0.zip",
        ],
    )

    http_archive(
        name = "io_grpc_grpc_java",
        sha256 = "2484054e9ac47d3b4d4a797b9a0caaf4f50f23e13efb5b23ce3703b363f13023",
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
        name = "com_googlecode_diffutils",
        artifact = "com.googlecode.java-diff-utils:diffutils:jar:1.3.0",
        sha256 = "61ba4dc49adca95243beaa0569adc2a23aedb5292ae78aa01186fa782ebdc5c2",
    )

    maven_jar(
        name = "com_google_errorprone_error_prone_annotations",
        artifact = "com.google.errorprone:error_prone_annotations:2.1.3",
        sha256 = "03d0329547c13da9e17c634d1049ea2ead093925e290567e1a364fd6b1fc7ff8",
    )

    maven_jar(
        name = "com_google_code_findbugs_jsr305",
        artifact = "com.google.code.findbugs:jsr305:3.0.2",
        sha256 = "766ad2a0783f2687962c8ad74ceecc38a28b9f72a2d085ee438b7813e928d0c7",
    )

    maven_jar(
        name = "com_google_truth_truth",
        artifact = "com.google.truth:truth:1.1.3",
        sha256 = "fc0b67782289a2aabfddfdf99eff1dcd5edc890d49143fcd489214b107b8f4f3",
    )
