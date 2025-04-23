#version 300 es

in vec2 a_Position;
in vec2 a_TexCoord;
in vec3 a_Color;

uniform vec2 u_Offset;
uniform vec2 u_Viewport;

out vec3 v_Color;
out vec2 v_TexCoord;

void main()
{
  float offset = float(gl_InstanceID) * 75.0;
  gl_Position = vec4(
    u_Viewport.x * (a_Position.x + offset) - 1.0,
    1.0 - u_Viewport.y * a_Position.y,
    0.0,
    1.0);
  v_Color = a_Color;
  v_TexCoord = a_TexCoord / 96.0;
}
