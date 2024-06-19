precision mediump float;

uniform sampler2D u_Sampler;
uniform vec3 u_Color;

varying vec2 v_TexCoord;

void main()
{
  gl_FragColor = vec4(u_Color.xyz, 1.0);
}
