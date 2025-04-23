attribute vec2 a_Position;

uniform vec2 u_Offset;
uniform vec2 u_Viewport;

void main()
{
  gl_Position = vec4(
    u_Viewport.x * (a_Position.x + u_Offset.x) - 1.0,
    1.0 - u_Viewport.y * (a_Position.y + u_Offset.y),
    0.0,
    1.0);
}
