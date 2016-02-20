#ifndef TRIANGLE_H
#define TRIANGLE_H
#include <stdio.h>
#include "Particle.hpp"

class Triangle
{
public:
	Particle *particles[3];
	glm::vec3 normal;

	Triangle();
	virtual ~Triangle();

	void setTriangle(Particle *p1, Particle *p2, Particle *p3);
	void calculateNormal();
protected:
private:
};

#endif // TRIANGLE_H