#version 330
#define M_PI 3.14159265358979323846 
uniform vec4 u_color;
uniform float u_time;
//In this demo, the window has the following size:
// 1200 x 700
uniform vec2 u_window_size;
out vec4 fragColor;

void main() {
  vec2 center = u_window_size * 0.5;
  float distance_x = abs(gl_FragCoord.x - center.x);
  float distance_y = abs(gl_FragCoord.y - center.y);

  float y = float(distance_x == distance_y);


  fragColor = vec4(y, 
                  y, 
                  0.0, 1.0);
}