#version 330 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texture1;
uniform vec2 mousePos;

void main()
{
  /* FragColor = mix(texture(texture1, TexCoord), vec4(ourColor, 1.0f), 0.5); */
  /* FragColor = vec4(TexCoord, 1.0f, 1.0f); */
  float dist = 1 - distance(TexCoord, mousePos)*3;
  vec4 texpoint = texture(texture1, TexCoord);
  FragColor = vec4(texpoint * dist);
  /* FragColor = light; */
}
