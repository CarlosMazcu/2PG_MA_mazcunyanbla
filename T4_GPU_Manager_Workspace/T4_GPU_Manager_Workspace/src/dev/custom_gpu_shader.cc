#include "dev/custom_gpu_manager.h"
#include "dev/custom_gpu_shader.h"
#include "dev/custom_gpu_program.h"
#include "dev/custom_gpu_buffer.h"
#include "EDK3/dev/glew.h"

namespace EDK3 {

namespace dev {

CustomGPUShader::CustomGPUShader() {
	is_compiled_ = false;
	type_ = EDK3::dev::Shader::Type::kType_Invalid;
	internal_id_ = 0;
}

void CustomGPUShader::loadSource(const Type shader_type, const char* source, const unsigned int source_size) {
	
	
	type_ = shader_type;
	switch (shader_type)
	{
	case kType_Vertex:
		internal_id_ = glCreateShader(GL_VERTEX_SHADER);
		break;
	case kType_Fragment:
		internal_id_ = glCreateShader(GL_FRAGMENT_SHADER);

		break;
	case kType_Geometry:
		internal_id_ = glCreateShader(GL_GEOMETRY_SHADER);
		break;
	default:
		break;
	}
	GLint size = source_size;
	glShaderSource(internal_id_, 1, &source, &size);
}


bool CustomGPUShader::compile(EDK3::scoped_array<char>* output_log) {
	glCompileShader(internal_id_);
	GLint params;
	glGetShaderiv(internal_id_, GL_COMPILE_STATUS, &params);
	
	if (params != GL_TRUE)
	{
		if (output_log) {
			GLint length = 0;
			glGetShaderiv(internal_id_, GL_INFO_LOG_LENGTH, &length);
			output_log->alloc(length + 1);
			glGetShaderInfoLog(internal_id_, length, nullptr, output_log->get());
			output_log->get()[length] = '\0';
		}
		is_compiled_ = false;
		
	}
	else {
		if (output_log) {
			output_log->alloc(1);
			output_log->get()[0] = '\0';
		}
		is_compiled_ = true;
	}
	//glGetShaderInfoLog
	return is_compiled_;

}
bool CustomGPUShader::is_compiled() const { 
	return is_compiled_;
}

const EDK3::dev::Shader::Type CustomGPUShader::type() const {
	return type_; 
}

GLuint CustomGPUShader::internal_id() const {
	return internal_id_;
}

CustomGPUShader::~CustomGPUShader(){
	glDeleteShader(internal_id_);
}
CustomGPUShader::CustomGPUShader(const CustomGPUShader& other){
	is_compiled_ = other.is_compiled_;
	type_ = other.type_;
	internal_id_ = other.internal_id_;


}

CustomGPUShader& CustomGPUShader::operator=(const CustomGPUShader& other) {

	is_compiled_ = other.is_compiled_;
	type_ = other.type_;
	internal_id_ = other.internal_id_;

	return *this;
}




}//dev
}//EDK3