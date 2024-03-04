/**
 *
 * @brief Custom quad source file.
 * @author Ivan Sancho, ESAT 2022-23
 * @subject Graphic Programming
 *
 */

#include "geometry_custom_quad.h"
#include "ESAT/math.h"
#include "EDK3/dev/gpumanager.h"


namespace EDK3 {

    QuadCustom::QuadCustom() {}
    QuadCustom::~QuadCustom() {}
    QuadCustom& QuadCustom::operator=(const QuadCustom& other)
    {
        if(this != &other){}
        return *this;
    }

//IMPORTANT!!!
//TODO constructors, destructor and =operator here!!!

void QuadCustom::init(const float quad_size) {
  //TODO demand graphic resources to the GPUManager.
    EDK3::dev::GPUManager::Instance()->newBuffer(&elements_buffer);
    EDK3::dev::GPUManager::Instance()->newBuffer(&order_buffer);

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

  //****** Positions:
  //TODO
    ESAT::Vec3 pos[] = { {-0.5, 0.5, 0.0},
                         {0.5, 0.5, 0.0},
                         {-0.5, -0.5, 0.0},
                          {0.5, 0.5, 0.0} };


  //****** Normals:
  //TODO
    ESAT::Vec3 norm[] = { {0.0, 0.0, 1.0},
                          {0.0, 0.0, 1.0},
                          {0.0, 0.0, 1.0},
                          {0.0, 0.0, 1.0} };

  //****** UVs:
  //TODO
    ESAT::Vec2 uv[] = { {0.0, 1.0},
                        {1.0, 1.0},
                        {0.0, 0.0},
                        {1.0, 0.0} };
  //****** Colors:
  //TODO


  //****** Upload data:
  //TODO initialize and upload data to the "elements_buffer".
  //Size in bytes: 12 data per element * 1 faces * 4 vertices per face * sizeof(float)
  //48 attributes has a face.
    constexpr int kNData = 8 * 4;
    float data[kNData];
    int j = 0;
    for (int i = 0; i < 4; ++i)
    {
        data[j++] = pos[i].x;
        data[j++] = pos[i].y;
        data[j++] = pos[i].z;

        data[j++] = norm[i].x;
        data[j++] = norm[i].y;
        data[j++] = norm[i].z;

        data[j++] = uv[i].x;
        data[j++] = uv[i].y;
    }
    elements_buffer->init(sizeof(data));
    elements_buffer->uploadData(data, sizeof(data), 0);

  //****** Upload order:
  //TODO initialize and upload data to the "order_buffer".
  //Remember to order triangles in counter clockwise direction!!
  //6 vertices per face * 1 faces * sizeof(short int)
    short int order_index[] = {0, 3, 1, /**/3, 0, 2};
    order_buffer->init(sizeof(order_index));
    order_buffer->uploadData(order_index, sizeof(data), 0);
}

bool QuadCustom::bindAttribute(const Attribute a,
                               unsigned int where_to_bind_attribute) const {
  //TODO
  //EDK3::dev::GPUManager::Instance()->enableVertexAttribute
    switch (a)
    {
    case Attribute::A_POSITION:
        EDK3::dev::GPUManager::Instance()->enableVertexAttribute(
                elements_buffer.get(), 0, EDK3::T_FLOAT_3, false, 0, sizeof(float) * 8);
        break;
    case Attribute::A_NORMAL:
        EDK3::dev::GPUManager::Instance()->enableVertexAttribute(
            elements_buffer.get(), 1, EDK3::T_FLOAT_3, false, sizeof(float) * 3, sizeof(float) * 8);
        break;
    case Attribute::A_UV:
        EDK3::dev::GPUManager::Instance()->enableVertexAttribute(
            elements_buffer.get(), 2, EDK3::T_FLOAT_2, false, sizeof(float) * 6, sizeof(float) * 8);
        break;
    default:
        return false;
        break;
    }
    return true;
}

void QuadCustom::render() const {
  //TODO
    EDK3::dev::GPUManager::Instance()->drawElements(EDK3::dev::GPUManager::DrawMode::kDrawMode_Triangles,
        6, order_buffer.get(), EDK3::Type::T_UINT, 0);
}

} //EDK3
