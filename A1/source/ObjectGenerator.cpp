#include "ObjectGenerator.h"
#define NUM_ARRAY_ELEMENTS(a) sizeof(a) / sizeof(*a); //Used to dynamically caculate the number of elements in an array

//Create the spring mass vertex and color data
ShapeData ObjectGenerator::makeSpringMass(glm::vec3 anchorPosition, GLfloat springWidth, GLfloat stretch, GLfloat width, GLfloat height) {
	ShapeData ret;
	glm::vec3 springColor{ 1.0f, 0.0f, 0.0f }, massColor{ 0.0f, 1.0f, 0.0f }, connectionPoint{ anchorPosition.x, anchorPosition.y - (17 * stretch), 0.0f };
	Vertex vertices[] = {
		glm::vec3(anchorPosition.x, anchorPosition.y, 0.0f), // 0
		springColor,
		glm::vec3(anchorPosition.x, anchorPosition.y - (0.1 *stretch),0.0f), // 1
		springColor,
		glm::vec3(anchorPosition.x + springWidth, anchorPosition.y - (1 * stretch), 0.0f), // 2
		springColor,
		glm::vec3(anchorPosition.x - springWidth, anchorPosition.y - (3 * stretch), 0.0f), // 3
		springColor,
		glm::vec3(anchorPosition.x + springWidth, anchorPosition.y - (5 * stretch), 0.0f), // 4
		springColor,
		glm::vec3(anchorPosition.x - springWidth, anchorPosition.y - (7 * stretch), 0.0f), // 5
		springColor,
		glm::vec3(anchorPosition.x + springWidth, anchorPosition.y - (9 * stretch), 0.0f), // 6
		springColor,
		glm::vec3(anchorPosition.x - springWidth, anchorPosition.y - (11 * stretch), 0.0f), // 7
		springColor,
		glm::vec3(anchorPosition.x + springWidth, anchorPosition.y - (13 * stretch), 0.0f), // 8
		springColor,
		glm::vec3(anchorPosition.x - springWidth, anchorPosition.y - (15 * stretch), 0.0f), // 9
		springColor,
		glm::vec3(anchorPosition.x, anchorPosition.y - (16 * stretch), 0.0f), // 10
		springColor,
		connectionPoint, // 11 ===> The joint between the spring and the mass
		springColor,
		//=================Mass==============//
		glm::vec3(connectionPoint.x - width, connectionPoint.y, 0.0f), //top Left 12
		massColor,
		glm::vec3(connectionPoint.x + width, connectionPoint.y, 0.0f), //top Right 13
		massColor,
		glm::vec3(connectionPoint.x + width, connectionPoint.y - height, 0.0f), // bottom right 14
		massColor,
		glm::vec3(connectionPoint.x - width, connectionPoint.y - height, 0.0f), // bottom left 15
		massColor,
	};
	ret.numVertices = NUM_ARRAY_ELEMENTS(vertices);
	ret.vertices = new Vertex[ret.numVertices];

	//Used to ensure the data we have just created on the stack is moved into location that won't be overwritten
	//As opposed to a reference to a stack array which can be damaged 
	memcpy(ret.vertices, vertices, sizeof(vertices)); //memcpy(dest, source, size);

	GLushort indices[] = { 0,1 ,1,2, 2,3, 3,4, 4,5, 5,6, 6,7, 7,8, 8,9, 9,10, 10,11, 11,12, 12,13, 13,14, 14,15, 15,12 };
	ret.numIndices = NUM_ARRAY_ELEMENTS(indices);
	ret.indices = new GLushort[ret.numIndices];

	//Used to ensure the data we have just created on the stack is moved into location that won't be overwritten
	//As opposed to a reference to a stack array which can be damaged 
	memcpy(ret.indices, indices, sizeof(indices));

	return ret;
}

ShapeData ObjectGenerator::makeAngularSpringMass(glm::vec3 anchorPosition, GLfloat springWidth, GLfloat stretch, GLfloat width, GLfloat height, GLfloat angle) {
	ShapeData ret;
	angle = glm::radians(angle);
	glm::vec3 springColor{ 1.0f, 0.0f, 0.0f }, massColor{ 0.0f, 1.0f, 0.0f }, connectionPoint{ anchorPosition.x, anchorPosition.y - (17 * stretch), 0.0f };
	
	//The original model
	Vertex springMass[] = {
		glm::vec3(anchorPosition.x, anchorPosition.y, 0.0f), // 0
		springColor,
		glm::vec3(anchorPosition.x, anchorPosition.y - (0.1 *stretch) ,0.0f), // 1
		springColor,
		glm::vec3(anchorPosition.x + springWidth, anchorPosition.y - (1 * stretch) , 0.0f), // 2
		springColor,
		glm::vec3(anchorPosition.x - springWidth, anchorPosition.y - (3 * stretch) , 0.0f), // 3
		springColor,
		glm::vec3(anchorPosition.x + springWidth, anchorPosition.y - (5 * stretch) , 0.0f), // 4
		springColor,
		glm::vec3(anchorPosition.x - springWidth, anchorPosition.y - (7 * stretch) , 0.0f), // 5
		springColor,
		glm::vec3(anchorPosition.x + springWidth, anchorPosition.y - (9 * stretch) , 0.0f), // 6
		springColor,
		glm::vec3(anchorPosition.x - springWidth, anchorPosition.y - (11 * stretch) , 0.0f), // 7
		springColor,
		glm::vec3(anchorPosition.x + springWidth, anchorPosition.y - (13 * stretch) , 0.0f), // 8
		springColor,
		glm::vec3(anchorPosition.x - springWidth, anchorPosition.y - (15 * stretch) , 0.0f), // 9
		springColor,
		glm::vec3(anchorPosition.x, anchorPosition.y - (16 * stretch) , 0.0f), // 10
		springColor,
		connectionPoint, // 11 ===> The joint between the spring and the mass
		springColor,
		//=================Mass==============//
		glm::vec3(connectionPoint.x - width, connectionPoint.y, 0.0f), //top Left 12
		massColor,
		glm::vec3(connectionPoint.x + width, connectionPoint.y, 0.0f), //top Right 13
		massColor,
		glm::vec3(connectionPoint.x + width, connectionPoint.y - height, 0.0f), // bottom right 14
		massColor,
		glm::vec3(connectionPoint.x - width, connectionPoint.y - height, 0.0f), // bottom left 15
		massColor,
	};

	//New vertices recomputed around anchorPosition
	Vertex vertices[] = {
		glm::vec3(anchorPosition.x, anchorPosition.y, 0.0f), // 0
		springColor,
		glm::vec3(getRotatedX(angle, springMass[1], anchorPosition),  getRotatedY(angle, springMass[1], anchorPosition), 0.0f), // 1
		springColor,
		glm::vec3(getRotatedX(angle, springMass[2], anchorPosition),  getRotatedY(angle, springMass[2], anchorPosition), 0.0f), // 2
		springColor,
		glm::vec3(getRotatedX(angle, springMass[3], anchorPosition),  getRotatedY(angle, springMass[3], anchorPosition), 0.0f), // 3
		springColor,
		glm::vec3(getRotatedX(angle, springMass[4], anchorPosition),  getRotatedY(angle, springMass[4], anchorPosition), 0.0f), // 4
		springColor,
		glm::vec3(getRotatedX(angle, springMass[5], anchorPosition),  getRotatedY(angle, springMass[5], anchorPosition), 0.0f), // 5
		springColor,
		glm::vec3(getRotatedX(angle, springMass[6], anchorPosition),  getRotatedY(angle, springMass[6], anchorPosition), 0.0f), // 6
		springColor,
		glm::vec3(getRotatedX(angle, springMass[7], anchorPosition),  getRotatedY(angle, springMass[7], anchorPosition), 0.0f), // 7
		springColor,
		glm::vec3(getRotatedX(angle, springMass[8], anchorPosition),  getRotatedY(angle, springMass[8], anchorPosition), 0.0f), // 8
		springColor,
		glm::vec3(getRotatedX(angle, springMass[9], anchorPosition),  getRotatedY(angle, springMass[9], anchorPosition), 0.0f), // 9
		springColor,
		glm::vec3(getRotatedX(angle, springMass[10], anchorPosition),  getRotatedY(angle, springMass[10], anchorPosition), 0.0f), // 10
		springColor,
		glm::vec3(getRotatedX(angle, springMass[11], anchorPosition),  getRotatedY(angle, springMass[11], anchorPosition), 0.0f), // 11 ===> The joint between the spring and the mass
		springColor,
		//=================Mass==============//
		glm::vec3(getRotatedX(angle, springMass[12], anchorPosition),  getRotatedY(angle, springMass[12], anchorPosition), 0.0f), // 12
		massColor,
		glm::vec3(getRotatedX(angle, springMass[13], anchorPosition),  getRotatedY(angle, springMass[13], anchorPosition), 0.0f), // 13
		massColor,
		glm::vec3(getRotatedX(angle, springMass[14], anchorPosition),  getRotatedY(angle, springMass[14], anchorPosition), 0.0f), // 14
		massColor,
		glm::vec3(getRotatedX(angle, springMass[15], anchorPosition),  getRotatedY(angle, springMass[15], anchorPosition), 0.0f), // 15
		massColor,
	};
	ret.numVertices = NUM_ARRAY_ELEMENTS(vertices);
	ret.vertices = new Vertex[ret.numVertices];

	//Used to ensure the data we have just created on the stack is moved into location that won't be overwritten
	//As opposed to a reference to a stack array which can be damaged 
	memcpy(ret.vertices, vertices, sizeof(vertices)); //memcpy(dest, source, size);

	GLushort indices[] = { 0,1 ,1,2, 2,3, 3,4, 4,5, 5,6, 6,7, 7,8, 8,9, 9,10, 10,11, 11,12, 12,13, 13,14, 14,15, 15,12 };
	ret.numIndices = NUM_ARRAY_ELEMENTS(indices);
	ret.indices = new GLushort[ret.numIndices];
	ret.connectionPoint = connectionPoint;

	//Used to ensure the data we have just created on the stack is moved into location that won't be overwritten
	//As opposed to a reference to a stack array which can be damaged 
	memcpy(ret.indices, indices, sizeof(indices));

	return ret;
}

//Compute the new rotated X position of the vertices around anchorPosisition
GLfloat ObjectGenerator::getRotatedX(GLfloat angle, Vertex old, glm::vec3 anchorPosition) {
	return (glm::cos(angle) * (old.position.x - anchorPosition.x)) + (glm::sin(angle) * (old.position.y - anchorPosition.y)) + anchorPosition.x;
}
//Compute the new rotated Y position of the vertices around anchorPosisition
GLfloat ObjectGenerator::getRotatedY(GLfloat angle, Vertex old, glm::vec3 anchorPosition) {
	return -(glm::sin(angle) * (old.position.x - anchorPosition.x)) + (glm::cos(angle) * (old.position.y - anchorPosition.y)) + anchorPosition.y;
}