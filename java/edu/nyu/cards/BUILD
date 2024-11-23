package(default_visibility = ["//visibility:public"])

java_library(
    name = "cards",
    srcs = glob(["**/*.java"]),
    deps = [
        "//cards_proto:cards_java_proto",
        "//third_party/auto_value",
        "@maven//:com_google_code_findbugs_jsr305",
        "@maven//:com_google_errorprone_error_prone_annotations",
        "@maven//:com_google_guava_guava",
    ],
)
