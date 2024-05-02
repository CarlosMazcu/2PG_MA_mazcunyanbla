#include "dev/custom_gpu_manager.h"
#include "dev/custom_gpu_shader.h"
#include "dev/custom_gpu_program.h"
#include "dev/custom_gpu_buffer.h"
#include "EDK3/dev/glew.h"



namespace EDK3 {

namespace dev {
	CustomGPUBuffer::CustomGPUBuffer()
	{
		internal_id_ = 0;
		size_ = 0;
		target_ = EDK3::dev::Buffer::Target::kTarget_Vertex_Data;
	}

	void CustomGPUBuffer::init(unsigned int size)
	{
		glGenBuffers(1, &internal_id_);
		glBindBuffer(GL_ARRAY_BUFFER, internal_id_);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
		size_ = size;
	}

	void CustomGPUBuffer::bind(const Target t) const
	{
		CustomGPUBuffer* p = const_cast<CustomGPUBuffer*>(this);
		p->target_ = t;
		switch (t)
		{
		case kTarget_Vertex_Data:
			glBindBuffer(GL_ARRAY_BUFFER, internal_id_);
			break;

		case kTarget_Elements:
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, internal_id_);
			break;
		default:
			break;
		}
	}

	unsigned int CustomGPUBuffer::size() const
	{
		return size_;
	}

	void CustomGPUBuffer::uploadData(const void* data, unsigned int size,
									unsigned int offset)
	{
		switch (target_)
		{
		case kTarget_Vertex_Data:
			glBindBuffer(GL_ARRAY_BUFFER, internal_id_);
			glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
			break;

		case kTarget_Elements:
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, internal_id_);
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);

			break;
		default:
			break;
		}
		
	}

	void CustomGPUBuffer::release()
	{
		glDeleteBuffers(1, &internal_id_);
	}

	CustomGPUBuffer::~CustomGPUBuffer(){}

	CustomGPUBuffer::CustomGPUBuffer(const CustomGPUBuffer& other)
	{
		size_ = other.size_;
		internal_id_ = other.internal_id_;
		target_ = other.target_;
	}

	CustomGPUBuffer& CustomGPUBuffer::operator=(const CustomGPUBuffer& other)
	{
		size_ = other.size_;
		internal_id_ = other.internal_id_;
		target_ = other.target_;

		return *this;
	}







}//dev

}//EDK3
