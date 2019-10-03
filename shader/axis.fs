#version 330 core

out vec4 fragColor;
in vec3 bColor;

void main()
{
    fragColor = vec4(bColor, 1.0f);
}