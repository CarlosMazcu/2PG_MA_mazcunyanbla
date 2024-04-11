/**
 *
 * @brief Custom quad source file.
 * @author Ivan Sancho, ESAT 2022-23
 * @subject Graphic Programming
 *
 */

#include "geometry_custom_terrain.h"
#include "ESAT/math.h"
#include "EDK3/dev/gpumanager.h"
#include "EDK3/scoped_array.h"
#include "Perlin/SimplexNoise.h"

namespace EDK3 {

	TerrainCustom::TerrainCustom() {}
	TerrainCustom::~TerrainCustom() {}

	TerrainCustom& TerrainCustom::operator=(const TerrainCustom& other)
	{
		if (this != &other) {}
		return *this;
	}
	void TerrainCustom::init(const int num_cols, const int num_rows,
		const float height_mult, const float size,
		const bool is_centered)
	{
		EDK3::dev::GPUManager::Instance()->newBuffer(&elements_buffer);
		EDK3::dev::GPUManager::Instance()->newBuffer(&order_buffer);

		float offset_x = 0.0f;
		float offset_z = 0.0f;
		if (is_centered)
		{
			offset_x = -(num_cols * size) / 2;
			offset_z = -(num_rows * size) / 2;
		}

		int vertices = (num_cols + 1) * (num_rows + 1);
		
		EDK3::scoped_array<float> points;
		points.alloc(vertices * 8);

		int index = 0;

		for (int z = 0; z <= num_rows; z++)
		{
			for (int x = 0; x <= num_cols; x++)
			{
				//positions
				float pos_x = x * size + offset_x;
				float pos_y = SimplexNoise::noise(x * size + offset_x * height_mult, z * size + offset_z * height_mult);
				float pos_z = z * size + offset_z;
				points[index++] = pos_x;
				points[index++] = pos_y;  //0.0f * size;
				points[index++] = pos_z;

				//normal
				float length = sqrt(pos_x * pos_x + pos_y * pos_y + pos_z * pos_z);
				points[index++] = pos_x / length;
				points[index++] = pos_y / length;
				points[index++] = pos_z / length;

				//UV
				points[index++] = (float)x / (float)num_cols;
				points[index++] = (float)z / (float)num_rows;
			}
		}

		elements_buffer->init(points.sizeInBytes());
		elements_buffer->uploadData(points.get(), points.sizeInBytes(), 0);

		int total_triangles = num_cols * num_rows * 2;
		EDK3::scoped_array<int> order;

		order.alloc(total_triangles * 3);

		index = 0;

		for(int z = 0; z < num_rows; z++)
		{
			for (int x = 0; x < num_cols; x++)
			{

				int top_left = z * (num_cols + 1) + x;          // Esquina superior izquierda
				int top_right = top_left + 1;                   // Esquina superior derecha
				int bottom_left = (z + 1) * (num_cols + 1) + x; // Esquina inferior izquierda
				int bottom_right = bottom_left + 1;             // Esquina inferior derecha

				//positions
				order[index++] = top_left;
				order[index++] = bottom_left;
				order[index++] = bottom_right;

				order[index++] = top_left;
				order[index++] = bottom_right;
				order[index++] = top_right;

			}
		}

		order_buffer->init(order.sizeInBytes());
		order_buffer->uploadData(order.get(), order.sizeInBytes(), 0);

	}

	

	bool TerrainCustom::bindAttribute(const Attribute a,
		unsigned int where_to_bind_attribute) const
	{
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

	void TerrainCustom::render() const
	{
		EDK3::dev::GPUManager::Instance()->drawElements(EDK3::dev::GPUManager::DrawMode::kDrawMode_Triangles, order_buffer->size(), order_buffer.get(),
			EDK3::Type::T_UINT, 0);
	}


} //EDK3
