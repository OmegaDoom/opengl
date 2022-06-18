cc_binary(
    name = "hdr",
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
