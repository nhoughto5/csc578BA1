#include "Triangle.hpp"

Triangle::Triangle()
{
	normal = glm::vec3();
}

Triangle::~Triangle()
{
}

void Triangle::setTriangle(Particle *p1, Particle *p2, Particle *p3)
{
	particles[0] = p1;
	particles[1] = p2;
	particles[2] = p3;
}

void Triangle::calculateNormal()
{
	glm::vec3 v1 = particles[1]->getCurrentPosition() - particles[2]->getCurrentPosition();
	glm::vec3 v2 = particles[2]->getCurrentPosition() - particles[0]->getCurrentPosition();

	normal = glm::cross(v1,v2);
}