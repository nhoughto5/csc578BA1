#include "Spring.hpp"
#include <iostream>
Spring::Spring(Particle *p1, Particle *p2) : 
	p1(p1), 
	p2(p2), 
	k(75.0f)
{
	restLength = length(p1->getCurrentPosition() - p2->getCurrentPosition());
}

Spring::~Spring() {

}
GLfloat length(glm::vec3 a, glm::vec3 b) {
	glm::vec3 diff = a - b;
	return glm::sqrt((diff.x * diff.x) + (diff.y * diff.y) +(diff.z * diff.z));
}
void Spring::calculateForces() {
	glm::vec3 springVector = normalize((p2->getCurrentPosition() - p1->getCurrentPosition()));
	GLfloat stretchLength = length(p2->getCurrentPosition(),  p1->getCurrentPosition());
	GLfloat displacementFromRest = restLength - stretchLength;
	glm::vec3 springForce = -k * displacementFromRest * normalize(springVector);
	//Multiply the displacements by the spring constant to get 
	//A vector which represents the force on each spring


	if (p1->getIndex() == 55 || p2->getIndex() == 55) {
		std::cout << p1->getIndex() << " to " << p2->getIndex();
		//std::cout << "Spring " << p1->getIndex() << " to " << p2->getIndex() << "   p1: " << p1->getCurrentPosition().x  << ", " << p1->getCurrentPosition().y << ", " << p1->getCurrentPosition().z << "   p2: " << p2->getCurrentPosition().x  << ", " << p2->getCurrentPosition().y << ", " << p2->getCurrentPosition().z << "    stretchLength: " << stretchLength << "         restLength:" << restLength<<"\n";
		//if(glm::abs(springVector.y) > 0.0001)	std::cout << p1->getIndex() << " to " << p2->getIndex() << "SpringForce.y: " << springForce.y << "  StretchLength: "<< stretchLength<<"\n";
	}
	p1->addToSumOfSpringForces(springForce);
	p2->addToSumOfSpringForces(-springForce);
}