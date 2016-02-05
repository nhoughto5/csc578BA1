#include "ClothCamera.hpp"
#include <iostream>
#include <glm\gtx\transform.hpp>

ClothCamera::ClothCamera() :
	viewDirection{0.0f, 0.0f, -1.0f },
	upDirection{0.0f,1.0f,0.0f },
	thresholdJump(8.0f),
	MOVEMENT_SPEED(0.1f), ROTATION_SPEED(0.0025f), SPIN_SPEED(40.0f)
{

}
ClothCamera::~ClothCamera() {

}

glm::mat4 ClothCamera::getCameraMatrix() {
	return glm::lookAt(position, position + viewDirection, upDirection);
}

void ClothCamera::strafeCamera(int direction) {
	switch (direction) {
	case 0:
		position += MOVEMENT_SPEED * viewDirection;
		break;
	case 1:
		position -= MOVEMENT_SPEED * viewDirection;
		break;
	case 2:
		position -= MOVEMENT_SPEED * strafeDirection;
		break;
	case 3:
		position += MOVEMENT_SPEED * strafeDirection;
		break;
	case 4:
		position += MOVEMENT_SPEED * upDirection;
		break;
	case 5:
		position -= MOVEMENT_SPEED * upDirection;
		break;
	case 6:
		viewDirection = glm::mat3(glm::rotate(SPIN_SPEED * ROTATION_SPEED, upDirection)) * viewDirection;
		break;
	case 7:
		viewDirection = glm::mat3(glm::rotate(-SPIN_SPEED * ROTATION_SPEED, upDirection)) * viewDirection;
		break;
	default:
		break;
	}
	std::cout << "Position: " << position.x << ", " << position.y << ", " << position.z << "\n";
}
void ClothCamera::newPosition(glm::vec3 newPosition) {
	position = newPosition + 0.1f * upDirection;

}
void ClothCamera::lookAt(glm::vec3 target) {
	viewDirection = position - target;
}
void ClothCamera::mouseUpdate(const glm::vec2& newMousePosition) {
	glm::vec2 mouseDelta = newMousePosition - oldMousePosition;
	if (glm::length(mouseDelta) > 50.0f)
	{
		oldMousePosition = newMousePosition;
		return;
	}
	strafeDirection = glm::cross(viewDirection, upDirection);
	glm::mat4 rotator = glm::rotate(-mouseDelta.x * ROTATION_SPEED, upDirection) * glm::rotate(-mouseDelta.y * ROTATION_SPEED, strafeDirection);

	viewDirection = glm::mat3(rotator) * viewDirection;

	oldMousePosition = newMousePosition;
}