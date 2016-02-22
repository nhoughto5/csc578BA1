#include "Particle.hpp"
#include <iostream>
Particle::Particle(glm::vec3 pos, GLushort index_, glm::vec3 colour_, int posX_, int posY_) :
	currentPosition(pos),
	previousPosition(pos),
	stationary(false),
	gravity{ 0.0f, -0.00002f, 0.0f },
	wind{ 0.0f, 0.0f, 0.0f },
	velocity{ 0.0f, 0.0f, 0.0f },
	mNormal{ 0.0f, 0.0f, 0.0f },
	index(index_),
	colour(colour_),
	Damping(0.51f),
	mass(1.0f),
	acceleration{0.0f, 0.0f, 0.0f},
	posX(posX),
	posY(posY)
{

}

Particle::~Particle() {

}
void Particle::verletIntegration(atlas::utils::Time const& t) {
	if (!stationary) {
		glm::vec3 temp = currentPosition;
		currentPosition = currentPosition + (currentPosition - previousPosition) * (1.0f - Damping) + (acceleration * t.deltaTime);
		previousPosition = temp;
	}
}
//Limit a vector's length to a maximum value
glm::vec3 vectorClamp(glm::vec3 V, GLfloat maxLength) {
	GLfloat lengthSquared = V.x * V.x + V.y * V.y + V.z * V.z;
	if ((lengthSquared > maxLength * maxLength) && (lengthSquared > 0.0f)) {
		GLfloat ratio = maxLength / glm::sqrt(lengthSquared);
		V *= ratio;
	}
	return V;
}
void Particle::eulerIntegration(atlas::utils::Time const& t) {
	if (!stationary) {
		currentPosition = currentPosition + (velocity * t.deltaTime);
		velocity = velocity + (acceleration * t.deltaTime);
		velocity = vectorClamp(velocity, 0.5f);
	}
}
void Particle::updateGeometry(atlas::utils::Time const& t) {
	verletIntegration(t);
	//eulerIntegration(t);
	//if (index == 55) {
	//	std::cout << "Total Force: " << acceleration.x << ", " << acceleration.y << ", " << acceleration.z << "    ";
	//	std::cout << "Velocity: " << velocity.x << ", " << velocity.y << ", " << velocity.z << "\n";
	//}
} 
glm::vec3 Particle::getColour() {
	return colour;
}
void Particle::makeMoveable() {
	stationary = false;
}
GLushort Particle::getIndex() {
	return index;
}
void Particle::makeStationary() {
	stationary = true;
}
void Particle::setPosition(glm::vec3 newPos) {
	currentPosition = newPos;
}
void Particle::offSetPosition(glm::vec3 newPos) {
	if(!stationary)	currentPosition += newPos;
}
glm::vec3 Particle::getCurrentPosition() {
	return currentPosition;
}
void Particle::addForce(glm::vec3 force) {
	acceleration += force / mass;
	//if (index == 55) {
	//	std::cout << "Add: " << force.x << ", " << force.y << ", " << force.z << "    Total Acceleration: ";
	//	std::cout << acceleration.x << ", " << acceleration.y << ", " << acceleration.z << "\n";
	//}
}
void Particle::clearForces() {
	acceleration = glm::vec3{ 0.0f, 0.0f, 0.0f };
	//if (index == 55) {
	//	std::cout << acceleration.x << ", " << acceleration.y << ", " << acceleration.z << "\n";
	//}
}
glm::vec3 Particle::getVelocity() {
	return velocity;
}
void Particle::setWind(glm::vec3 wind_) {
	wind = wind_;
}
void Particle::resetNormal() {
	mNormal = glm::vec3{ 0.0f, 0.0f, 0.0f };
}
void Particle::addToNormal(glm::vec3 V) {
	mNormal += glm::normalize(V);
}
int Particle::getPosX() {
	return posX;
}
int Particle::getPosY(){
	return posY;
}