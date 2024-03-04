#version 330

layout (location = 0) out vec4 fragColor;
uniform sampler2D u_texture;
in vec2 uv;


void main() {
  vec2 tmp = uv * 6.0;
  fragColor = texture(u_texture, tmp);
}
