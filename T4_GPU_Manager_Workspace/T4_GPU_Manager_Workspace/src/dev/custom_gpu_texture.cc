#include "dev/custom_gpu_texture.h"
#include "EDK3/dev/glew.h"

namespace EDK3 
{
    CustomGPUTexture::CustomGPUTexture()
    {
        id_ = 0;
    }

    void CustomGPUTexture::init(Type t, Format internal_format, unsigned int width,
                                unsigned int height, unsigned int depth)
    {
        Texture::init(t, internal_format, width, height, depth);
    }

    void CustomGPUTexture::set_data(const Format f,
                                    const EDK3::Type t,
                                    const void* data,
                                    unsigned int mipmap_LOD)
    {
        //glGenTextures
        //glBindTexture
        //glTexImage
        //glPixelStorei
        //glPixelStorei
    }

    void CustomGPUTexture::bind(unsigned int textUnit) const 
    {
        //glActiveTexture
        //glBindTexture
    }
    unsigned int CustomGPUTexture::internal_id() const 
    {
        return id_;
    }

    void CustomGPUTexture::set_min_filter(Filter f) 
    {
        Texture::set_min_filter(f);
        switch (f)
        {
        case EDK3::Texture::F_NEAREST: 
            glTexParameteri(...);
            break;
        case EDK3::Texture::F_LINEAR:
            glTexParameteri(...);

            break;
        case EDK3::Texture::F_NEAREST_MIPMAP_NEAREST:
            glTexParameteri(...);

            break;
        case EDK3::Texture::F_LINEAR_MIPMAP_NEAREST:
            glTexParameteri(...);

            break;
        case EDK3::Texture::F_NEAREST_MIPMAP_LINEAR:
            glTexParameteri(...);

            break;
        case EDK3::Texture::F_LINEAR_MIPMAP_LINEAR:
            glTexParameteri(...);

            break;
        default:
            break;
        }
    }
    void CustomGPUTexture::set_mag_filter(Filter f) 
    {
        Texture::set_mag_filter(f);
        switch (f)
        {
        case EDK3::Texture::F_NEAREST:
            glTexParameteri(...);
            break;
        case EDK3::Texture::F_LINEAR:
            glTexParameteri(...);
            break;
 
        default:
            break;
        }
    }
    void CustomGPUTexture::set_wrap_s(Wrap c)
    {
        Texture::set_wrap_s(c);
        switch (c)
        {
        case EDK3::Texture::W_REPEAT:
            glTexParameteri(...);
            break;
        case EDK3::Texture::W_MIRRORED_REPEAT:
            glTexParameteri(...);
            break;
        case EDK3::Texture::W_CLAMP_TO_EDGE:
            glTexParameteri(...);
            break;

        default:
            break;
        }
    }
    void CustomGPUTexture::set_wrap_t(Wrap c) 
    {
        Texture::set_wrap_t(c);
        switch (c)
        {
        case EDK3::Texture::W_REPEAT:
            glTexParameteri(...);
            break;
        case EDK3::Texture::W_MIRRORED_REPEAT:
            glTexParameteri(...);
            break;
        case EDK3::Texture::W_CLAMP_TO_EDGE:
            glTexParameteri(...);
            break;

        default:
            break;
        }
    }
    void CustomGPUTexture::set_wrap_r(Wrap c) 
    {
        Texture::set_wrap_r(c);
        switch (c)
        {
        case EDK3::Texture::W_REPEAT:
            glTexParameteri(...);
            break;
        case EDK3::Texture::W_MIRRORED_REPEAT:
            glTexParameteri(...);
            break;
        case EDK3::Texture::W_CLAMP_TO_EDGE:
            glTexParameteri(...);
            break;

        default:
            break;
        }
    }
    void CustomGPUTexture::generateMipmaps() const 
    {
        //glGenMipMap
    }

    CustomGPUTexture::~CustomGPUTexture() 
    {
        //glDeleteTextures
    }

    CustomGPUTexture::CustomGPUTexture(const CustomGPUTexture& other) 
    {
        id_ = other.id_;
    
    }
    CustomGPUTexture& CustomGPUTexture::operator=(const CustomGPUTexture& other) 
    {
        id_ = other.id_;
    }


}//EDK3