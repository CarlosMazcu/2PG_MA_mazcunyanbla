#version 330

out vec4 fragColor;


struct Light{
   int type;
   vec3 pos;
   vec3 dir;
   vec3 diff_color;
   vec3 spec_color;
   float linear_att;
   float quad_att;
   float constant_att;
   float shininess;
   float strength;
};

const int kMaxLights = 8;
uniform Light u_light[kMaxLights]; 
uniform int u_number_lights;
uniform vec3 u_camera_pos;
uniform vec3 u_ambient;

vec3  DirectionalLight(...){}
vec3  SpotLight(...){}
vec3  PointLight(...){}

void main() {
    vec3 final_color = vec3(0.0, 0.0, 0.0);
    for(int i = 0; i < kMaxLights; ++i){
        if(i < u_number_lights){
            switch(u_light[i].type){
                case 0: final_color = DirectionalLight(...);
                break;
                case 1: final_color = SpotLight(...);
                break;
                case 2: final_color = PointLight(...);
                break;
            }
        }
    }
    FragColor = vec4(finally_color, 1.0) * texture(u_texture, uv);
}