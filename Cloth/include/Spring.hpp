#ifndef SPRING_HPP
#define SPRING_HPP
#pragma once
#include <atlas/utils/Geometry.hpp>
#include "Particle.hpp"

class Spring {
public:
	Spring(Particle *p1, Particle *p2);
	~Spring();
	void update();
	GLfloat getLength();
private:
	GLfloat ks, kd;
	Particle *p1, *p2;
	GLfloat restLength, currentLength;
	GLfloat length(glm::vec3 a, glm::vec3 b);
};
#endif // !SPRING_HPP
