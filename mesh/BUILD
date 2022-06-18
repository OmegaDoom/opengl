cc_binary(
    name = "mesh",
    srcs = glob([
        "src/*.cpp",
        "src/*.hpp",
        "src/*.h",
    ]),
    data = glob([
        "resources/**/*",
        "shaders/*",
    ]),
    deps = [
        "//gl",
        "//glew",
        "//glfw",
        "//glm",
    ],
)
