/**
 *
 * @brief Custom quad source file.
 * @author Ivan Sancho, ESAT 2022-23
 * @subject Graphic Programming
 *
 */

#include "geometry_surface.h"
#include "ESAT/math.h"
#include "EDK3/dev/gpumanager.h"
#include "Perlin/SimplexNoise.h"


namespace EDK3 {

    SurfaceCustom::SurfaceCustom() {}
    SurfaceCustom::~SurfaceCustom() {}
    SurfaceCustom& SurfaceCustom::operator=(const SurfaceCustom& other)
    {
        if (this != &other) {}
        return *this;
    }

    void SurfaceCustom::init(const float* surface_points, const int num_heights, const int num_revs) {

        const int num_vertices = (num_heights+1) * (num_revs+1);
        const int num_triangles = num_vertices * 2;

        EDK3::dev::GPUManager::Instance()->newBuffer(&elements_buffer);
        EDK3::dev::GPUManager::Instance()->newBuffer(&order_buffer);

        EDK3::scoped_array<float> vertices;
        vertices.alloc(num_vertices*8);

        EDK3::scoped_array<int> triangles;
        triangles.alloc(num_triangles * 3);

        int vertex_index = 0;
        float PI = 3.14159265359;

        const float theta = PI / num_heights;
        const float omega = (PI * 2.0f) / num_revs;

        for (int i = 0; i <= num_heights; ++i) {

            float alpha = (2 * PI / num_heights) * i;

            for (int j = 0; j <= num_revs; ++j) {

                   // vertices[vertex_index++] = surface_points[3 * j] * cos(alpha);
                   // vertices[vertex_index++] = surface_points[3 * j + 1]; 
                   // vertices[vertex_index++] = (surface_points[3 * j + 2]) * sin(alpha);

                    vertices[vertex_index++] = surface_points[3 * j] * cos(alpha) + surface_points[3 * j + 2] * sin(alpha);
                    vertices[vertex_index++] = surface_points[3 * j + 1];
                    vertices[vertex_index++] = (-surface_points[3 * j]) * sin(alpha) + surface_points[3 * j + 2] * cos(alpha);

                    // Normal 
                    vertices[vertex_index++] = cosf(omega * j) * cosf(theta * i - 1.57f);;
                    vertices[vertex_index++] = sinf(theta * i - 1.57f);
                    vertices[vertex_index++] = sinf(omega * j) * cosf(theta * i - 1.57f);

                    // UV
                    vertices[vertex_index++] = (float)(j) / (float)num_revs;
                    vertices[vertex_index++] = (float)(i) / (float)num_heights;
            }
        }
        printf("\n%d", vertex_index);

        // Generación de los triángulos
        int triangle_index = 0;
        for (int i = 0; i <= num_heights && i != num_heights; ++i) {
            for (int j = 0; j <= num_revs; ++j) {
                int first = (i * (num_revs + 1)) + j;
                int second = first + 1;
                int third = first + num_revs + 1;
                int fourth = third + 1;

                // Primer triángulo
                triangles[triangle_index++] = first;
                triangles[triangle_index++] = third;
                triangles[triangle_index++] = second;

                // Segundo triángulo
                triangles[triangle_index++] = second;
                triangles[triangle_index++] = third;
                triangles[triangle_index++] = fourth;
            }
        }

        elements_buffer->init(vertices.sizeInBytes());
        elements_buffer->uploadData(vertices.get(), vertices.sizeInBytes(), 0);

        order_buffer->init(triangles.sizeInBytes());
        order_buffer->uploadData(triangles.get(), triangles.sizeInBytes(), 0);
    }


    bool SurfaceCustom::bindAttribute(const Attribute a, unsigned int where_to_bind_attribute) const {
        switch (a)
        {
        case Attribute::A_POSITION:
            EDK3::dev::GPUManager::Instance()->enableVertexAttribute(elements_buffer.get(), where_to_bind_attribute, EDK3::T_FLOAT_3, false, 0, sizeof(float) * 8);
            break;
        case Attribute::A_NORMAL:
            EDK3::dev::GPUManager::Instance()->enableVertexAttribute(elements_buffer.get(), where_to_bind_attribute, EDK3::T_FLOAT_3, false, sizeof(float) * 3, sizeof(float) * 8);
            break;
        case Attribute::A_UV:
            EDK3::dev::GPUManager::Instance()->enableVertexAttribute(elements_buffer.get(), where_to_bind_attribute, EDK3::T_FLOAT_3, false, sizeof(float) * 6, sizeof(float) * 8);
            break;
        default:
            return false;
            break;
        }
        return true;
    }

     
    void SurfaceCustom::render() const {
        EDK3::dev::GPUManager::Instance()->drawElements(EDK3::dev::GPUManager::DrawMode::kDrawMode_Triangles, order_buffer->size(), order_buffer.get(), EDK3::Type::T_UINT, 0);
    }


} //EDK3
