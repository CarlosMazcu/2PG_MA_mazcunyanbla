#include "geometry_custom_sphere.h"
#include "EDK3/dev/gpumanager.h"
#include "EDK3/scoped_array.h"
#include <cmath>

namespace EDK3 {

    SphereCustom::SphereCustom() : is_initialized_(false), num_heights_(20), num_revs_(20) {}

    SphereCustom::~SphereCustom() {}

    void SphereCustom::init(const float sphere_size, const int num_heights, const int num_revs)
    {
        is_initialized_ = true;
        num_heights_ = num_heights;
        num_revs_ = num_revs;

        EDK3::dev::GPUManager::Instance()->newBuffer(&elements_buffer);
        EDK3::dev::GPUManager::Instance()->newBuffer(&order_buffer);

        int num_vertices = (num_heights + 1) * (num_revs + 1);
        EDK3::scoped_array<float> points;
        points.alloc(num_vertices * 8);

        int index = 0;

        for (int i = 0; i <= num_heights; i++)
        {

            for (int j = 0; j <= num_revs; j++)
            {
                float alpha = (float)i * 3.14f / num_heights;
                float sin_alpha = sin(alpha);
                float cos_alpha = cos(alpha);

                float beta = (float)j * 6.28f / num_revs;
                float sin_beta = sin(beta);
                float cos_beta = cos(beta);

                float x = cos_beta * sin_alpha;
                float y = cos_alpha;
                float z = sin_beta * sin_alpha;
                //position
                points[index++] = x * sphere_size;
                points[index++] = y * sphere_size;
                points[index++] = z * sphere_size;

                float length = sqrt(((x * sphere_size) * (x * sphere_size)) + ((y * sphere_size) * (y * sphere_size)) + ((z * sphere_size) * (z * sphere_size)));
                //normal
                points[index++] = (x * sphere_size)/length;
                points[index++] = (y * sphere_size)/length;
                points[index++] = (z * sphere_size) /length;
                //UV
                points[index++] = (float)j / num_revs;
                points[index++] = (float)i / num_heights;
            }
        }

        elements_buffer->init(points.sizeInBytes());
        elements_buffer->uploadData(points.get(), points.sizeInBytes(), 0);

        int num_triangles = num_heights * num_revs * 2;
        EDK3::scoped_array<int> order;
        order.alloc(num_triangles * 3);

        index = 0;

        for (int i = 0; i < num_heights; i++)
        {
            for (int j = 0; j < num_revs; j++)
            {
                index = (i * num_revs + j) * 6;
                int first = i * (num_revs + 1) + j;
                int second = first + num_revs + 1;

                order[index] = first;
                order[index + 1] = second;
                order[index + 2] = first + 1;

                order[index + 3] = second;
                order[index + 4] = second + 1;
                order[index + 5] = first + 1;
            }
        }

        order_buffer->init(order.sizeInBytes());
        order_buffer->uploadData(order.get(), order.sizeInBytes(), 0);
    }

    bool SphereCustom::bindAttribute(const Attribute a, unsigned int where_to_bind_attribute) const
    {
        if (!is_initialized_)
            return false;

        switch (a)
        {
        case Attribute::A_POSITION:
            EDK3::dev::GPUManager::Instance()->enableVertexAttribute(elements_buffer.get(), 0, EDK3::T_FLOAT_3, false, 0, sizeof(float) * 8);
            break;
        case Attribute::A_NORMAL:
            EDK3::dev::GPUManager::Instance()->enableVertexAttribute(elements_buffer.get(), 1, EDK3::T_FLOAT_3, false, sizeof(float) * 3, sizeof(float) * 8);
            break;
        case Attribute::A_UV:
            EDK3::dev::GPUManager::Instance()->enableVertexAttribute(elements_buffer.get(), 2, EDK3::T_FLOAT_2, false, sizeof(float) * 6, sizeof(float) * 8);
            break;
        default:
            return false;
            break;
        }
        return true;
    }

    void SphereCustom::render() const
    {
        if (!is_initialized_)
            return;

        EDK3::dev::GPUManager::Instance()->drawElements(EDK3::dev::GPUManager::DrawMode::kDrawMode_Triangles, order_buffer->size(), order_buffer.get(), EDK3::Type::T_UINT, 0);
    }

} // EDK3
