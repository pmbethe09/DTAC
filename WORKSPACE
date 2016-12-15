workspace(name = "com_github_pmbethe09_dtac")

load("//proto:java_proto_library.bzl", "java_proto_repositories")

java_proto_repositories(shared = 1, cc = 1)

new_git_repository(
    name = "com_github_google_googletest",
    remote = "https://github.com/google/googletest",
    tag = "release-1.7.0",
    build_file = "third_party/BUILD.gtest",
)

bind(
    name = "gtest",
    actual = "@com_github_google_googletest//:gtest",
)
