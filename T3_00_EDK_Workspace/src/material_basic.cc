/**
 *
 * @brief Basic material source file.
 * @author Ivan Sancho, ESAT 2022-23
 * @subject Graphic Programming
 *
 */


#include "material_basic.h"
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

    static const char kFragmentShader[] =
        "#version 330\n"
        "uniform sampler2D u_albedo_1;\n"
        "uniform sampler2D u_albedo_2;\n"
        "uniform vec4 u_color;\n"
        "uniform int u_use_texture;\n"
        "in vec2 uv;\n"
        "out vec4 fragColor;\n"


        "void main() {\n"
        "if(0 == u_use_texture){"
        "   fragColor = u_color;}\n"
        "else{"
        "   fragColor = u_color * texture(u_albedo_1, uv);}\n"

        "}\n";

    //IMPORTANT!!!
    //Type the constructors, destructor and operator= here, just in this point!!!
    //If you don't do it, it won't compile!!!


    MaterialBasic::MaterialBasic() {}
    MaterialBasic::~MaterialBasic() {}
    MaterialBasic& MaterialBasic::operator=(const MaterialBasic& other) {
        if (this != &other) {}
        return *this;
    }

    void MaterialBasic::init() {
        //1: Solicite al menos dos sombreadores y un programa al Administrador de GPU.
        //2: carga el código fuente en los sombreadores solicitados.
        //3: compila ambos sombreadores.
        //4: Adjunte sombreadores al programa.
        //5: Finalmente... ¡vincula el programa!
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

    bool MaterialBasic::enable(const EDK3::MaterialSettings* mat) const {

        const MaterialBasicSettings* ms = dynamic_cast<const MaterialBasicSettings*>(mat);
        if (ms) {
            MaterialBasicSettings* non_const_ms = const_cast<MaterialBasicSettings*>(ms);
            int use_texture = 0;
            program_->use();
            unsigned int use_texture_loc = program_->get_uniform_position("u_use_texture");
            program_->set_uniform_value(use_texture_loc, EDK3::Type::T_INT_1, &use_texture);

            float color[] = { 1.0f, 1.0f,0.0f, 1.0f };
            non_const_ms->set_color(color);
            unsigned int color_loc = program_->get_uniform_position("u_color");
            program_->set_uniform_value(color_loc, Type::T_FLOAT_4, non_const_ms->color());
            return true;
        }
        else {
            const MaterialBasicTexturedSettings* ms2 = dynamic_cast<const MaterialBasicTexturedSettings*>(mat);
            if (ms2) {
                program_->use();
                int use_texture = 1;
                unsigned int use_texture_loc = program_->get_uniform_position("u_use_texture");
                program_->set_uniform_value(use_texture_loc, EDK3::Type::T_INT_1, &use_texture);
                unsigned int albedo_loc = 0;
                int slot = 0;
                if (ms2->texture(0).get())
                {
                    ms2->texture(0)->bind(slot);
                    albedo_loc = program_->get_uniform_position("u_albedo_1");
                    program_->set_uniform_value(albedo_loc, EDK3::Type::T_INT_1, &slot);
                    slot++;
                }
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

    void MaterialBasic::setupCamera(const float projection[16],
        const float view[16]) const {
        //Set the projection and view matrices as uniforms here!
        ESAT::Mat4 proj = ESAT::Mat4FromColumns(projection);
        ESAT::Mat4 vw = ESAT::Mat4FromColumns(view);

        ESAT::Mat4 result = ESAT::Mat4Identity();
        result = ESAT::Mat4Multiply(proj, vw);

        program_->set_uniform_value(program_->get_uniform_position("u_vp_matrix"), T_MAT_4x4, result.d);

    }

    void MaterialBasic::setupModel(const float model[16]) const {
        ESAT::Mat4 md = ESAT::Mat4FromColumns(model);
        program_->set_uniform_value(program_->get_uniform_position("u_model"), T_MAT_4x4, md.d);
    }

    unsigned int MaterialBasic::num_attributes_required() const {
        return 3;
    }

    EDK3::Attribute MaterialBasic::attribute_at_index(const unsigned int attrib_idx) const {
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

    EDK3::Type MaterialBasic::attribute_type_at_index(const unsigned int attrib_idx) const {
        //Search on "EDK3::Type" enum.
        //Depending on how attributes are stored in the geometry buffer.
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