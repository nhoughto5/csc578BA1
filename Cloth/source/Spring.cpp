#include "Spring.hpp"
#include <iostream>
Spring::Spring(Particle *p1, Particle *p2) : 
	p1(p1), 
	p2(p2), 
	ks(500.0f),
	kd(50.0f)
{
	restLength = length(p1->getCurrentPosition(), p2->getCurrentPosition());
}

Spring::~Spring() {

}
GLfloat Spring::length(glm::vec3 a, glm::vec3 b) {
	glm::vec3 diff = a - b;
	return glm::sqrt((diff.x * diff.x) + (diff.y * diff.y) +(diff.z * diff.z));
}
//Normalize a vector with a safeguard for division by zero
glm::vec3 getNormalized(glm::vec3 V) {
	GLfloat length = glm::length(V);
	if (length < 0.0000001) return glm::vec3{ 0.0f, 0.0f, 0.0f };
	else return glm::normalize(V);
}
void Spring::update() {
	//========== Euler =========//
	//GLfloat vel1 = glm::length(p1->getVelocity());
	//GLfloat vel2 = glm::length(p2->getVelocity());
	//glm::vec3 pos1 = p1->getCurrentPosition();
	//glm::vec3 pos2 = p2->getCurrentPosition();
	//glm::vec3 p1_p2 = (pos2 - pos1) / getLength();
	//glm::vec3 f1 = ((ks*(getLength() - restLength)) - (kd*(vel2-vel1) * p1_p2)) * p1_p2;
	//p1->addForce(f1);
	//p2->addForce(-f1);



	//============ Verlet ==========//
	glm::vec3 p1_to_p2 = p2->getCurrentPosition() - p1->getCurrentPosition(); // vector from p1 to p2
	float current_distance = glm::length(p1_to_p2); // current distance between p1 and p2
	glm::vec3 correctionVector = p1_to_p2*(1 - restLength / current_distance); // The offset vector that could moves p1 into a distance of rest_distance to p2
	glm::vec3 correctionVectorHalf = correctionVector*0.5f; // Lets make it half that length, so that we can move BOTH p1 and p2.
	p1->offSetPosition(correctionVectorHalf); // correctionVectorHalf is pointing from p1 to p2, so the length should move p1 half the length needed to satisfy the constraint.
	p2->offSetPosition(-correctionVectorHalf); // we must move p2 the negative direction of correctionVectorHalf since it points from p2 to p1, and not p1 to p2.	

}
GLfloat Spring::getLength() {
	return length(p2->getCurrentPosition(), p1->getCurrentPosition());
}