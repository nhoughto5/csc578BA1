#include "Spring.hpp"
#include <iostream>
Spring::Spring(Particle *p1, Particle *p2) : 
	p1(p1), 
	p2(p2), 
	ks(20.0f),
	kd(0.2f)
{
	restLength = length(p1->getCurrentPosition() - p2->getCurrentPosition());
}

Spring::~Spring() {

}
GLfloat length(glm::vec3 a, glm::vec3 b) {
	glm::vec3 diff = a - b;
	return glm::sqrt((diff.x * diff.x) + (diff.y * diff.y) +(diff.z * diff.z));
}
void Spring::update() {
	glm::vec3 springVector = normalize((p2->getCurrentPosition() - p1->getCurrentPosition()));

	glm::vec3 vel1 = p1->getVelocity();
	glm::vec3 vel2 = p2->getVelocity();
	GLfloat v1 = springVector.x * vel1.x + springVector.y * vel1.y + springVector.z * vel1.z;
	GLfloat v2 = springVector.x * vel2.x + springVector.y * vel2.y + springVector.z * vel2.z;

	GLfloat forces = -ks * (restLength - this->getLength()) - kd * (v1 - v2);

	glm::vec3 springForce = forces * springVector;

	p1->addForce(springForce);
	p2->addForce(-springForce);
}
GLfloat Spring::getLength() {
	return length(p2->getCurrentPosition(), p1->getCurrentPosition());
}