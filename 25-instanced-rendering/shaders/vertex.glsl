#version 300 es

layout(location = 0) in vec2 a_Position;
layout(location = 1) in int a_Character;

uniform vec2 u_Offset;
uniform vec2 u_Viewport;

out vec3 v_Color;
out vec2 v_TexCoord;

void main()
{
  float offset = float(gl_InstanceID) * 30.0;

  gl_Position = vec4(
    u_Viewport.x * (a_Position.x * 30.0 + offset) - 1.0,
    1.0 - u_Viewport.y * a_Position.y * 60.0,
    0.0,
    1.0);

  vec2 glyphSize = vec2(6.0 / 96.0, 12.0 / 96.0);

  // position of a_Character in atlas
  int glyph = a_Character - 32;
  int col = glyph % 16;
  int row = glyph / 16;

  v_TexCoord = (vec2(col, row) + a_Position) * glyphSize;
}
