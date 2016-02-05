#include "angularSpring.hpp"
#include "ShaderPaths.hpp"
#include "atlas\gl\Shader.hpp"
#include "atlas\core\Macros.hpp"
#include <atlas/utils/Geometry.hpp>
#include <stdio.h>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

const int NUM_VERTICES_PER_LINE = 2;
const int NUMFLOATSPERVERTICES = 6;
const int VERTEX_BYTE_SIZE = NUMFLOATSPERVERTICES * sizeof(float);
GLint numSpringIndices2;

angularSpring::angularSpring():
	anchorPosition{ 0.0f, 0.0f, 0.0f }, //Where the spring is anchored to
	displacement{ 0.0f, 0.0f, 0.0f },
	stretch{ 0.02f }, //How far the nodes seperate
	springWidth{ 0.05f }, //How close the node get to the spring axis
	mVelocity(0.0f),
	k(5.00f), //Spring Constant
	mMass(0.01f),
	angleDegrees(180.0f),
	period(1.25f)
{
	USING_ATLAS_GL_NS; 
	USING_ATLAS_MATH_NS;
	glGenVertexArrays(1, &mVertexArrayObject);
	glBindVertexArray(mVertexArrayObject);
	
	//Create the shape data object
	mSpringMass = ObjectGenerator::makeAngularSpringMass(anchorPosition, springWidth, stretch, massWidth, massHeight, angleDegrees);
	//Store a local copy of the conneciton point between the end of the spring and the top of the mass
	connectionPoint = mSpringMass.getConnectionPoint(); 
	//Keep a local copy of the number of indices so the clean up function can be used
	//And the number of vertices is not lost
	numSpringIndices2 = mSpringMass.numIndices; 

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
		ShaderInfo{ GL_VERTEX_SHADER, shaderDir + "angularSpring.vs.glsl" },
		ShaderInfo{ GL_FRAGMENT_SHADER, shaderDir + "angularSpring.fs.glsl" }
	};
	// Create a new shader and add it to our list.
	mShaders.push_back(ShaderPointer(new Shader));
	mShaders[0]->compileShaders(shaders);
	mShaders[0]->linkShaders();
	mShaders[0]->disableShaders();
	mSpringMass.cleanup(); //Prevent memory leakage
}

angularSpring::~angularSpring()
{
	glDeleteVertexArrays(1, &mVertexArrayObject);
	glDeleteBuffers(1, &mVertexBufferObject);
	glDeleteBuffers(1, &springIndexBufferID);
}

void angularSpring::renderGeometry(atlas::math::Matrix4 projection, atlas::math::Matrix4 view){
	// To avoid warnings from unused variables, you can use the 
	//UNUSED macro.
	UNUSED(projection);
	UNUSED(view);

	mShaders[0]->enableShaders();
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject); //Bind the vertex and color information to the GL_ARRAY_BUFFER binding point
	glDrawElements(GL_LINES, numSpringIndices2, GL_UNSIGNED_SHORT, 0); //Draw the model according to the indices
	mShaders[0]->disableShaders();
}

//Increases the angleDegrees variable according to the perdiod of rotation. The ObjectGenerator class
//Is then used to recreate the vertex position data rotateted around the anchor position 
void angularSpring::updateGeometry(atlas::utils::Time const& t) {
	//Increase the angle according to the period
	angleDegrees += t.deltaTime * (360.0f / period);

	//Distance from the centre of rotation to the joint between the spring and the mass
	radius = glm::distance(connectionPoint, anchorPosition);

	//Standard velocity used to compute centrifugal force
	mVelocity = float((2 * M_PI * radius) / period);

	//Force pulling the mass away from the centre of rotation
	centrifugalForce = (mMass * mVelocity * mVelocity) / radius; // Fc = (m*sqrd(V) / r)

	//Force caused by the spring perventing the mass from flying away
	springForce = -(k * displacement.y);

	//Total force
	mForce = centrifugalForce + springForce;

	//Use the total force to calculate the spring's dispalcement from its rest length
	displacement.y = mForce / k;
	GLfloat stretched = stretch + displacement.y; //Displacement plus the natural resting position of the spring
	GLfloat springWidth2 = springWidth - displacement.y;
	
	//Regenerate the spring mass object with the new rotated vertex information
	mSpringMass = ObjectGenerator::makeAngularSpringMass(anchorPosition, springWidth2, stretched, massWidth, massHeight, angleDegrees);

	//Save a local copy of the connectionPoint location to be used in the radius calculation of the following iteration
	connectionPoint = mSpringMass.getConnectionPoint();

	//Overwrite the vertex data in the buffer
	glBufferSubData(GL_ARRAY_BUFFER, 0, mSpringMass.vertexBufferSize(), mSpringMass.vertices);

	//Prevent memory leakage
	mSpringMass.cleanup();
}