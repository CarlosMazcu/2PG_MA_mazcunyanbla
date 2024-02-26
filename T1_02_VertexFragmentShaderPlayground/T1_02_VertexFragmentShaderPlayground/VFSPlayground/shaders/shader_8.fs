#version 330

uniform sampler2D u_texture;
out vec4 fragColor;
in vec3 normal;

void main() {
  fragColor = vec4(normal + vec3(0.5, 0.5, 0.5), 1.0);
}
