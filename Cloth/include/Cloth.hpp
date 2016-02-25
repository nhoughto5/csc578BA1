#ifndef CLOTH_HPP
#define CLOTH_HPP
#pragma once
#include <atlas/utils/Geometry.hpp>
#include "Particle.hpp"
#include "Spring.hpp"
#include "ShaderPaths.hpp"
USING_ATLAS_MATH_NS;
USING_ATLAS_GL_NS;
class Cloth : public atlas::utils::Geometry {
public:
	Cloth(GLfloat width, GLfloat height, GLuint numParticlesWide, GLuint numParticlesHigh);
	~Cloth();
	void renderGeometry(atlas::math::Matrix4 projection,atlas::math::Matrix4 view) override;
	void updateGeometry(atlas::utils::Time const& t) override;
private:

	Particle* getParticle(GLuint x, GLuint y);
	void addWind(glm::vec3 windForce);
	void addWindForceToTriangle(Particle* p1, Particle* p2, Particle* p3, glm::vec3 windForce);
	void sendDataToGPU();
	void defineVAO();
	GLfloat width, height, clothRotationAngle;
	GLuint numParticlesWide, numParticlesHigh, clothVertexBufferID, clothIndexBufferID;
	GLuint VAO;
	std::vector<Particle> mParticles;
	std::vector<GLuint> mParticleIndices;
	std::vector<Spring> mSprings;
	glm::vec3 clothRotationVector, clothPosition, gravity, ambientLight, diffuseLightPosition;
	GLfloat restLength;
};
#endif // !CLOTH_HPP
