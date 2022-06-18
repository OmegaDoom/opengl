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
    vec4 leftTop = texture(diffuse, vec2(texCoords.x - pointSizeX, texCoords.y + pointSizeY));
    vec4 left = texture(diffuse, vec2(texCoords.x - pointSizeX, texCoords.y));
    vec4 leftBottom = texture(diffuse, vec2(texCoords.x - pointSizeX, texCoords.y - pointSizeY));
    vec4 rightTop = texture(diffuse, vec2(texCoords.x + pointSizeX, texCoords.y + pointSizeY));
    vec4 right = texture(diffuse, vec2(texCoords.x + pointSizeX, texCoords.y));
    vec4 rightBottom = texture(diffuse, vec2(texCoords.x + pointSizeX, texCoords.y - pointSizeY));
    vec4 top = texture(diffuse, vec2(texCoords.x + pointSizeX, texCoords.y - pointSizeY));
    vec4 bottom = texture(diffuse, vec2(texCoords.x + pointSizeX, texCoords.y - pointSizeY));

    vec4 sx = -leftTop - 2 * left - leftBottom + rightTop + 2 * right + rightBottom;
    vec4 sy = -leftTop - 2 * top - rightTop + leftBottom + 2 * bottom + rightBottom;
    fragColor = sqrt(sx * sx + sy * sy);
}
