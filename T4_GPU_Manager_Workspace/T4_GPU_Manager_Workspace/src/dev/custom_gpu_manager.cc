#include "dev/custom_gpu_manager.h"
#include "dev/custom_gpu_shader.h"
#include "dev/custom_gpu_program.h"
#include "dev/custom_gpu_buffer.h"
#include "dev/custom_gpu_texture.h"



namespace EDK3 {

namespace dev {

void CustomGPUManager::newShader(EDK3::ref_ptr<EDK3::dev::Shader>* output)
{
	output->allocT<CustomGPUShader>();
}


void CustomGPUManager::newProgram(EDK3::ref_ptr<EDK3::dev::Program> *output)
{
    output->allocT<CustomGPUProgram>();
}
void CustomGPUManager::newBuffer(EDK3::ref_ptr<EDK3::dev::Buffer>* output)
{
    output->allocT<CustomGPUBuffer>();
}

void CustomGPUManager::newTexture1D(const Texture::Format internal_format,
    const unsigned int size,
    EDK3::ref_ptr<Texture>* output) 
   {
   CustomGPUTexture* text = output->allocT<CustomGPUTexture>();
   text->init(EDK3::Texture::Type::T_1D, internal_format, size, 1, 1);

}
void CustomGPUManager::newTexture2D(const Texture::Format internal_format,
    const unsigned int width, const unsigned int height,
    EDK3::ref_ptr<Texture>* output) {

    CustomGPUTexture* text = output->allocT<CustomGPUTexture>();
    text->init(EDK3::Texture::Type::T_1D, internal_format, width, height, 1);
}
void CustomGPUManager::newTexture3D(const Texture::Format internal_format,
    const unsigned int width, const unsigned int height,
    const unsigned int depth,
    EDK3::ref_ptr<Texture>* output) 
{
    CustomGPUTexture* text = output->allocT<CustomGPUTexture>();
    text->init(EDK3::Texture::Type::T_1D, internal_format, width, height, depth);

}
/*
void CustomGPUManager::newBuffer(EDK3::ref_ptr<EDK3::dev::Buffer> *output){
}

 void CustomGPUManager::newTexture1D(const Texture::Format internal_format,
     const unsigned int size,
     EDK3::ref_ptr<Texture>* output) {

 }
 void CustomGPUManager::newTexture2D(const Texture::Format internal_format,
     const unsigned int width, const unsigned int height,
     EDK3::ref_ptr<Texture>* output) {
 }
 void CustomGPUManager::newTexture3D(const Texture::Format internal_format,
     const unsigned int width, const unsigned int height,
     const unsigned int depth,
     EDK3::ref_ptr<Texture>* output) {
 }


void CustomGPUManager::newFramebuffer(EDK3::ref_ptr<Framebuffer>* output) {

}
void CustomGPUManager::clearFrameBuffer(const float rgba[4],
                                const unsigned int FrameBufferOrMask =
                                kFrameBufferColor | kFrameBufferDepth){


                                }
void CustomGPUManager::enableDepthTest(const CompareFunc f){
}
void CustomGPUManager::disableDepthTest(){
}
   */
 

}//dev

}//EDK3
