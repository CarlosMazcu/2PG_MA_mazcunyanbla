#version 330

uniform vec4 u_color;
uniform float u_time;
# define M_PI 3.14159265358979323846
//In this demo, the window has the following size:
// 1200 x 700
uniform vec2 u_window_size;
out vec4 fragColor;

void main() {
  fragColor = vec4(sin((M_PI * 2) * 10 * ((gl_FragCoord.x + 30) / u_window_size.x)), 
                  cos((M_PI * 2) * 12 * ((gl_FragCoord.y) / u_window_size.y)), 
                  0.0, 1.0);
}