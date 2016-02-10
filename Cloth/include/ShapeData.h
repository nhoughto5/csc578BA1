#ifndef SHAPE_DATA_H
#define SHAPE_DATA_H
#pragma once
#include <atlas/utils/Geometry.hpp>

#include "Vertex.h"

struct ShapeData {
	ShapeData() : vertices(0), numVertices(0), indices(0), numIndices(0) {};
	Vertex* vertices;
	GLuint numVertices;
	GLushort* indices;
	GLuint numIndices;
	glm::vec3 connectionPoint;
	glm::vec3 getConnectionPoint() {
		return connectionPoint;
	}
	GLsizeiptr vertexBufferSize() const {
		return numVertices * sizeof(Vertex);
	}
	GLsizeiptr indexBufferSize() const {
		return numIndices * sizeof(GLushort);
	}
	void cleanup() {
		vertices = NULL;
		indices = NULL;
		numIndices = numVertices = 0;
	}
};
#endif