#extension GL_OES_standard_derivatives: enable

precision mediump float;

uniform sampler2D u_Sampler;
uniform vec3 u_Color;

varying vec2 v_TexCoord;

float median(float r, float g, float b) {
  return max(min(r, g), min(max(r, g), b));
}

void main()
{
  vec3 s = texture2D(u_Sampler, v_TexCoord).rgb;

  // Signed distance
  float sigDist = median(s.r, s.g, s.b) - 0.5;
  float alpha = clamp(sigDist / fwidth(sigDist) + 0.5, 0.0, 1.0);

  // Alpha Test
  if (alpha < 0.01) discard;

  // Output
  gl_FragColor = vec4(1.0, 1.0, 1.0, alpha);
}
