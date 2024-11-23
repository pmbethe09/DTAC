licenses(["notice"])  # Apache License 2.0

java_plugin(
    name = "autovalue-plugin",
    generates_api = 1,
    processor_class = "com.google.auto.value.processor.AutoValueProcessor",
    deps = ["@maven//:com_google_auto_value_auto_value"],
)

java_library(
    name = "auto_value",
    exported_plugins = [":autovalue-plugin"],
    visibility = ["//visibility:public"],
    exports = ["@maven//:com_google_auto_value_auto_value_annotations"],
)
