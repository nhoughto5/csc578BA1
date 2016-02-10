#include "Cloth.hpp"
#include <glm\gtx\transform.hpp>
Cloth::Cloth(GLfloat width_, GLfloat height_, GLuint numParticlesWide_, GLuint numParticlesHigh_) :
	width(width_),
	height(height_),
	numParticlesHigh(numParticlesHigh_),
	numParticlesWide(numParticlesWide_),
	clothRotationVector{0.0f, 0.0f, 0.0f},
	clothTranslation{ 0.0f, 0.0f, 0.0f },
	clothRotationAngle(0.0f)
{
	glm::vec3 clothColour{1.0f, 0.0f, 0.0f};
	//Create Particles
	GLuint count = 0;
	for (GLuint y = 0; y < numParticlesHigh; ++y) {
		for (GLuint x = 0; x < numParticlesWide; ++x) {
			glm::vec3 pos = {(width * (x / (float)numParticlesWide)), (-height * (y / (float)numParticlesHigh)), 0.0f};
			mParticles.push_back(Particle(pos, count, clothColour));
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

			//Create the X pattern 
			if (x < numParticlesWide - 2 && y < numParticlesHigh - 2) {
				mSprings.push_back(Spring(getParticle(x, y), getParticle(x+2,y+2)));
				mSprings.push_back(Spring(getParticle(x+2,y), getParticle(x,y+2)));
			};
		}
	}

	//Set the top left and right as stationary
	getParticle(0, 0)->makeStationary();
	getParticle(numParticlesWide - 1, 0)->makeStationary();

	//Make Indices for Particles
	for (GLuint row = 0; row < numParticlesWide - 1; ++row) {
		for (GLuint col = 0; col < numParticlesHigh - 1; ++col) {
			//Triangle one
			mParticleIndices.push_back(getParticle(row,col)->index);
			mParticleIndices.push_back(getParticle(row+1,col)->index);
			mParticleIndices.push_back(getParticle(row, col+1)->index);

			//Triangle two
			mParticleIndices.push_back(getParticle(row, col+1)->index);
			mParticleIndices.push_back(getParticle(row+1, col)->index);
			mParticleIndices.push_back(getParticle(row+1, col+1)->index);
		}
	}
	glGenBuffers(1, &clothVertexBufferID);
	glGenBuffers(1, &clothIndexBufferID);

	defineVAO();
	sendDataToGPU();
	std::string shaderDir = generated::ShaderPaths::getShaderDirectory();
	std::vector<ShaderInfo> shaders
	{
		{ GL_VERTEX_SHADER, shaderDir + "Cloth.vs.glsl" },
		{ GL_FRAGMENT_SHADER, shaderDir + "Cloth.fs.glsl" }
	};

	mShaders.push_back(ShaderPointer(new Shader));
	mShaders[0]->compileShaders(shaders);
	mShaders[0]->linkShaders();
	mUniforms.insert(UniformKey("mvpMat",mShaders[0]->getUniformVariable("mvpMat")));
	mShaders[0]->disableShaders();
}

Cloth::~Cloth() {

}

void Cloth::renderGeometry(atlas::math::Matrix4 projection, atlas::math::Matrix4 view) {
	mModel = glm::translate(clothTranslation) * glm::rotate(clothRotationAngle, clothRotationVector);
	auto mvpMat = projection * view * mModel;
	glUniformMatrix4fv(mUniforms["mvpMat"], 1, GL_FALSE, &mvpMat[0][0]);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, mParticleIndices.size(), GL_UNSIGNED_SHORT, 0);
}
void Cloth::defineVAO() {
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, clothVertexBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), 0); //Define vertex position buffer locations
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(3 * sizeof(GLfloat))); //Define vertex colour buffer locations
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, clothIndexBufferID);
}
void Cloth::sendDataToGPU() {

	glBindBuffer(GL_ARRAY_BUFFER, clothVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER,mParticles.size() * sizeof(Particle), &mParticles[0], GL_STREAM_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, clothIndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mParticleIndices.size() * sizeof(GLuint), &mParticleIndices[0], GL_STATIC_DRAW);
}
Particle* Cloth::getParticle(GLuint x, GLuint y) {
	return &mParticles[y*numParticlesWide + x]; 
}