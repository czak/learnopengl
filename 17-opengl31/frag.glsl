#version 450 core

uniform Main {
  uniform vec4 color;
};

out vec4 FragColor;

void main()
{
  FragColor = color;
}
