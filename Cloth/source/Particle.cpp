#include "Particle.hpp"
#include <iostream>
Particle::Particle(glm::vec3 pos, GLushort index_, glm::vec3 colour_) :
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
	mass(0.1f),
	Damping(0.8f)
{

}

Particle::~Particle() {

}

void Particle::addForce(glm::vec3 force) {
	acceleration += force / mass;
}
void limit(glm::vec3 & v, GLfloat maxLength) {
	GLfloat lengthSquared = v.x * v.x + v.y * v.y + v.z * v.z;
	
	if ((lengthSquared > maxLength * maxLength) && (lengthSquared > 0)) {
		GLfloat ratio = maxLength / glm::sqrt(lengthSquared);
		v.x *= ratio;
		v.y *= ratio;
		v.z *= ratio;
		
	}
}
void Particle::updateGeometry(atlas::utils::Time const& t) {
	if (!stationary) {
		previousPosition = currentPosition;
		glm::vec3 forceOfGravity = mass * gravity;
		glm::vec3 forceOfAirResistance = -dragCoefficient * velocity * velocity;
		glm::vec3 totalForce = forceOfGravity + mass*totalSpringForces - velocity*Damping;
		acceleration = totalForce / mass;

		//Perform Euler Integration
		currentPosition += t.deltaTime * velocity;
		velocity += t.deltaTime * acceleration;
		//limit(velocity, 0.50f);
		//velocity *= Damping;
		//============== End Euler==============//
		if (index == 3) {
			std::cout << "totalSpringForces: " << totalSpringForces.x << ", " << totalSpringForces.y << ", " << totalSpringForces.z << "   Velocity: {" << velocity.x << ", " << velocity.y << ", " << velocity.z << "\n";
			std::cout << "new Position " << currentPosition.x << ", " << currentPosition.y << ", " << currentPosition.z << "\n";
		}
		//Reset the forces acting on the particle from all of the springs
		//So that a new accumulated total can be calculated.
		totalSpringForces = glm::vec3{ 0.0f, 0.0f, 0.0f };
		acceleration = glm::vec3{ 0.0f, 0.0f, 0.0f };

	}
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
	if(index == 3)	std::cout << "  Force Added on 3: " << force.x << ", " << force.y << ", " << force.z << "\n";
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