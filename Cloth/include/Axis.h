#ifndef AXIS_H
#define AXIS_H
#pragma once
#include <atlas/utils/Geometry.hpp>
USING_ATLAS_MATH_NS;
USING_ATLAS_GL_NS;
class Axis : public atlas::utils::Geometry {
public:
	Axis();
	~Axis();
	void renderGeometry(atlas::math::Matrix4 projection,atlas::math::Matrix4 view) override;
private:
	GLfloat axisLength;
	glm::vec3 xAxisColor, yAxisColor, zAxisColor;
	GLuint axisVertexBufferID, axisIndexBuffer, numIndices, vertexArrayObject;
};
#endif // AXIS_H


