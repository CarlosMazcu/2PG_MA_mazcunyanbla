#version 330

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_uv;
out vec2 uv;
uniform vec2 u_window_size;
uniform vec3 u_camera_position;
uniform float u_time;

//By default, in this demo the camera is in the following position:
//0.0, 2.0, 3.0
//By default, in this demo the object is in the following position:
//0.0, 0.0, 0.0
//By default, in this demo the window has the following size:
//1000.0, 600.0

//Example of function which returns a translation matrix:
mat4 InitAsTranslate(float tx, float ty, float tz) {
  return mat4(1.0, 0.0, 0.0, 0.0,
              0.0, 1.0, 0.0, 0.0,
              0.0, 0.0, 1.0, 0.0,
              tx,  ty,  tz,  1.0);
}

mat4 InitAsScale(float sx, float sy, float sz)
{
  return mat4(sz, 0.0, 0.0, 0.0,
              0.0, sy, 0.0, 0.0,
              0.0, 0.0, sz, 0.0,
              0.0, 0.0, 0.0, 1.0);
}

mat4 InitAsRotate(float angle, vec3 axis)
{
  axis = normalize(axis);
  float s = sin(angle);
  float c = cos(angle);
  float oc = 1.0 - c;
  return mat4(oc * (axis.x * axis.x) + c,        oc * (axis.x * axis.y) - axis.z * s, oc * axis.x * axis.z + axis.y * s, 0.0,
              oc * axis.x * axis.y + axis.z * s, oc * (axis.y * axis.y) + c,          oc * axis.y * axis.z - axis.x * s, 0.0,
              oc * axis.x * axis.z - axis.y * s, oc * axis.y * axis.z + axis.x * s,   oc * (axis.z * axis.z) + c,        0.0,
              0.0,                               0.0,                                 0.0,                               1.0);
}

mat4 InitAsView(vec3 target_point, vec3 camera_position)
{
  vec3 fwd = normalize(target_point - camera_position);
  vec3 left = normalize(cross(fwd, vec3(0.0, 1.0, 0.0)));
  vec3 up = normalize(cross(left, fwd));

  float dot_left_pos = dot(left, camera_position);
  float dot_up_pos = dot(up, camera_position);
  float dot_fwd_pos = dot(fwd, camera_position);

  return mat4(left.x, up.x, -fwd.x, 0.0,
              left.y, up.y, -fwd.y, 0.0,
              left.z, up.z, -fwd.z, 0.0,
              -dot_left_pos, -dot_up_pos, dot_fwd_pos, 1.0);

}

mat4 InitAsProjection(float fov, float aspect, float znear, float zfar){
  float r = tan(fov * 0.5);

  return mat4((1.0)/(aspect * r), 0.0,     0.0,                                 0.0,
              0.0,               (1.0)/r,  0.0,                                 0.0,
              0.0,                0.0,    -((zfar+znear)/(zfar-znear)),        -1.0,
              0.0,                0.0,    -((2.0 * zfar * znear)/(zfar-znear)), 0.0);
}

mat4 InitAsOrtographic(float left, float right, float bottom, float top, float znear, float zfar){

  return mat4( (2.0)/(right-left), 0.0, 0.0, 0.0,
                0.0, (2.0)/(top - bottom), 0.0, 0.0,
                0.0, 0.0, -((2.0)/(zfar-znear)), 0.0,
                -((right + left)/(right - left)), -((top + bottom)/(top - bottom)), -((zfar+znear)/(zfar-znear)), 1.0);
}


void main(){
  mat4 scale = InitAsScale(1.0, 1.0, 1.0);
  mat4 translate = InitAsTranslate(0.0, 0.0, -5.0);
  mat4 translate_origin = InitAsTranslate(2.0, 0.0, 0.0);
  mat4 rotate_orbit = InitAsRotate(u_time * 0.001, vec3(0.0, 1.0, 0.0));

  mat4 rotate_z = InitAsRotate(1.0, vec3(0.0, 0.0, 1.0));
  mat4 rotate_y = InitAsRotate(-u_time * 0.0015, vec3(0.0, 1.0, 0.0));
  mat4 view = InitAsView(vec3(0.0,1.5,0.0), u_camera_position);
  mat4 projection = InitAsProjection(0.6, u_window_size.x/u_window_size.y, 0.01, 100.0);

  mat4 model = translate * rotate_orbit * translate_origin * rotate_z * rotate_y * scale;
  gl_Position = projection * view * model * vec4(a_position, 1.0);
  uv = a_uv;
}