#version 330

layout(location = 0) out vec4 fragColor;

uniform vec3 u_camera_position;

in vec3 normal;
in vec3 pos;

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
LightColor pointLight;

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
  vec3 diffuse = light.light_color * d * attenuation;


  vec3 view_dir = normalize(u_camera_position - pos);
  vec3 reflect_dir = reflect(-light.light_dir, normal);
  float s = pow(max(dot(view_dir, reflect_dir), 0.0), 32.0);
  vec3 spec_color = light.specular_color * s * attenuation;

  return diffuse + spec_color;
}


vec3 AmbientLight(LightColor light){
  return light.light_color * light.shininess;
}

void main() {

  pointLight.light_color = vec3(1.0, 1.0, 1.0);
  pointLight.specular_color = vec3(1.0, 1.0, 1.0);
  pointLight.light_pos = vec3(0.0, 200.0, 0.0);
  pointLight.light_dir = normalize(pointLight.light_pos - pos);
  pointLight.constant = 1.0;
  pointLight.linear = 0.0007;
  pointLight.quadratic = 0.00005;
  vec3 color = PointLight(pointLight);

  fragColor = vec4(color, 1.0);
}
