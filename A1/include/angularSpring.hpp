#ifndef ANGULAR_HPP
#define ANGULAR_HPP

#pragma once
#include <atlas/utils/Geometry.hpp>
#include "ObjectGenerator.h"
class angularSpring : public atlas::utils::Geometry {
public:
	angularSpring();
	~angularSpring();
	void renderGeometry(atlas::math::Matrix4 projection, atlas::math::Matrix4 view) override;
	void updateGeometry(atlas::utils::Time const& t) override;
private:
	GLuint mVertexArrayObject, mVertexBufferObject, springIndexBufferID;
	GLfloat stretch, springWidth;
	glm::vec3 anchorPosition, displacement, connectionPoint;
	GLfloat massWidth = 0.2f, massHeight = 0.2f, angleDegrees, radianAngle;
	ShapeData mSpringMass;
	GLfloat mMass, mForce, mVelocity, k, springForce, angularVelocity, centrifugalForce, radius, period;

};
#endif