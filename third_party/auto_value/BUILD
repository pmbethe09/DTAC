licenses(["notice"])  # Apache License 2.0

java_plugin(
    name = "autovalue-plugin",
    generates_api = 1,
    processor_class = "com.google.auto.value.processor.AutoValueProcessor",
    deps = ["@auto_value_repo//jar"],
)

java_library(
    name = "auto_value",
    exported_plugins = [":autovalue-plugin"],
    visibility = ["//visibility:public"],
    exports = ["@auto_value_repo//jar"],
)
