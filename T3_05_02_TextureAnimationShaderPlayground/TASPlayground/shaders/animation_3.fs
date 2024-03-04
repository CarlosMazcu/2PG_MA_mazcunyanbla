#version 330

layout (location = 0) out vec4 fragColor;
uniform sampler2D u_texture;
in vec2 uv;


void main() {
  fragColor = texture(u_texture, uv);
}
