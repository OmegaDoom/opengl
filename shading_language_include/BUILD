genrule(
    name = "gen_constants",
    srcs = ["src/Constants.hpp"],
    outs = ["binary_constants.hpp"],
    cmd = "python $(locations src/constants_generator.py) $(locations src/Constants.hpp) > $@",
    tools = ["src/constants_generator.py"]
)

cc_library(
  name = "gen_constants_lib",
  hdrs = [":gen_constants"],
)

cc_binary(
    name = "shading_language_include",
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
        ":gen_constants_lib",
    ],
)
