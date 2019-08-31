#version 450 core

out vec4 FragColor;
uniform float x, y;

void main()
{
  FragColor = vec4(x, y, 0.0f, 1.0f);
}
