#version 330

uniform mat4 u_vp_matrix;
uniform mat4 u_m_matrix;
uniform float u_time;

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_uv;

out vec2 uv;

mat3 RotateZ(float angle){
  return mat3(cos(angle), sin(angle), 0.0,
              -sin(angle), cos(angle), 0.0,
               0.0,      0.0,         1.0);
}

mat3 Translate2D(vec2 xy)
{
  return mat3(1.0, 0.0, 0.0,
              0.0, 1.0, 0.0,
              xy.x, xy.y, 1.0);
}

mat3 Scale2D(vec2 xy)
{
  return mat3(xy.x, 0.0, 0.0,
              0.0, xy.y, 0.0,
              0.0, 0.0, 1.0);
}

void main() {
  gl_Position = u_vp_matrix * u_m_matrix * vec4(a_position, 1.0);
  vec3 tmp = vec3(a_uv, 1.0);

  tmp = Translate2D(vec2(-0.5, -0.5)) * tmp;
  
  //tmp = RotateZ(u_time * 0.001) * tmp,
  tmp = Scale2D(vec2(cos(u_time * 0.001) + 2.0, (-cos(u_time * 0.001) + 2.0))) * tmp;
  tmp = Translate2D(vec2(0.5, 0.5)) * tmp;
  uv = tmp.xy;

  
}