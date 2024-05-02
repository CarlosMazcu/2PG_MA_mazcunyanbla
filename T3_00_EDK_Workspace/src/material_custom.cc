/**
 *
 * @brief Basic material source file.
 * @author Ivan Sancho, ESAT 2022-23
 * @subject Graphic Programming
 *
 */


#include "material_custom.h"
#include "EDK3/dev/gpumanager.h"

namespace EDK3 {

MaterialCustom::MaterialCustom() {}
MaterialCustom::~MaterialCustom() {}
MaterialCustom::MaterialCustom(const MaterialCustom& mat) {}

MaterialCustom& MaterialCustom::operator=(const MaterialCustom& mat) {
    return *this;
}
//IMPORTANT!!!
//Type the constructors, destructor and operator= here, just in this point!!!
//If you don't do it, it won't compile!!!

void MaterialCustom::init(const char* vertex_shader_path, const char* fragment_shader_path){
  //1: Request at least two shaders and one program to the GPU Manager.
    EDK3::scoped_array<char> error_log;
    EDK3::dev::GPUManager& GPU = *EDK3::dev::GPUManager::Instance();

    EDK3::ref_ptr<EDK3::dev::Shader> fragment_shader;
    GPU.newShader(&fragment_shader);
    EDK3::ref_ptr<EDK3::dev::Shader> vertex_shader;
    GPU.newShader(&vertex_shader);

    GPU.newProgram(&program_);
    

  //2: Load the source code to the requested shaders.
  /*  vertex_vertex->loadSource(EDK3::dev::Shader::Type::kType_Vertex, kVertexShader, strlen(kVertexShader));
    fragment_shader->loadSource(EDK3::dev::Shader::Type::kType_Fragment, kFragmentShader, strlen(kFragmentShader));*/
    if (!EDK3::Material::loadFragmentShaderFile(&fragment_shader, fragment_shader_path))
    {
        printf("Fragment shader error \n");
        exit(99);
    }
    
    if (!EDK3::Material::loadVertexShaderFile(&vertex_shader, vertex_shader_path))
    {
        printf("Vertex shader error \n");
        exit(99);
    }

  //3: Compile both shaders.
    if(!vertex_shader->compile(&error_log)) printf("VERTEX: %s\n", error_log.get());
    if(!fragment_shader->compile(&error_log)) printf("FRAGMENT: %s\n", error_log.get());


  //4: Attach shaders to the program.
    program_->attach(vertex_shader.get());
    program_->attach(fragment_shader.get());
  //5: Finally... link the program!
    program_->link();
}

bool MaterialCustom::enable(const EDK3::MaterialSettings* mat) const {
  //Enable the material...
  //... and use the uniforms located in the material settings!
   program_->use();
   //const LightSettings* light_set = dynamic_cast<const LightSettings*>(mat);
   // if (light_set) {
   //     int counter = 0; 
   //     for (int i = 0; i < 8; ++i) {
   //         if (light_set->lights_[i].enabled_) {
   //             char name[60] = '\0';
   //             //.. Light type
   //             sprintf(name, "u_light[%d].type", counter);
   //             int loc = program_->get_uniform_position(name);
   //             program_->set_uniform_value(loc, EDK3::Type::T_INT_1, light_set->lights_[i].type_);

   //             //.. Light pos
   //             sprintf(name, "u_light[%d].pos", counter);
   //             loc = program_->get_uniform_position(name);
   //             program_->set_uniform_value(loc, EDK3::Type::T_FLOAT_3, light_set->lights_[i].pos_);

   //             //.. Light dir
   //             sprintf(name, "u_light[%d].dir", counter);
   //             loc = program_->get_uniform_position(name);
   //             program_->set_uniform_value(loc, EDK3::Type::T_FLOAT_3, light_set->lights_[i].dir_);

   //             //.. Diff Color
   //             sprintf(name, "u_light[%d].diff_color", counter);
   //             loc = program_->get_uniform_position(name);
   //             program_->set_uniform_value(loc, EDK3::Type::T_FLOAT_3, light_set->lights_[i].diff_color_);

   //             //.. Spec color
   //             sprintf(name, "u_light[%d].spec_color", counter);
   //             loc = program_->get_uniform_position(name);
   //             program_->set_uniform_value(loc, EDK3::Type::T_FLOAT_3, light_set->lights_[i].spec_color_);

   //             //.. linea att 
   //             sprintf(name, "u_light[%d].linear_att", counter);
   //             loc = program_->get_uniform_position(name);
   //             program_->set_uniform_value(loc, EDK3::Type::T_FLOAT, light_set->lights_[i].linear_att_);

   //             //.. quad att
   //             sprintf(name, "u_light[%d].quad_att", counter);
   //             loc = program_->get_uniform_position(name);
   //             program_->set_uniform_value(loc, EDK3::Type::T_FLOAT, light_set->lights_[i].quad_att_);

   //             //.. constant att
   //             sprintf(name, "u_light[%d].constant_att", counter);
   //             loc = program_->get_uniform_position(name);
   //             program_->set_uniform_value(loc, EDK3::Type::T_FLOAT, light_set->lights_[i].constant_att_);

   //             //.. shininess
   //             sprintf(name, "u_light[%d].shininess", counter);
   //             loc = program_->get_uniform_position(name);
   //             program_->set_uniform_value(loc, EDK3::Type::T_FLOAT, light_set->lights_[i].shininess_);

   //             //.. strength
   //             sprintf(name, "u_light[%d].strength", counter);
   //             loc = program_->get_uniform_position(name);
   //             program_->set_uniform_value(loc, EDK3::Type::T_FLOAT, light_set->lights_[i].strength_);
   //             counter++;
   //         }
   //     }

   //     int location;
   //     location = program_->get_uniform_position('u_number_lights');
   //     program_->set_uniform_value(location, EDK3::Type::T_INT, counter);
   //     
   // 
   // }

  const MaterialCustomSettings* ms = dynamic_cast<const MaterialCustomSettings*>(mat);
  int use_texture = 0;
  if (ms) {
      MaterialCustomSettings* non_const_ms = const_cast<MaterialCustomSettings*>(ms);
    unsigned int use_texture_loc = program_->get_uniform_position("u_use_texture");
    program_->set_uniform_value(use_texture_loc, Type::T_INT_1, &use_texture);
    float color[4] = { 1.0f, 1.0f, 0.0f, 1.0f };
    non_const_ms->set_color(color);
    unsigned int color_loc = program_->get_uniform_position("u_color");
    program_->set_uniform_value(color_loc, Type::T_FLOAT_4, non_const_ms->color());
    return true;
  }
  else {
    const MaterialCustomTexturedSettings* ms2 = dynamic_cast<const MaterialCustomTexturedSettings*>(mat);
    if (ms2) {
      //Use material:
      //Color uniform:
      unsigned int color_loc = program_->get_uniform_position("u_color");
      program_->set_uniform_value(color_loc, Type::T_FLOAT_4, ms2->color());
      //Use texture uniform:
      use_texture = 1;
      unsigned int use_texture_loc = program_->get_uniform_position("u_use_texture");
      program_->set_uniform_value(use_texture_loc, Type::T_INT_1, &use_texture);
      unsigned int albedo_loc = 0;
      //First texture:
      int slot = 0;
      if (ms2->texture(0).get()) {
        ms2->texture(0)->bind(slot);
        albedo_loc = program_->get_uniform_position("u_albedo_1");
        program_->set_uniform_value(albedo_loc, EDK3::Type::T_INT_1, &slot);
        slot++;
      }
      //Second texture:
      if (ms2->texture(1).get()) {
        ms2->texture(1)->bind(slot);
        albedo_loc = program_->get_uniform_position("u_albedo_2");
        program_->set_uniform_value(albedo_loc, EDK3::Type::T_INT_1, &slot);
      }
      return true;
    }
  }
  return false;
}

void MaterialCustom::setupCamera(const float projection[16],
                                const float view[16]) const {
  //Set the projection and view matrices as uniforms here!
    ESAT::Mat4 local_projection = ESAT::Mat4FromColumns(projection);
    ESAT::Mat4 local_view = ESAT::Mat4FromColumns(view);
    ESAT::Mat4 m = ESAT::Mat4Multiply(local_projection, local_view);
    program_->set_uniform_value(program_->get_uniform_position("u_vp_matrix"), EDK3::Type::T_MAT_4x4, m.d);

}

void MaterialCustom::setupModel(const float model[16]) const {
  //Set the model matrix as uniform here!
    program_->set_uniform_value(program_->get_uniform_position("u_m_matrix"), EDK3::Type::T_MAT_4x4, model);
}

unsigned int MaterialCustom::num_attributes_required() const {
  //Depending on how attributes the geometry has.
    return 3;
}

EDK3::Attribute MaterialCustom::attribute_at_index(const unsigned int attrib_idx) const {
  //Search on "EDK3::Attribute" enum.
  //Depending on how attributes are stored in the geometry buffer.
  //unsigned int id = program_->get_attrib_location(attrib_idx);
  switch (attrib_idx){
    case 0:
        return EDK3::Attribute::A_POSITION;
        break;
    case 1:
        return EDK3::Attribute::A_NORMAL;
        break;
    case 2:
        return EDK3::Attribute::A_UV;
        break;
    default:
        return EDK3::Attribute::A_NONE;
        break;
  }
}

EDK3::Type MaterialCustom::attribute_type_at_index(const unsigned int attrib_idx) const {
  //Search on "EDK3::Type" enum.
  //Depending on how attributes are stored in the geometry buffer.
    //unsigned int id = program_->get_attrib_location(attrib_idx);
    switch (attrib_idx) {
    case 0:
        return EDK3::Type::T_FLOAT_3;
        break;
    case 1:
        return EDK3::Type::T_FLOAT_3;
        break;
    case 2:
        return EDK3::Type::T_FLOAT_2;
        break;
    default:
        return EDK3::Type::T_NONE;
        break;
    }
}

} //EDK3
