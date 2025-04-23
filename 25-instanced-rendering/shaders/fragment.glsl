#version 300 es
precision mediump float;

uniform sampler2D u_Sampler;
uniform vec3 u_Color;

in vec3 v_Color;
in vec2 v_TexCoord;

out vec4 FragColor;

void main()
{
  float tx = texture(u_Sampler, v_TexCoord).a;
  FragColor = vec4(v_Color, tx);
}
