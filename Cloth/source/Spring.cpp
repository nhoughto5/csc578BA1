#include "Spring.hpp"

Spring::Spring(Particle *p1, Particle *p2) : p1(p1), p2(p2), k(0.5f){
	restVector = p2->getCurrentPosition() - p1->getCurrentPosition();
}

Spring::~Spring() {

}

void Spring::calculateForces() {
	glm::vec3 stretchLength = p2->getCurrentPosition() - p1->getCurrentPosition();
	glm::vec3 displacementFromRestVector = stretchLength - restVector;

	//Multiply the displacements by the spring constant to get 
	//A vector which represents the force on each spring
	p1->addToSumOfSpringForces(k * displacementFromRestVector);
	p2->addToSumOfSpringForces(-k * displacementFromRestVector);
}