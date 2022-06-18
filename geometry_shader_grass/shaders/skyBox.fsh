#version 430 core
in vec3 texCoords;
out vec4 fragColor;

uniform samplerCube diffuse;

void main()
{
    fragColor = texture(diffuse, texCoords);
}
