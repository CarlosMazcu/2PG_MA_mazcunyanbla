#include "dev/custom_gpu_shader.h"

namespace EDK3 {

namespace dev {

CustomGPUShader::CustomGPUShader() {
	is_compiled_ = false;
	type_ = EDK3::dev::Shader::Type::kType_Invalid;
	internal_id_ = 0;

}

void loadSource(const Type shader_type, const char* source, const unsigned int source_size) {
	//glShaderSource
	//glCreateChader

}


bool compile(EDK3::scoped_array<char>* output_log = NULL) {
	//glCompileShader


}
bool is_compiled() const { return is_compiled_;}
const EDK3::dev::Shader::Type type() const { return type_; }
unsigned int internal_id() const {

	return internal_id_;
}

CustomGPUShader::~CustomGPUShader(){
// glDeleteShader

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