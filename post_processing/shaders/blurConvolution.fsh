#version 430 core
in vec2 texCoords;
out vec4 fragColor;

uniform sampler2D diffuse;

uniform int width; 
uniform int height; 

void main()
{
    float pointSizeX = 1.f / width;
    float pointSizeY = 1.f / height;
    vec4 sum = vec4(0.0);
    for (int x = -4; x <= 4; x++)
    {
        for (int y = -4; y <= 4; y++)
        {
            sum += texture(diffuse, vec2(texCoords.x + x * pointSizeX, texCoords.y + y * pointSizeY)) / 81.0;
        }
    }
    fragColor = sum;
}
