#include "Particle.hpp"

Particle::Particle(glm::vec3 pos, GLuint index_, glm::vec3 colour_) :
	currentPosition(pos),
	previousPosition(pos),
	acceleration{ 0.0f, 0.0f, 0.0f },
	stationary(false),
	gravity{ 0.0f, -9.81f, 0.0f },
	wind{ 0.0f, 0.0f, 0.0f },
	velocity{ 0.0f, 0.0f, 0.0f },
	dragCoefficient(0.2f),
	mNormal{ 0.0f, 0.0f, 0.0f },
	index(index_),
	colour(colour_),
	mass(0.5f)
{

}

Particle::~Particle() {

}

void Particle::addForce(glm::vec3 force) {
	acceleration += force / mass;
}

//Verlet Integration
void Particle::updateGeometry(atlas::utils::Time const& t, GLfloat DAMPING) {
	if (!stationary) {
		previousPosition = currentPosition;
		glm::vec3 forceOfGravity = mass * gravity;
		glm::vec3 forceOfAirResistance = -dragCoefficient * velocity * velocity;
		glm::vec3 totalForce = forceOfGravity + forceOfAirResistance + wind + totalSpringForces;
		acceleration = totalForce / mass;

		//Perform Euler Integration
		velocity += t.deltaTime * acceleration;
		currentPosition += t.deltaTime * velocity;
		//============== End Euler==============//

		//Reset the forces acting on the particle from all of the springs
		//So that a new accumulated total can be calculated.
		totalSpringForces = glm::vec3{ 0.0f, 0.0f, 0.0f };
	}
}
void Particle::makeMoveable() {
	stationary = false;
}
void Particle::makeStationary() {
	stationary = true;
}
void Particle::resetAcceleration() {
	acceleration = glm::vec3{ 0.0f, 0.0f ,0.0f };
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

void Particle::addToSumOfSpringForces(glm::vec3 force) {
	totalSpringForces += force;
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