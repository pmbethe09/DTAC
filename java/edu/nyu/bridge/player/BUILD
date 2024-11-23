package(default_visibility = ["//visibility:public"])

java_library(
    name = "player",
    srcs = glob(["**/*.java"]),
    deps = [
        "//bridge_proto:bridge_java_proto",
        "//cards_proto:cards_java_proto",
        "//java/edu/nyu/bridge/scoring",
        "@maven//:com_google_code_findbugs_jsr305",
    ],
)
