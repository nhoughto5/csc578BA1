#include "Axis.h"
#include "ObjectGenerator.h"
#include "ShapeData.h"
#include "ShaderPaths.hpp"
const int NUM_VERTICES_PER_LINE = 2;
const int NUMFLOATSPERVERTICES = 6;
const int VERTEX_BYTE_SIZE = NUMFLOATSPERVERTICES * sizeof(float);
Axis::Axis() {
	ShapeData Axis = ObjectGenerator::makeAxis();
	glGenBuffers(1, &axisVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, axisVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, Axis.vertexBufferSize(), Axis.vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &axisIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, axisIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Axis.indexBufferSize(), Axis.indices, GL_STATIC_DRAW);
	numIndices = Axis.numIndices;
	Axis.cleanup();

	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, axisVertexBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (char*)(sizeof(float) * 3));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, axisIndexBuffer);


	std::string shaderDir = generated::ShaderPaths::getShaderDirectory();

	// Now set up the information for our shaders.
	std::vector<ShaderInfo> shaders
	{
		ShaderInfo{ GL_VERTEX_SHADER, shaderDir + "Axis.vs.glsl" },
		ShaderInfo{ GL_FRAGMENT_SHADER, shaderDir + "Axis.fs.glsl" }
	};
	// Create a new shader and add it to our list.
	mShaders.push_back(ShaderPointer(new Shader));
	mShaders[0]->compileShaders(shaders);
	mShaders[0]->linkShaders();

	GLuint var;
	var = mShaders[0]->getUniformVariable("uMVP");
	mUniforms.insert(UniformKey("uMVP", var));
	mShaders[0]->disableShaders();
}
Axis::~Axis() {
	glDeleteVertexArrays(1, &vertexArrayObject);
}
void Axis::renderGeometry(atlas::math::Matrix4 projection, atlas::math::Matrix4 view) {
	glBindVertexArray(vertexArrayObject);
	mShaders[0]->enableShaders();
	Matrix4 mvp = projection * view * mModel;
	glUniformMatrix4fv(mUniforms["uMVP"], 1, GL_FALSE, &mvp[0][0]);
	glLineWidth(5.0f);
	glDrawElements(GL_LINES, numIndices, GL_UNSIGNED_SHORT, 0);
	glLineWidth(1.0f);
	mShaders[0]->disableShaders();
}