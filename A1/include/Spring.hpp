#ifndef A1_SPRING_HPP
#define A1_SPRING_HPP
#pragma once
#include <atlas\utils\Geometry.hpp>
#include "ObjectGenerator.h"

USING_ATLAS_MATH_NS;
class Spring : public atlas::utils::Geometry{
public:
	Spring();
	~Spring();
	void renderGeometry(atlas::math::Matrix4 projection, atlas::math::Matrix4 view) override;
	void updateGeometry(atlas::utils::Time const& t) override;

private:
	GLuint mVertexArrayObject, mVertexBufferObject, springIndexBufferID;
	GLfloat stretch, springWidth;
	glm::vec3 anchorPosition, displacement;
	GLfloat massWidth = 0.2f, massHeight = 0.2f;
	ShapeData mSpringMass;

	GLfloat mMass, mForce, springForce, forceOfGravity, gravity, k, mVelocity, damping;
};

#endif
