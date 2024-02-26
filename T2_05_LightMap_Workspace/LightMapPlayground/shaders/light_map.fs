#version 330
layout(location = 0) out vec4 fragColor;
uniform vec3 u_camera_position;
in vec2 uv;
in vec3 normal;
in vec3 pos;


//Textures:
uniform sampler2D u_albedo_texture;
uniform sampler2D u_specular_texture;

struct LightColor {
  vec3 light_color;
  vec3 light_dir;
  vec3 light_pos;
  vec3 specular_color;
  vec3 spot_direction;
  float shininess;
  float spec_strength;
  float constant;
  float linear;
  float quadratic;
  float cutoff;
  float phi;
  int type;
};

LightColor spotLight[6];
LightColor ambient;

vec3 PointLight(LightColor light){
  float distance = length(light.light_pos - pos);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);


  float d = max(dot(light.light_dir, normal), 0.0);
  vec3 diffuse_color = light.light_color * d * attenuation;
  
  vec3 view_dir = normalize(u_camera_position - pos);
  vec3 reflect_dir = reflect(-light.light_dir, normal);
  float s = pow(max(dot(view_dir, reflect_dir), 0.0), 32.0);
  light.specular_color = light.specular_color * s * attenuation;

  return diffuse_color + light.specular_color;
}

vec3 DirectionalLight(LightColor light){
  float d = max(dot(light.light_dir, normal), 0.0);

  vec3 diffuse_color = light.light_color * d;
  
  vec3 view_dir = normalize(u_camera_position - pos);
  vec3 reflect_dir = reflect(-view_dir, normal);
  float s = pow(max(dot(view_dir, reflect_dir), 0.0), 32.0);
  light.specular_color = light.specular_color * s;

  return diffuse_color + light.specular_color;
}

vec3 SpotLight(LightColor light) {
  float distance    = length(light.light_pos - pos);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 

  float spot_angle = dot(-light.light_dir, light.spot_direction);

  float spot_factor = smoothstep(light.cutoff, 1.0, spot_angle);

  attenuation = spot_factor;

  float d = max(dot(light.light_dir, normal), 0.0);
  vec3 diffuse = light.light_color * d * attenuation * texture(u_albedo_texture, uv).xyz;


  vec3 view_dir = normalize(u_camera_position - pos);
  vec3 reflect_dir = reflect(-light.light_dir, normal);
  float s = pow(max(dot(view_dir, reflect_dir), 0.0), 32.0);
  vec3 spec_color = light.specular_color * s * attenuation * texture(u_specular_texture, uv).xyz;;

  return diffuse + spec_color;
}


vec3 AmbientLight(LightColor light){
  return light.light_color * light.shininess;
}

void main() {
  spotLight[0].light_color = vec3(1.0, 1.0, 1.0);
  spotLight[0].light_pos = vec3(2.0, 0.0, 0.0);
  spotLight[0].light_dir = normalize(spotLight[0].light_pos - pos);
  spotLight[0].spot_direction = normalize(vec3(-1.0, 0.0, 0.0));
  spotLight[0].specular_color = vec3(1.0, 1.0, 1.0);
  spotLight[0].constant = 1.0;
  spotLight[0].linear = 0.7;
  spotLight[0].quadratic = 1.0;

  spotLight[1].light_color = vec3(1.0, 1.0, 1.0);
  spotLight[1].light_pos = vec3(0.0, 2.0, 0.0);
  spotLight[1].light_dir = normalize(spotLight[1].light_pos - pos);
  spotLight[1].spot_direction = normalize(vec3(0.0, -1.0, 0.0));
  spotLight[1].specular_color = vec3(1.0, 1.0, 1.0);
  spotLight[1].constant = 1.0;
  spotLight[1].linear = 0.7;
  spotLight[1].quadratic = 1.0;

  spotLight[2].light_color = vec3(1.0, 1.0, 1.0);
  spotLight[2].light_pos = vec3(0.0, 0.0, 2.0);
  spotLight[2].light_dir = normalize(spotLight[2].light_pos - pos);
  spotLight[2].spot_direction = normalize(vec3(0.0, 0.0, -1.0));
  spotLight[2].specular_color = vec3(1.0, 1.0, 1.0);
  spotLight[2].constant = 1.0;
  spotLight[2].linear = 0.7;
  spotLight[2].quadratic = 1.0;

  spotLight[3].light_color = vec3(1.0, 1.0, 1.0);
  spotLight[3].light_pos = vec3(-2.0, 0.0, 0.0);
  spotLight[3].light_dir = normalize(spotLight[3].light_pos - pos);
  spotLight[3].spot_direction = normalize(vec3(1.0, 0.0, 0.0));
  spotLight[3].specular_color = vec3(1.0, 1.0, 1.0);
  spotLight[3].constant = 1.0;
  spotLight[3].linear = 0.7;
  spotLight[3].quadratic = 1.0;

  spotLight[4].light_color = vec3(1.0, 1.0, 1.0);
  spotLight[4].light_pos = vec3(0.0, -2.0, 0.0);
  spotLight[4].light_dir = normalize(spotLight[4].light_pos - pos);
  spotLight[4].spot_direction = normalize(vec3(0.0, 1.0, 0.0));
  spotLight[4].specular_color = vec3(1.0, 1.0, 1.0);
  spotLight[4].constant = 1.0;
  spotLight[4].linear = 0.7;
  spotLight[4].quadratic = 1.0;

  spotLight[5].light_color = vec3(1.0, 1.0, 1.0);
  spotLight[5].light_pos = vec3(0.0, 0.0, -2.0);
  spotLight[5].light_dir = normalize(spotLight[5].light_pos - pos);
  spotLight[5].spot_direction = normalize(vec3(0.0, 0.0, 1.0));
  spotLight[5].specular_color = vec3(1.0, 1.0, 1.0);
  spotLight[5].constant = 1.0;
  spotLight[5].linear = 0.7;
  spotLight[5].quadratic = 1.0;



  ambient.light_color = vec3(0.3, 0.0, 0.0);
  ambient.shininess = 0.2;

  vec3 color = SpotLight(spotLight[0]) + SpotLight(spotLight[1]) + 
               SpotLight(spotLight[2]) + SpotLight(spotLight[3]) + 
               SpotLight(spotLight[4]) + SpotLight(spotLight[5]) + AmbientLight(ambient);

  fragColor = vec4(color, 1.0);
}
