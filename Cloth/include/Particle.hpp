#ifndef PARTICLE_HPP
#define PARTICLE_HPP
#pragma once
#include <atlas/utils/Geometry.hpp>
class Particle {
public:
	Particle(glm::vec3 pos, GLuint index_, glm::vec3 colour_);
	~Particle();

	void updateGeometry(atlas::utils::Time const& t);
	void makeMoveable();
	void makeStationary();
	void setPosition(glm::vec3 newPos);
	void offSetPosition(glm::vec3 newPos);
	void addForce(glm::vec3 force);
	void setWind(glm::vec3 wind_);
	void resetNormal();
	void addToNormal(glm::vec3 V);
	glm::vec3 getCurrentPosition();
	glm::vec3 getColour();
	GLuint getIndex();
	void clearForces();
	glm::vec3 getVelocity();
private:	
	glm::vec3 currentPosition, colour, mNormal, previousPosition, forceOfGravity, acceleration, gravity, wind, velocity;
	bool stationary;
	GLuint index;
	GLfloat Damping, mass;
	void verletIntegration(atlas::utils::Time const& t), eulerIntegration(atlas::utils::Time const& t);
};
#endif // !PARTICLE_HPP
