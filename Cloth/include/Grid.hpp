#ifndef GRID_HPP
#define GRID_HPP
#pragma once
#include <atlas/utils/Geometry.hpp>

class Grid : public atlas::utils::Geometry{
public:
	Grid();
	~Grid();

	void renderGeometry(atlas::math::Matrix4 projection, atlas::math::Matrix4 view) override;

private:
	GLuint mBuffer;
	GLuint mVao;
	int mNumVertex;
};

#endif // !GRID_HPP
