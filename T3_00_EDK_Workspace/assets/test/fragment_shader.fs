#version 330
uniform sampler2D u_albedo_1;
uniform sampler2D u_albedo_2;
uniform vec4 u_color;
uniform int u_use_texture;
in vec2 uv;
out vec4 fragColor;
void main() {
    if (0 == u_use_texture) {
        fragColor = u_color;
    }
    else {
        fragColor = u_color * texture(u_albedo_1, uv);
    }
}