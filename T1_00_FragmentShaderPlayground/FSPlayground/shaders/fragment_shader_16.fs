#version 330
#define M_PI 3.14159265358979323846 
uniform vec4 u_color;
uniform float u_time;
//In this demo, the window has the following size:
// 1200 x 700
uniform vec2 u_window_size;
out vec4 fragColor;

void main() {
  vec3 bg_color = vec3(1.0, 1.0, 1.0);
  float pixel = 4.7;
  float r = 1.0/pixel;
  vec2 window_size = u_window_size * r;
  vec2  pixel_coord;
  modf(gl_FragCoord.x / pixel, pixel_coord.x);
  modf(gl_FragCoord.y / pixel, pixel_coord.y);

  //body
  vec2 center = window_size * 0.5;
  float big_radius = 140.0 * r;
  float distance = length(pixel_coord - center);
  vec3 pacman_color = vec3(1.0, 1.0, 0.0);
  float c = step(distance, big_radius);
  vec3 color = mix(bg_color, pacman_color, c);


  //eye
  float petit_radius = 15.0 * r;
  float distance_eye = length(pixel_coord - vec2(window_size.x * 0.55, window_size.y * 0.6));
  float eye = step(distance_eye, petit_radius);
  color = mix(color, bg_color, eye);

  //mouth
  float mouth_threshold = 0.90 + (sin(u_time * 0.001) * 0.5 + 0.5) * 0.10;
  vec2 angle = vec2(cos(0.0), sin(0.0));
  vec2 mouth_center = center;
  float mouth_dot = dot(angle, normalize(pixel_coord - mouth_center));
  float mouth = step(mouth_dot, mouth_threshold);
  color = mix(bg_color, color, mouth);


  fragColor = vec4(color, 1.0);
}