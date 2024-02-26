 #version 330

uniform sampler2D u_texture;
out vec4 fragColor;
in vec2 uv;

void main() {
  vec4 luffy = texture(u_texture, uv);
  fragColor = luffy;
}
