/**
 *
 * @brief Custom quad source file.
 * @author Ivan Sancho, ESAT 2022-23
 * @subject Graphic Programming
 *
 */

#include "geometry_custom_cube.h"
#include "ESAT/math.h"
#include "EDK3/dev/gpumanager.h"


namespace EDK3 {



    CubeCustom::CubeCustom() { is_initialized_ = false; }
    CubeCustom::~CubeCustom() {}
    CubeCustom& CubeCustom::operator=(const CubeCustom& other)
    {
        if(this != &other){}
        return *this;
    }

//IMPORTANT!!!
//TODO constructors, destructor and =operator here!!!
void CubeCustom::init8v(const float cube_size) {
    if (is_initialized_) {
        return;
    }
    is_initialized_ = true;
        EDK3::dev::GPUManager::Instance()->newBuffer(&elements_buffer);
        EDK3::dev::GPUManager::Instance()->newBuffer(&order_buffer);

        //TODO demand graphic resources to the GPUManager.

        /**
         The quad face is like this:
               ------------
               |\        |
               | \       |
               |  \      |
               |   \     |
               |    \    |
               |     \   |
               |      \  |
               |       \ |
               |        \|
               ------------
          That's the order of its face.
         */
        float size = cube_size * 0.5f;

        ESAT::Vec3 positions[]{
            {-size, size, size}, //0
            {size, size, size}, //1
            
            {size, -size, size}, //2
            {-size, -size, size}, //3

            {-size, size, -size}, //4
            {size, size, -size}, //5
            
            {size, -size, -size}, //6
            {-size, -size, -size}, //7

        };
        //TODO

		ESAT::Vec3 normals[]{
			{-0.577, 0.577,  0.577}, // 0
	        {0.577, 0.577,  0.577}, // 1 

	        {-0.577, 0.577,  0.577}, // 3
	        {-0.577, -0.577,  0.577}, // 2

	        {-0.577, 0.577,  -0.577}, // 4 
	        {0.577, 0.577,  -0.577}, // 5

	        {0.577, -0.577,  -0.577}, // 7
	        {-0.577, -0.577,  -0.577}, // 6 

		};
              

        //TODO


        ESAT::Vec2 UVs[]{
            {0.0, 1.0}, //0
            {1.0, 1.0}, //1
            {1.0, 0.0}, //3
            {0.0, 0.0}, //2

            {0.0, 0.0}, //4
            {1.0, 0.0}, //5
            {0.0, 1.0},  //7
            {1.0, 1.0} //6
        };


        //****** Upload data:
        //TODO initialize and upload data to the "elements_buffer".
        //Size in bytes: 12 data per element * 1 faces * 4 vertices per face * sizeof(float)
        //48 attributes has a face.
        constexpr int KNData = 8 * 4 * 2;
        float data[KNData];
        int j = 0;
        for (int i = 0; i < 8; i++) {
            data[j++] = positions[i].x;
            data[j++] = positions[i].y;
            data[j++] = positions[i].z;

            data[j++] = normals[i].x;
            data[j++] = normals[i].y;
            data[j++] = normals[i].z;

            data[j++] = UVs[i].x;
            data[j++] = UVs[i].y;
        }
        elements_buffer->init(sizeof(data));
        elements_buffer->uploadData(data, sizeof(data), 0);

        //****** Upload order:
        //TODO initialize and upload data to the "order_buffer".
        //Remember to order triangles in counter clockwise direction!!
        //6 vertices per face * 1 faces * sizeof(short int)
        unsigned int order[] = {
            0, 2, 1, /**/ 0, 3, 2,
            1, 6, 5, /**/ 1, 2, 6,
            5, 7, 4, /**/ 5, 6, 7,
            3, 6, 2, /**/ 3, 7, 6, 
            4, 3, 0, /**/ 4, 7, 3,
            4, 1, 5, /**/ 4, 0, 1
        };
        order_buffer->init(sizeof(order));
        order_buffer->uploadData(order, sizeof(order), 0);
        is_initialized_ = true;

    }

void CubeCustom::init24v(const float cube_size) {}


bool CubeCustom::bindAttribute(const Attribute a,
                               unsigned int where_to_bind_attribute) const {
  //TODO
  //EDK3::dev::GPUManager::Instance()->enableVertexAttribute
    switch (a)
    {
    case Attribute::A_POSITION:
        EDK3::dev::GPUManager::Instance()->enableVertexAttribute(
                elements_buffer.get(), where_to_bind_attribute, EDK3::T_FLOAT_3, false, 0, sizeof(float) * 8);
        break;
    case Attribute::A_NORMAL:
        EDK3::dev::GPUManager::Instance()->enableVertexAttribute(
            elements_buffer.get(), where_to_bind_attribute, EDK3::T_FLOAT_3, false, sizeof(float) * 3, sizeof(float) * 8);
        break;
    case Attribute::A_UV:
        EDK3::dev::GPUManager::Instance()->enableVertexAttribute(
            elements_buffer.get(), where_to_bind_attribute, EDK3::T_FLOAT_2, false, sizeof(float) * 6, sizeof(float) * 8);
        break;
    default:
        return false;
        break;
    }
    return true;
}

void CubeCustom::render() const {
  //TODO
    EDK3::dev::GPUManager::Instance()->drawElements(EDK3::dev::GPUManager::DrawMode::kDrawMode_Triangles,
        36, order_buffer.get(), EDK3::Type::T_UINT, 0);
}

} //EDK3
