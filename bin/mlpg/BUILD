# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_library
cc_library(
    name = "_mlpgGO",
    srcs = ["_mlpgGO.cc"],
    hdrs = ["mlpgGO.hpp"],
    copts = [""],
    deps = [
        "//lib:common",
        "@openblas_lib//:dylib",
        "@openblas_hdrs//:hdrs",
    ],
)

# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_binary
cc_binary(
    name = "mlpgGO",
    srcs = ["mlpgGO.cc"],
    copts = [""],
    deps = [
        "//lib:common",
        ":_mlpgGO",
    ],
)