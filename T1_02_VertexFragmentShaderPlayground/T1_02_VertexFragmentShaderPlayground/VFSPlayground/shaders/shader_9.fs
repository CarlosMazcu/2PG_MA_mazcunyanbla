#version 330

uniform sampler2D u_texture;
out vec4 fragColor;
in vec3 normal;
void main() {
  fragColor = vec4(normal.x, 0.0, 0.0, 1.0);
}