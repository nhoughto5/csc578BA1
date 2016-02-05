#ifndef SHAPEGENERATOR_H
#define SHAPEGENERATOR_H
#pragma once
#include <atlas/utils/Geometry.hpp>
#include "ShapeData.h"
#include "Vertex.h"
class ObjectGenerator {
public:
	static ShapeData makeSpringMass(glm::vec3 anchorPosition, GLfloat springWidth, GLfloat d, GLfloat width, GLfloat height);
	static ShapeData makeAngularSpringMass(glm::vec3 anchorPosition, GLfloat springWidth, GLfloat d, GLfloat width, GLfloat height, GLfloat radianAngle);
private:
	static GLfloat getRotatedX(GLfloat angle, Vertex old, glm::vec3 anchorPosition);
	static GLfloat getRotatedY(GLfloat angle, Vertex old, glm::vec3 anchorPosition);
};
#endif // !SHAPEGENERATOR_H