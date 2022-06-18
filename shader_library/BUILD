cc_binary(
    name = "shader_library",
    srcs = glob([
        "src/*.cpp",
        "src/*.hpp",
        "src/*.h",
    ]),
    data = glob([
        "*.png",
        "shaders/*",
    ]),
    deps = [
        "//gl",
        "//glew",
        "//glfw",
        "//glm",
    ],
)
