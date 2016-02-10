#ifndef PARTICLE_HPP
#define PARTICLE_HPP
#pragma once
#include <atlas/utils/Geometry.hpp>
class Particle {
public:
	Particle(glm::vec3 pos, GLuint index_, glm::vec3 colour_);
	~Particle();

	void addForce(glm::vec3 force);
	void updateGeometry(atlas::utils::Time const& t, GLfloat DAMPING);
	void makeMoveable();
	void makeStationary();
	void resetAcceleration();
	void setPosition(glm::vec3 newPos);
	void offSetPosition(glm::vec3 newPos);
	void addToSumOfSpringForces(glm::vec3 force);
	void setWind(glm::vec3 wind_);
	void resetNormal();
	void addToNormal(glm::vec3 V);
	glm::vec3 getCurrentPosition();

	GLuint index;
private:	
	glm::vec3 currentPosition, colour, previousPosition, acceleration, totalSpringForces, gravity, wind, velocity, mNormal;
	GLfloat mass, dragCoefficient;
	bool stationary;
};
#endif // !PARTICLE_HPP
