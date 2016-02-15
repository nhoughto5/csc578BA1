//#include "ClothCamera.hpp"
//#include <iostream>
//#include <glm\gtx\transform.hpp>
//
//ClothCamera::ClothCamera() :
//	thresholdJump(8.0f),
//	MOVEMENT_SPEED(0.1f), ROTATION_SPEED(0.0025f), SPIN_SPEED(40.0f),
//	orientation(1.0f, 0.0f, 0.0f, 0.0f),
//	X_AXIS{ 1.0f, 0.0f, 0.0f },
//	Y_AXIS{ 0.0f, 1.0f, 0.0f },
//	Z_AXIS{ 0.0f, 0.0f, 1.0f }
//{
//	updateVectors();
//}
//ClothCamera::~ClothCamera() {
//}
//void ClothCamera::updateVectors() {
//	upDirection = getUp();
//	forward = getForward();
//	right = getRight();
//	left = glm::cross(upDirection, forward);
//	std::cout << "Forward: {" << forward.x << ", " << forward.y << ", " << forward.z << "}   "
//		<< "upDirection: {" << upDirection.x << ", " << upDirection.y << ", " << upDirection.z << "}   "
//		<< "Right: {" << right.x << ", " << right.y << ", " << right.z << "}   "
//		<< "Left: {" << left.x << ", " << left.y << ", " << left.z << "}\n";
//}
//glm::mat4 ClothCamera::getCameraMatrix() {
//	return glm::lookAt(position, position + forward, upDirection);
//}
//
//void ClothCamera::move(CameraDirection dir) {
//	switch (dir) {
//	case FORWARD:
//		position += MOVEMENT_SPEED * forward;
//		break;
//	case BACK:
//		position -= MOVEMENT_SPEED * forward;
//		break;
//	case LEFT:
//		position += MOVEMENT_SPEED * right;
//		break;
//	case RIGHT:
//		position -= MOVEMENT_SPEED * right;
//		break;
//	case UP:
//		position += MOVEMENT_SPEED * upDirection;
//		break;
//	case DOWN:
//		position -= MOVEMENT_SPEED * upDirection;
//		break;
//	case LOOKLEFT:
//		forward = glm::mat3(glm::rotate(SPIN_SPEED * ROTATION_SPEED, upDirection)) * forward;
//		break;
//	case LOOKRIGHT:
//		forward = glm::mat3(glm::rotate(-SPIN_SPEED * ROTATION_SPEED, upDirection)) * forward;
//		break;
//	case ROLLCCW:
//		roll(5.0f);
//		break;
//	case ROLLCW:
//		roll(-5.0f);
//		break;
//	default:
//		break;
//	}
//}
//void ClothCamera::roll(GLfloat theta) {
//	orientation.x = forward.x + sin(theta / 2);
//	orientation.x = forward.y + sin(theta / 2);
//	orientation.x = forward.z + sin(theta / 2);
//	orientation.w = cos(theta / 2);
//	updateVectors();
//}
//void ClothCamera::newPosition(glm::vec3 newPosition) {
//	position = newPosition + 0.1f * upDirection;
//}
//void ClothCamera::lookAt(glm::vec3 target_) {
//	target = target_;
//	forward = -normalize(position - target);
//}
//void ClothCamera::mouseUpdate(const glm::vec2& newMousePosition) {
//	glm::vec2 mouseDelta = newMousePosition - oldMousePosition;
//	if (glm::length(mouseDelta) > 50.0f)
//	{
//		oldMousePosition = newMousePosition;
//		return;
//	}
//	updateVectors();
//	glm::mat4 rotator = glm::rotate(-mouseDelta.x * ROTATION_SPEED, upDirection) * glm::rotate(-mouseDelta.y * ROTATION_SPEED, left);
//
//	forward = glm::mat3(rotator) * forward;
//
//	oldMousePosition = newMousePosition;
//}
//
//glm::vec3 ClothCamera::getForward() {
//	return glm::vec3(2 * (orientation.x * orientation.z + orientation.w * orientation.y), 2 * (orientation.y * orientation.z - orientation.w * orientation.x), 1 - 2 * (orientation.x * orientation.x + orientation.y * orientation.y));
//}
//glm::vec3 ClothCamera::getUp() {
//	return glm::vec3(2 * (orientation.x * orientation.y - orientation.w * orientation.z), 1 - 2 * (orientation.x * orientation.x + orientation.z * orientation.z), 2 * (orientation.y * orientation.z + orientation.w * orientation.x));
//}
//glm::vec3 ClothCamera::getRight() {
//	return glm::vec3(1 - 2 * (orientation.y * orientation.y + orientation.z * orientation.z), 2 * (orientation.x * orientation.y + orientation.w * orientation.z), 2 * (orientation.x * orientation.z - orientation.w * orientation.y));
//}