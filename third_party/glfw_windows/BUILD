cc_import(
    name = "glfw_import",
    static_library = "lib/glfw3.lib",
    visibility = ["//visibility:private"],
)

cc_library(
    name = "glfw_windows",
    hdrs = glob(["include/GLFW/*.h"]),
    strip_include_prefix = "include",
    deps = ["glfw_import"],
    visibility = ["//visibility:public"],
)
