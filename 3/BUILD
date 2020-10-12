licenses(["notice"])  # 3-clause BSD

package(default_visibility = ["//visibility:public"])

load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_proto_library")
load("@com_github_grpc_grpc//bazel:cc_grpc_library.bzl", "cc_grpc_library")

proto_library(
    name = "func_calc_proto",
    srcs = ["func_calc.proto"],
)


cc_grpc_library(
    name = "func_calc_cc_grpc",
    srcs = [":func_calc_proto"],
    grpc_only = True,
    deps = [":func_calc_cc_proto"],
)

cc_proto_library(
    name = "func_calc_cc_proto",
    deps = [":func_calc_proto"],
)

cc_binary(
    name = "greeter_client",
    srcs = ["greeter_client.cc"],
    defines = ["BAZEL_BUILD"],
    deps = [
        ":func_calc_cc_grpc",
        "@com_github_grpc_grpc//:grpc++",
    ],
)

cc_binary(
    name = "greeter_server",
    srcs = ["greeter_server.cc"],
    defines = ["BAZEL_BUILD"],
    deps = [
        ":func_calc_cc_grpc",
        "@com_github_grpc_grpc//:grpc++",
    ],
)