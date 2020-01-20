package(default_visibility = ["//visibility:public"])

java_library(
    name = "cards",
    srcs = glob(["**/*.java"]),
    deps = [
        "//cards_proto:cards_java_proto",
        "//third_party/auto_value",
        "@com_google_code_findbugs_jsr305//jar",
        "@com_google_errorprone_error_prone_annotations//jar",
        "@com_google_guava_guava//jar",
    ],
)
