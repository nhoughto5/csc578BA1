#include "Cloth.hpp"
#include <iostream>
Cloth::Cloth(GLfloat width_, GLfloat height_, GLuint numParticlesWide_, GLuint numParticlesHigh_) :
	width(width_),
	height(height_),
	numParticlesHigh(numParticlesHigh_),
	numParticlesWide(numParticlesWide_),
	clothRotationVector{0.0f, 0.0f, 0.0f},
	clothPosition{ 0.0f, 5.0f, 0.0f },
	clothRotationAngle(0.0f)
{
	USING_ATLAS_MATH_NS;
	USING_ATLAS_GL_NS;
	glm::vec3 clothColour{1.0f, 0.3f, 0.1f};
	//Create Particles
	GLuint count = 0;
	restLength = (width * (1 / (float)numParticlesWide));
	for (GLuint y = 0; y < numParticlesHigh; ++y) {
		for (GLuint x = 0; x < numParticlesWide; ++x) {
			glm::vec3 pos = {(width * (x / (float)numParticlesWide)), (-height * (y / (float)numParticlesHigh)), 0.0f};
			mParticles.push_back(Particle(pos, count, clothColour, x, y));
			++count;
		}
	}
	//Create Springs
	for (GLuint x = 0; x < numParticlesWide; ++x) {
		for (GLuint y = 0; y < numParticlesHigh; ++y) {

			//============ Structural springs ==========//
			//Connect to the particle to the immediate right of the current particle
			if (x < numParticlesWide - 1) mSprings.push_back(Spring(getParticle(x,y), getParticle(x+1,y)));

			//Connect to the particle that is immediately below the current particle
			if (y < numParticlesHigh - 1) mSprings.push_back(Spring(getParticle(x,y), getParticle(x,y+1)));

			//============ Shear Springs ================//
			//Connect the shear springs to make the X pattern
			if (x < numParticlesWide - 1 && y < numParticlesHigh - 1) {
				mSprings.push_back(Spring(getParticle(x, y), getParticle(x + 1, y + 1)));
				mSprings.push_back(Spring(getParticle(x+1, y), getParticle(x, y+1)));
			}

			//============ Bend Springs ===============//
			//Connect the current particle to the second particle over to the right
			if (x < numParticlesWide - 2) mSprings.push_back(Spring(getParticle(x,y), getParticle(x+2,y)));

			//Connect the current particle to the particle two below
			if (y < numParticlesHigh - 2) mSprings.push_back(Spring(getParticle(x,y), getParticle(x, y+2)));
		}
	}

	////Set the top left and right as stationary
	getParticle(0, 0)->makeStationary();
	getParticle(numParticlesWide - 1, 0)->makeStationary();
	//for (int i = 0; i < numParticlesWide; ++i) {
	//	getParticle(i, 0)->makeStationary();
	//}

	//Make Indices for Particles
	for (GLuint row = 0; row < numParticlesWide - 1; ++row) {
		for (GLuint col = 0; col < numParticlesHigh - 1; ++col) {
			//Triangle one
			mParticleIndices.push_back(getParticle(row,col)->getIndex());
			mParticleIndices.push_back(getParticle(row,col+1)->getIndex());
			mParticleIndices.push_back(getParticle(row+1, col)->getIndex());

			//Triangle two
			mParticleIndices.push_back(getParticle(row, col+1)->getIndex());
			mParticleIndices.push_back(getParticle(row+1, col+1)->getIndex());
			mParticleIndices.push_back(getParticle(row+1, col)->getIndex());
		}
	}
	glGenBuffers(1, &clothVertexBufferID);
	glGenBuffers(1, &clothIndexBufferID);
	sendDataToGPU();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, clothIndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mParticleIndices.size() * sizeof(GLushort), &mParticleIndices[0], GL_STATIC_DRAW);
	defineVAO();
	
	std::string shaderDir = generated::ShaderPaths::getShaderDirectory();
	std::vector<ShaderInfo> shaders
	{
		{ GL_VERTEX_SHADER, shaderDir + "Cloth.vs.glsl" },
		{ GL_FRAGMENT_SHADER, shaderDir + "Cloth.fs.glsl" }
	};
	mModel = glm::translate(Matrix4(1.0f), clothPosition);
	mShaders.push_back(ShaderPointer(new Shader));
	mShaders[0]->compileShaders(shaders);
	mShaders[0]->linkShaders();
	mUniforms.insert(UniformKey("mvpMat",mShaders[0]->getUniformVariable("mvpMat")));
	mShaders[0]->disableShaders();
}

Cloth::~Cloth() {

}
void Cloth::addWindForceToTriangle(Particle* p1, Particle* p2, Particle* p3, glm::vec3 windForce) {
	glm::vec3 pos1 = p1->getCurrentPosition();
	glm::vec3 pos2 = p2->getCurrentPosition();
	glm::vec3 pos3 = p3->getCurrentPosition();

	glm::vec3 v1 = pos2 - pos1;
	glm::vec3 v2 = pos3 - pos1;

	glm::vec3 normal = glm::cross(v1,v2);
	normal = glm::normalize(normal);
	glm::vec3 force = normal * glm::dot(normal, windForce);
	p1->addForce(force);
	p2->addForce(force);
	p3->addForce(force);
}
void Cloth::addWind(glm::vec3 windForce) {
	for (int x = 0; x < numParticlesWide-1; ++x) {
		for (int y = 0; y < numParticlesHigh-1; ++y) {
			addWindForceToTriangle(getParticle(x + 1, y), getParticle(x, y), getParticle(x, y + 1), windForce);
			addWindForceToTriangle(getParticle(x + 1, y + 1), getParticle(x + 1, y), getParticle(x, y + 1), windForce);
		}
	}
}
void Cloth::renderGeometry(atlas::math::Matrix4 projection, atlas::math::Matrix4 view) {
	mShaders[0]->enableShaders();
	auto mvpMat = projection * view * mModel;
	glUniformMatrix4fv(mUniforms["mvpMat"], 1, GL_FALSE, &mvpMat[0][0]);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, mParticleIndices.size(), GL_UNSIGNED_SHORT, nullptr);
	mShaders[0]->disableShaders();
}
void Cloth::updateGeometry(atlas::utils::Time const& t) {
	//Remove all previsouly accumulated forces so each iteration 
	//Is a clean instance
	for (int i = 0; i < mParticles.size(); ++i) {
		mParticles[i].clearForces();
		mParticles[i].addForce(glm::vec3(0.0f, -9.8f, 0.0f));
	}
	for (int i = 0; i < mSprings.size(); ++i) {
		mSprings[i].update();
	}
	
	for (int i = 0; i < mParticles.size(); ++i) {
		mParticles[i].updateGeometry(t);
	}
	//addWind(glm::vec3{0.5f, 0.0f, 0.2f} * t.deltaTime);
	sendDataToGPU();
}
void Cloth::defineVAO() {
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, clothVertexBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), 0); //Define vertex position buffer locations
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(3 * sizeof(float))); //Define vertex colour buffer locations
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, clothIndexBufferID);
}
void Cloth::sendDataToGPU() {
	glBindBuffer(GL_ARRAY_BUFFER, clothVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER,mParticles.size() * sizeof(Particle), &mParticles[0], GL_STREAM_DRAW);
}
Particle* Cloth::getParticle(GLuint x, GLuint y) {
	return &mParticles[y*numParticlesWide + x]; 
}