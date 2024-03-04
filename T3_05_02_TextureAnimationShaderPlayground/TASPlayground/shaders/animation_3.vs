#version 330

uniform mat4 u_vp_matrix;
uniform mat4 u_m_matrix;
uniform float u_time;

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_uv;

out vec2 uv;

mat3 Scale2D(vec2 xy)
{
  return mat3(xy.x, 0.0, 0.0,
              0.0, xy.y, 0.0,
              0.0, 0.0, 1.0);
}

mat3 Translate2D(vec2 xy)
{
  return mat3(1.0, 0.0, 0.0,
              0.0, 1.0, 0.0,
              xy.x, xy.y, 1.0);
}


void main() {
  gl_Position = u_vp_matrix * u_m_matrix * vec4(a_position, 1.0);
  uv = a_uv;
  float r = 1.0/6.0;
  uv.x *= r;
  uv.x += 1.0 - floor(u_time * 0.005) * r;
}