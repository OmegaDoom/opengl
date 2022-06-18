cc_library(
    name = "gl",
    deps = select(
	{
            "@platforms//os:linux": ["@gl_linux"],
            "@platforms//os:windows": [],
	},
    ),
    linkopts = select(
        {
            "@platforms//os:linux": [],
            "@platforms//os:windows": ["opengl32.lib"],
        },
    ),
    visibility = ["//visibility:public"],
)
