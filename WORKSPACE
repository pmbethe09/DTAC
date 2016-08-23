workspace(name = "dtac")

new_git_repository(
    name = "gtest_repo",
    remote = "https://github.com/google/googletest",
    tag = "release-1.7.0",
    build_file = "third_party/BUILD.gtest",
)

bind(
    name = "gtest",
    actual = "@gtest_repo//:gtest",
)
