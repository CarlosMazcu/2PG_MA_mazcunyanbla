#version 330

uniform vec4 u_color;
uniform float u_time;
#define M_PI 3.14159265358979323846 
//In this demo, the window has the following size:
// 1200 x 700
uniform vec2 u_window_size;
out vec4 fragColor;

void main() {
  float alpha=(gl_FragCoord.x + gl_FragCoord.y) / (u_window_size.x + u_window_size.y);

  float y=clamp(sin(6 * M_PI * 2 * alpha), 0.0, 1.0);
  
  fragColor = vec4(y, y,0.0,1.0);

}