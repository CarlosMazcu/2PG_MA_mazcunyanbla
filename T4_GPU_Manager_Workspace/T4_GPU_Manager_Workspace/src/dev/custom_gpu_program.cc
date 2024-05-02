#include "dev/custom_gpu_manager.h"
#include "dev/custom_gpu_shader.h"
#include "dev/custom_gpu_program.h"
#include "dev/custom_gpu_buffer.h"
#include "EDK3/dev/glew.h"


namespace EDK3 {

namespace dev {
	CustomGPUProgram::CustomGPUProgram()
	{
		internal_id_ = glCreateProgram();
	}
	CustomGPUProgram::CustomGPUProgram(const CustomGPUProgram& other)
	{
		internal_id_ = other.internal_id_;
	}
	CustomGPUProgram& CustomGPUProgram::operator=(const CustomGPUProgram& other)
	{
		internal_id_ = other.internal_id_;
		return *this;
	}
	CustomGPUProgram::~CustomGPUProgram()
	{
		//glDeleteProgram(internal_id_);
	}

	void CustomGPUProgram::attach(EDK3::dev::Shader* shader)
	{
		glAttachShader(internal_id_, shader->internal_id());
	}

	bool CustomGPUProgram::link(EDK3::scoped_array<char>* output_log)
	{

		glLinkProgram(internal_id_);
		GLint params;
		glGetProgramiv(internal_id_, GL_LINK_STATUS, &params);

		if (params != GL_TRUE)
		{
			if (output_log) {
				GLint length = 0;
				glGetProgramiv(internal_id_, GL_INFO_LOG_LENGTH, &length);
				output_log->alloc(length + 1);
				glGetProgramInfoLog(internal_id_, length, nullptr, output_log->get());
				output_log->get()[length] = '\0';
			}
			return false;

		}
		if (output_log) {
			output_log->alloc(1);
			output_log->get()[0] = '\0';
		}

		return true;
	}
	void CustomGPUProgram::use() const
	{
		glUseProgram(internal_id_);
	}

	int CustomGPUProgram::get_attrib_location(const char* name) const 
	{
		return glGetAttribLocation(internal_id_, name);
	}

	int CustomGPUProgram::get_uniform_position(const char* name) const 
	{
		return glGetUniformLocation(internal_id_, name);
	}

	void CustomGPUProgram::set_uniform_value(const int uniform_pos,
											const EDK3::Type uniform_type,
											const float* number) const {
		switch (uniform_type)
		{
		case T_FLOAT_1: 
			glUniform1fv(uniform_pos, 1, number);
			break;
		case T_FLOAT_2: 
			glUniform2fv(uniform_pos, 1, number);
			break;
		case T_FLOAT_3: 
			glUniform3fv(uniform_pos, 1, number);
			break;
		case T_FLOAT_4:
			glUniform4fv(uniform_pos, 1, number);
			break;
		case T_DOUBLE_1:
			glUniform1fv(uniform_pos, 1, number);
			break;
		case T_DOUBLE_2:
			glUniform2fv(uniform_pos, 1, number);
			break;
		case T_DOUBLE_3:
			glUniform3fv(uniform_pos, 1, number);
			break;
		case T_DOUBLE_4:
			glUniform4fv(uniform_pos, 1, number);
			break;
		case T_MAT_2x2:
			glUniformMatrix2fv(uniform_pos, 1, false, number);
			break;
		case T_MAT_3x3:
			glUniformMatrix3fv(uniform_pos, 1, false, number);
			break;
		case T_MAT_4x4:
			glUniformMatrix4fv(uniform_pos, 1, false, number);
			break;
		default:
			break;
		}
	// glUniform
	}

	void CustomGPUProgram::set_uniform_value(const int uniform_pos,
											const EDK3::Type uniform_type,
											const int* number) const {

		switch (uniform_type)
		{
		case T_INT_1:
			glUniform1iv(uniform_pos, 1, number);
			break;
		case T_INT_2:
			glUniform2iv(uniform_pos, 1, number);
			break;
		case T_INT_3:
			glUniform3iv(uniform_pos, 1, number);
			break;
		case T_INT_4:
			glUniform4iv(uniform_pos, 1, number);
			break;
		case T_BYTE_1:
			glUniform1iv(uniform_pos, 1, number);
			break;
		case T_BYTE_2:
			glUniform2iv(uniform_pos, 1, number);
			break;
		case T_BYTE_3:
			glUniform3iv(uniform_pos, 1, number);
			break;
		case T_BYTE_4:
			glUniform4iv(uniform_pos, 1, number);
			break;
		case T_SHORT_1:
			glUniform1iv(uniform_pos, 1, number);
			break;
		case T_SHORT_2:
			glUniform2iv(uniform_pos, 1, number);
			break;
		case T_SHORT_3:
			glUniform3iv(uniform_pos, 1, number);
			break;
		case T_SHORT_4:
			glUniform4iv(uniform_pos, 1, number);
			break;
		default:
			break;
		}
	// glUniform
	}

	void CustomGPUProgram::set_uniform_value(const int uniform_pos,
											const EDK3::Type uniform_type,
											const unsigned int* number) const {
		switch (uniform_type)
		{
		case T_UINT_1:
			glUniform1uiv(uniform_pos, 1, number);
			break;
		case T_UINT_2:
			glUniform2uiv(uniform_pos, 1, number);
			break;
		case T_UINT_3:
			glUniform3uiv(uniform_pos, 1, number);
			break;
		case T_UINT_4:
			glUniform4uiv(uniform_pos, 1, number);
			break;
		case T_UBYTE_1:
			glUniform1uiv(uniform_pos, 1, number);
			break;
		case T_UBYTE_2:
			glUniform2uiv(uniform_pos, 1, number);
			break;
		case T_UBYTE_3:
			glUniform3uiv(uniform_pos, 1, number);
			break;
		case T_UBYTE_4:
			glUniform4uiv(uniform_pos, 1, number);
			break;
		case T_USHORT_1:
			glUniform1uiv(uniform_pos, 1, number);
			break;
		case T_USHORT_2:
			glUniform2uiv(uniform_pos, 1, number);
			break;
		case T_USHORT_3:
			glUniform3uiv(uniform_pos, 1, number);
			break;
		case T_USHORT_4:
			glUniform4uiv(uniform_pos, 1, number);
			break;
		default:
			break;
		}
	// glUniform
	}

	GLuint CustomGPUProgram::internal_id() const
	{
		return internal_id_;
	}



}//dev

}//EDK3
