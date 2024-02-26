#version 330
#define M_PI 3.14159265358979323846 
uniform vec4 u_color;
uniform float u_time;
//In this demo, the window has the following size:
// 1200 x 700
uniform vec2 u_window_size;
out vec4 fragColor;

void main() {
  float num_x = 24.0;
  float num_y = 14.0;

  float alpha_x = gl_FragCoord.x / u_window_size.x;
  float x = clamp(((round(sin(alpha_x * (M_PI * 2) * num_x) * 0.5 + 0.5) - 1.0) * (-1.0)),0.0,1.0);

  float alpha_y = gl_FragCoord.y / u_window_size.y;
  float y = clamp(((round(sin(alpha_y * (M_PI * 2) * num_y) * 0.5 + 0.5) - 1.0) * (-1.0)),0.0,1.0);

  float c = x * y;

  fragColor = vec4(c ,c ,c , 1.0);
}