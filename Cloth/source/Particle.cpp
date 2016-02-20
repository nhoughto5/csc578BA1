#include "Particle.hpp"
#include <iostream>
Particle::Particle(glm::vec3 pos, GLushort index_, glm::vec3 colour_, int posX_, int posY_) :
	currentPosition(pos),
	previousPosition(pos),
	stationary(false),
	gravity{ 0.0f, -9.81f, 0.0f },
	wind{ 0.0f, 0.0f, 0.0f },
	velocity{ 0.0f, 0.0f, 0.0f },
	mNormal{ 0.0f, 0.0f, 0.0f },
	index(index_),
	colour(colour_),
	Damping(1.0f),
	totalForces{0.0f, 0.0f, 0.0f},
	posX(posX),
	posY(posY)
{

}

Particle::~Particle() {

}
glm::vec3 Particle::verletIntegration(atlas::utils::Time const& t) {
	glm::vec3 nextPos;
	nextPos = (2.0f * currentPosition) - previousPosition + totalForces * t.deltaTime;
	return nextPos;
}
void Particle::updateGeometry(atlas::utils::Time const& t) {

	velocity.x = totalForces.x * Damping;
	velocity.y = totalForces.y * Damping;
	velocity.z = totalForces.z * Damping;
	glm::vec3 temp = currentPosition;
	glm::vec3 next = verletIntegration(t);
	if (!stationary) {
		currentPosition = next;
	}
	previousPosition = temp;
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
	currentPosition += newPos;
}
glm::vec3 Particle::getCurrentPosition() {
	return currentPosition;
}
void Particle::addForce(glm::vec3 force) {
	totalForces += force;
}
void Particle::clearForces() {
	totalForces = glm::vec3{ 0.0f, 0.0f, 0.0f };
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
	mNormal += V;
}
int Particle::getPosX() {
	return posX;
}
int Particle::getPosY(){
	return posY;
}