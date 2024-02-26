#version 330

layout(location = 0) out vec4 fragColor;

uniform vec3 u_camera_position;

in vec3 normal;
in vec3 pos;

vec3 DirectionalLight(){
  vec3 lightColor = vec3(0.0, 1.0, 0.0);
  vec3 lightDir = vec3(1.0, 0.5, 0.6);

  vec3 specularColor = vec3(1.0, 1.0, 1.0);

  float d = max(dot(lightDir, normal), 0.0);

  vec3 diffuse_color = lightColor * d;
  
  vec3 view_dir = normalize(u_camera_position - pos);
  vec3 reflect_dir = reflect(-view_dir, normal);
  float s = pow(max(dot(view_dir, reflect_dir), 0.0), 32.0);
  vec3 spec_color = specularColor * s;

  return diffuse_color + spec_color;
}

vec3 PointLight(){
  vec3 lightColor = vec3(1.0, 0.0, 0.0);
  vec3 lightPos = vec3(-1.0, 0.0, 0.0);
  vec3 lightDir = normalize(pos - lightPos);

  vec3 specularColor = vec3(1.0, 1.0, 1.0);

  float d = max(dot(lightDir, normal), 0.0);
  vec3 diffuse_color = lightColor * d;
  
  vec3 view_dir = normalize(u_camera_position - pos);
  vec3 reflect_dir = reflect(-view_dir, normal);
  float s = pow(max(dot(view_dir, reflect_dir), 0.0), 32.0);
  vec3 spec_color = specularColor * s;

  return diffuse_color + spec_color;
}

vec3 AmbientLight(){
  vec3 ambient_color = vec3(1.0, 1.0, 1.0);
  float ambient_power = 0.2;
  return ambient_color * ambient_power;
}


void main() {

  vec3 color = DirectionalLight();
  color += PointLight();
  color += AmbientLight();

  fragColor = vec4(color, 1.0);
}
