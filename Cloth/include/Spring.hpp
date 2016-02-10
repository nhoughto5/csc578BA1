#ifndef SPRING_HPP
#define SPRING_HPP
#pragma once
#include <atlas/utils/Geometry.hpp>
#include "Particle.hpp"

class Spring {
public:
	Spring(Particle *p1, Particle *p2);
	~Spring();
	void calculateForces();
private:
	GLfloat k;
	Particle *p1, *p2;
	glm::vec3 restVector;
};
#endif // !SPRING_HPP
