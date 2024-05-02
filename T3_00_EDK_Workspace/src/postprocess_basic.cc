/**
 *
 * @brief Basic postprocess material source file.
 * @author Ivan Sancho, ESAT 2022-23
 * @subject Graphic Programming
 *
 */


#include "postprocess_basic.h"
#include <stdio.h>
#include "ESAT/time.h"
#include "EDK3/dev/gpumanager.h"
#include "ESAT/math.h"


namespace EDK3 {
  
static const char kVertexShader[] =
"#version 330\n"

"layout(location = 0) in vec3 a_position;\n"
"layout(location = 1) in vec3 a_normal;\n"
"layout(location = 2) in vec2 a_uv;\n"

"uniform mat4 u_model;\n"
"uniform mat4 u_vp_matrix;\n"
"out vec2 uv;\n"

"void main(){\n"
"  uv = a_uv;\n"
"  gl_Position = u_vp_matrix * u_model * vec4(a_position, 1.0);\n"
"}\n";

#define GLSL(x) "#version 330\n"#x
static const char* kFragmentShader = GLSL(
 
    uniform sampler2D u_texture;
    uniform vec4 u_color; 
    out vec4 fragColor; 
    in vec2 uv;
    void main() {
    fragColor = u_color * texture(u_texture, uv);
    }
);

PostprocessBasic::PostprocessBasic() { }
PostprocessBasic::~PostprocessBasic() { }
PostprocessBasic::PostprocessBasic(const PostprocessBasic& other) {
  program_ = other.program_;
}
PostprocessBasic::PostprocessBasic(PostprocessBasic&& other) {
  program_ = other.program_;
  other.program_.release();
}
PostprocessBasic& PostprocessBasic::operator=(const PostprocessBasic& other) {
  program_ = other.program_;
  return *this;
}

void PostprocessBasic::init() {
    EDK3::scoped_array<char> Log_Error;
    EDK3::ref_ptr<EDK3::dev::Shader> ShaderV;
    EDK3::ref_ptr<EDK3::dev::Shader> ShaderF;

    EDK3::dev::GPUManager& GPU = *EDK3::dev::GPUManager::Instance();
    GPU.newShader(&ShaderV);
    GPU.newShader(&ShaderF);
    GPU.newProgram(&program_);

    ShaderV->loadSource(EDK3::dev::Shader::Type::kType_Vertex, kVertexShader, strlen(kVertexShader));
    ShaderF->loadSource(EDK3::dev::Shader::Type::kType_Fragment, kFragmentShader, strlen(kFragmentShader));


    if (!ShaderV->compile(&Log_Error)) {
        printf("Algo ha salido mal (Vertex) %s \n", Log_Error.get());
    }
    else {

        program_->attach(ShaderV.get());
    }

    if (!ShaderF->compile(&Log_Error)) {
        printf("Algo ha salido mal (Frangment) %s \n", Log_Error.get());
    }
    else {

        program_->attach(ShaderF.get());
    }

    if (!program_->link(&Log_Error)) {
        printf("Algo ha salido mal (Linkado) %s \n", Log_Error.get());
    }

}

bool PostprocessBasic::enable(const EDK3::MaterialSettings *mat) const {
    program_->use();
    const PostprocessBasicSettings* ms = dynamic_cast<const PostprocessBasicSettings*>(mat);
    if (ms) {
        int loc = program_->get_uniform_position("u_texture");
        int slot = 0;
        ms->texture()->bind(slot);
        program_->set_uniform_value(loc, EDK3::Type::T_INT, &slot);

        loc = program_->get_uniform_position("u_color");
        program_->set_uniform_value(loc, EDK3::Type::T_FLOAT_4, ms->color());

        return true;
    }
   
    return false;
}

void PostprocessBasic::setupCamera(const float projection[16], const float view[16]) const {
    //Set the projection and view matrices as uniforms here!
    ESAT::Mat4 proj = ESAT::Mat4FromColumns(projection);
    ESAT::Mat4 vw = ESAT::Mat4FromColumns(view);

    ESAT::Mat4 result = ESAT::Mat4Identity();
    result = ESAT::Mat4Multiply(proj, vw);

    program_->set_uniform_value(program_->get_uniform_position("u_vp_matrix"), T_MAT_4x4, result.d);
}

void PostprocessBasic::setupModel(const float model[16]) const {
    ESAT::Mat4 md = ESAT::Mat4FromColumns(model);
    program_->set_uniform_value(program_->get_uniform_position("u_model"), T_MAT_4x4, md.d);
}

unsigned int PostprocessBasic::num_attributes_required() const {
  return 3;
}

EDK3::Attribute PostprocessBasic::attribute_at_index(const unsigned int attrib_idx) const {
    switch (attrib_idx)
    {
    case 0:
        return A_POSITION;
    case 1:
        return A_NORMAL;

    case 2:
        return A_UV;

    }
}

EDK3::Type PostprocessBasic::attribute_type_at_index(const unsigned int attrib_idx) const {
    switch (attrib_idx)
    {
    case 0:
        return T_FLOAT_3;
    case 1:
        return T_FLOAT_3;

    case 2:
        return T_FLOAT_2;

    }
}

} //EDK3
