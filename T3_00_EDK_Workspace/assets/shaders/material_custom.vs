#version 330

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_uv;

uniform mat4 u_model;
uniform mat4 u_vp_matrix;
out vec2 uv;

void main(){
  uv = a_uv;
  gl_Position = u_vp_matrix * u_model * vec4(a_position, 1.0);
}