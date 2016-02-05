#include "Spring.hpp"
#include "ShaderPaths.hpp"
#include "atlas\gl\Shader.hpp"
#include "atlas\core\Macros.hpp"
#include <atlas/utils/Geometry.hpp>
const int NUM_VERTICES_PER_LINE = 2;
const int NUMFLOATSPERVERTICES = 6;
const int VERTEX_BYTE_SIZE = NUMFLOATSPERVERTICES * sizeof(float);

GLint numSpringIndices;
#define NUM_ARRAY_ELEMENTS(a) sizeof(a) / sizeof(*a);
Spring::Spring() : 
	anchorPosition{ 0.0f, 1.0f, 0.0f }, //Where the spring is anchored to
	displacement{ 0.0f, -1.5f, 0.0f }, //Starting displacement from the rest length
	stretch{ 0.02f }, //How far the nodes seperate
	springWidth{ 0.05f }, //How close the node get to the spring axis
	gravity{ -9.81f }, 
	mVelocity(0.0f),
	k(1.0f),//Spring Constant
	mMass(0.03f),
	damping(0.05f)
{
	USING_ATLAS_GL_NS; //Short for atlas GL namespace
	USING_ATLAS_MATH_NS;
	glGenVertexArrays(1, &mVertexArrayObject);
	glBindVertexArray(mVertexArrayObject);

	//Create the shape data object
	mSpringMass = ObjectGenerator::makeSpringMass(anchorPosition, springWidth, stretch, massWidth, massHeight);
	
	//Keep a local copy of the number of indices so the clean up function can be used
	//And the number of vertices is not lost
	numSpringIndices = mSpringMass.numIndices;

	//Create the buffers for the vertices and color information 
	glGenBuffers(1, &mVertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, mSpringMass.vertexBufferSize(), NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (char*)(sizeof(float) * 3));

	//Generate the indices buffer and bind to the GL_ELEMENT_ARRAY_BUFFER binding point.
	glGenBuffers(1, &springIndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, springIndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mSpringMass.indexBufferSize(), mSpringMass.indices, GL_DYNAMIC_DRAW);
	// Get the path where our shaders are stored.
	std::string shaderDir = generated::ShaderPaths::getShaderDirectory();

	// Now set up the information for our shaders.
	std::vector<ShaderInfo> shaders
	{
		ShaderInfo{ GL_VERTEX_SHADER, shaderDir + "Spring.vs.glsl" },
		ShaderInfo{ GL_FRAGMENT_SHADER, shaderDir + "Spring.fs.glsl" }
	};
	// Create a new shader and add it to our list.
	mShaders.push_back(ShaderPointer(new Shader));
	mShaders[0]->compileShaders(shaders);
	mShaders[0]->linkShaders();
	mShaders[0]->disableShaders();

	//Prevent memory leakage
	mSpringMass.cleanup();
}

Spring::~Spring()
{
	glDeleteVertexArrays(1, &mVertexArrayObject);
	glDeleteBuffers(1, &mVertexBufferObject);
}

void Spring::renderGeometry(atlas::math::Matrix4 projection, atlas::math::Matrix4 view) {
	// To avoid warnings from unused variables, you can use the 
	//UNUSED macro.
	UNUSED(projection);
	UNUSED(view);
	glBindVertexArray(mVertexArrayObject);
	mShaders[0]->enableShaders();
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);

	//Retreive the location of the 'displacement' and 'anchorPosition' uniform variables
	GLint dispLocation = mShaders[0]->getUniformVariable("displacement");
	GLint anchorLocation = mShaders[0]->getUniformVariable("anchorPosition");
	glUniform3fv(anchorLocation, 1, &anchorPosition[0]);
	glUniform3fv(dispLocation, 1, &displacement[0]);
	glDrawElements(GL_LINES, numSpringIndices, GL_UNSIGNED_SHORT, 0);
	mShaders[0]->disableShaders();
}

void Spring::updateGeometry(atlas::utils::Time const& t) {
	
	forceOfGravity = mMass * gravity;
	springForce = -(k * displacement.y);

	//Calculate the total force acting on the mass.
	mForce = forceOfGravity + springForce - mVelocity * damping;

	//========= Euler Integrate =========//
	displacement.y = displacement.y + t.deltaTime * mVelocity;
	mVelocity = mVelocity + t.deltaTime * (mForce / mMass);
	//==================================//

	//Use the total force to calculate the spring's dispalcement from its rest length
	GLfloat stretched = stretch * (1.0f - (displacement.y));
	GLfloat springWidth2 = springWidth * (1.0f + (0.3f * displacement.y));

	//Regenerate the vertex data to create the effect of the nodes seperating and moving in and out towards 
	//The central axis of the spring.
	mSpringMass = ObjectGenerator::makeSpringMass(anchorPosition, springWidth2, stretched, massWidth, massHeight);
	glBufferSubData(GL_ARRAY_BUFFER, 0, mSpringMass.vertexBufferSize(), mSpringMass.vertices);
	mSpringMass.cleanup();
}
