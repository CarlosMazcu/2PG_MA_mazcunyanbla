#version 330

uniform sampler2D u_texture;
out vec4 fragColor;
uniform float u_time;

in vec3 normal;
in vec2 uv;

void main() {
  vec3 background_color = vec3(1.0, 0.8, 0.7);
  vec3 inner_color = vec3(1.0, 0.0, 0.0);
  vec3 outer_color = vec3(0.0, 0.0, 1.0);
  float offset = sin(u_time * 0.001 + abs(normal.y) + abs(normal.x * 2.0)) * 0.15 + 0.15;
  vec2 center = vec2(0.5, 0.5);
  float inner_threshold = 0.3 - offset;
  float outer_threshold = 0.4 - offset;

  float distance = length(uv - center);
  float outer = step(outer_threshold, distance);
  float inner = step(distance, inner_threshold);

  float alpha = (distance - inner_threshold) / (outer_threshold - inner_threshold);
  alpha = clamp(alpha, 0.0, 1.0);
  vec3 color = mix(inner_color, outer_color, alpha);
  color = mix(color, background_color, outer);
  color = mix(color, background_color, inner);

  fragColor = vec4(color, 1.0);
}
