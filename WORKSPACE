OpenBLAS_root = "/opt/OpenBLAS/"

new_local_repository(
    name = "openblas_lib",
    path = OpenBLAS_root+"/lib",
    build_file_content = """
cc_library(
    name = "dylib",
    srcs = ["libopenblas.dylib"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "lib",
    srcs = ["libopenblas.a"],
    visibility = ["//visibility:public"],
)
    """,
)

new_local_repository(
    name = "openblas_hdrs",
    path = OpenBLAS_root+"/include",
    build_file_content = """
cc_library(
    name = "hdrs",
    hdrs = glob(["*.h"]),
    visibility = ["//visibility:public"],
)
    """,
)

MKL_root = "/opt/intel/mkl/"

new_local_repository(
    name = "mkl_lib",
    path = MKL_root+"/lib",
    build_file_content = """
cc_library(
    name = "dylib",
    srcs = glob(["*.dylib"]),
    visibility = ["//visibility:public"],
)

cc_library(
    name = "lib",
    srcs = glob(["*.a"]),
    visibility = ["//visibility:public"],
)
    """,
)

new_local_repository(
    name = "mkl_hdrs",
    path = MKL_root+"/include",
    build_file_content = """
cc_library(
    name = "hdrs",
    hdrs = glob(["*.h"]),
    visibility = ["//visibility:public"],
)
    """,
)
