cc_binary(
    name = "geometry_shader_primitives",
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
        "//assimp",
        "//gl",
        "//glew",
        "//glfw",
        "//glm",
    ],
)
