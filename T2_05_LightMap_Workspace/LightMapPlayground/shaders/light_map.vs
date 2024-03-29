#version 330

//By default, in this demo the box object is located in the following position:
//  0.0, 0.0, 0.0

uniform mat4 u_m_matrix;
uniform mat4 u_vp_matrix;
uniform float u_time;
uniform vec3 u_camera_position;
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_uv;
out vec2 uv;
out vec3 normal;
out vec3 pos;

void main() {
  pos = (u_m_matrix * vec4(a_position, 1.0)).xyz;
  gl_Position = u_vp_matrix * u_m_matrix * vec4(a_position, 1.0);
  normal = (u_m_matrix * vec4(a_normal, 0.0)).xyz;
  uv = a_uv;
}
