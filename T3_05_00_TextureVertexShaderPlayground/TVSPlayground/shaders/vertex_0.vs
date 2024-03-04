#version 330

uniform mat4 u_vp_matrix;
uniform mat4 u_m_matrix;
uniform float u_time;

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_uv;

out vec2 uv;

void main() {
  gl_Position = u_vp_matrix * u_m_matrix * vec4(a_position, 1.0);
  uv = a_uv;
}//done