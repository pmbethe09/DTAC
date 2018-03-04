workspace(name = "com_github_pmbethe09_dtac")

PROTO_VERS = "3.5.1"
PROTO_SHA = "1f8b9b202e9a4e467ff0b0f25facb1642727cdf5e69092038f15b37c75b99e45"

http_archive(
    name = "com_google_protobuf",
    sha256 = PROTO_SHA,
    strip_prefix = "protobuf-" + PROTO_VERS,
    urls = ["https://github.com/google/protobuf/archive/v" + PROTO_VERS +".zip"],
)

http_archive(
    name = "com_github_google_protobuf",
    sha256 = PROTO_SHA,
    strip_prefix = "protobuf-" + PROTO_VERS,
    urls = ["https://github.com/google/protobuf/archive/v" + PROTO_VERS + ".zip"],
)

http_archive(
    name = "com_google_protobuf_java",
    sha256 = PROTO_SHA,
    strip_prefix = "protobuf-" + PROTO_VERS,
    urls = ["https://github.com/google/protobuf/archive/v" + PROTO_VERS +".zip"],
)

http_archive(
    name = "com_google_protobuf_cc",
    sha256 = PROTO_SHA,
    strip_prefix = "protobuf-" + PROTO_VERS,
    urls = ["https://github.com/google/protobuf/archive/v"+PROTO_VERS+".zip"],
)

new_git_repository(
    name = "com_github_google_googletest",
    build_file = "third_party/BUILD.gtest",
    remote = "https://github.com/google/googletest",
    tag = "release-1.8.0",
)

bind(
    name = "gtest",
    actual = "@com_github_google_googletest//:gtest",
)

maven_jar(
    name = "aopalliance_repo",
    artifact = "aopalliance:aopalliance:1.0",
    sha1 = "0235ba8b489512805ac13a8f9ea77a1ca5ebe3e8",
)

maven_jar(
    name = "auto_value_repo",
    artifact = "com.google.auto.value:auto-value:1.4.1",
    sha1 = "8172ebbd7970188aff304c8a420b9f17168f6f48",
)

maven_jar(
    name = "guava_repo",
    artifact = "com.google.guava:guava:19.0",
    sha1 = "6ce200f6b23222af3d8abb6b6459e6c44f4bb0e9",
)

maven_jar(
    name = "guice_repo",
    artifact = "com.google.inject:guice:4.0",
    sha1 = "0f990a43d3725781b6db7cd0acf0a8b62dfd1649",
)

maven_jar(
    name = "guice_assistedinject_repo",
    artifact = "com.google.inject.extensions:guice-assistedinject:4.0",
    sha1 = "8fa6431da1a2187817e3e52e967535899e2e46ca",
)

maven_jar(
    name = "jsr305_repo",
    artifact = "com.google.code.findbugs:jsr305:3.0.1",
    sha1 = "f7be08ec23c21485b9b5a1cf1654c2ec8c58168d",
)

maven_jar(
    name = "jsr330_repo",
    artifact = "javax.inject:javax.inject:1",
    sha1 = "6975da39a7040257bd51d21a231b76c915872d38",
)
