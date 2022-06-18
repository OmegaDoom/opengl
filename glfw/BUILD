cc_library(
    name = "glfw",
    deps = select(
	{
            "@platforms//os:linux": ["@glfw_linux"],
            "@platforms//os:windows": ["@glfw_windows"],
	},
    ),
    linkopts = select(
	{
            "@platforms//os:linux": [],
            "@platforms//os:windows": ["user32.lib", "gdi32.lib", "shell32.lib"],
	},
    ),
    visibility = ["//visibility:public"],
)
