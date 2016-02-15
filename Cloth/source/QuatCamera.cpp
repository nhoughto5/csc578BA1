#include "QuatCamera.h"
#include <iostream>
#include <glm\gtx\transform.hpp>
QuatCamera::QuatCamera(glm::vec3 pos, glm::vec3 up_, glm::vec3 forward_):
	yAxis{0.0f, 1.0f, 0.0f},
	position{pos},
	forward{forward_},
	up{up_},
	MOVEMENT_SPEED(0.1f), ROTATION_SPEED(0.0025f), SPIN_SPEED(40.0f)
{
	up = getNormalized(up);
	forward = getNormalized(forward);
}
QuatCamera::QuatCamera():
	yAxis{ 0.0f, 1.0f, 0.0f },
	position{ 0.0f, 0.0f, 0.0f },
	forward{ 0.0f, 0.0f, -1.0f },
	up{ 0.0f, 1.0f, 0.0f },
	MOVEMENT_SPEED(0.1f), ROTATION_SPEED(0.0025f), SPIN_SPEED(40.0f)
{
	up = getNormalized(up);
	forward = getNormalized(forward);
}
QuatCamera::~QuatCamera() {

}
void QuatCamera::input(CameraDirection dir_) {
	GLfloat movAmt = 0.5f;
	GLfloat rotAmt = 1.5f;

	switch (dir_) {
		case FORWARD:
			move(forward, movAmt);
			break;
		case BACK:
			move(forward, -movAmt);
			break;
		case LEFT:
			move(getLeft(), movAmt);
			break;
		case RIGHT:
			move(getRight(), movAmt);
			break;
		case UP:
			move(up, movAmt);
			break;
		case DOWN:
			move(up, -movAmt);
			break;
		case TILTDOWN:
			rotateX(rotAmt);
			break;
		case TILTUP:
			rotateX(-rotAmt);
			break;
		case LOOKLEFT:
			rotateY(rotAmt);
			break;
		case LOOKRIGHT:
			rotateY(-rotAmt);
			break;
		default:
			break;
	}
}
void QuatCamera::mouseUpdate(glm::vec2 newMousePosition) {
	glm::vec2 mouseDelta = newMousePosition - oldMousePosition;
	if (glm::length(mouseDelta) > 50.0f)
	{
		oldMousePosition = newMousePosition;
		return;
	}
	glm::mat4 rotator = glm::rotate(-mouseDelta.x * ROTATION_SPEED, up) * glm::rotate(-mouseDelta.y * ROTATION_SPEED, getLeft());
	forward = glm::mat3(rotator) * forward;
	oldMousePosition = newMousePosition;
}
void QuatCamera::setLookat(glm::vec3 loc) {
	forward = getNormalized(loc - position);
}
void QuatCamera::setPosition(glm::vec3 p) {
	position = p;
}
glm::mat4 QuatCamera::getCameraMatrix() {
	return glm::lookAt(position, position + forward, up);
}
void QuatCamera::move(glm::vec3 dir, GLfloat amt) {
	position += dir * amt;
}

glm::vec3 QuatCamera::getLeft() {
	return getNormalized(glm::cross(forward, up));
}
glm::vec3 QuatCamera::getRight() {
	return getNormalized(glm::cross(up,forward));
}
void QuatCamera::rotateX(GLfloat angle) {
	glm::vec3 Haxis = getNormalized(glm::cross(yAxis, forward));
	forward = rotate(forward, angle, Haxis);
	forward = getNormalized(forward);
	up = getNormalized(glm::cross(forward, Haxis));
}

void QuatCamera::rotateY(GLfloat angle) {
	glm::vec3 Haxis = getNormalized(glm::cross(yAxis, forward));
	forward = rotate(forward, angle, yAxis);
	forward = getNormalized(forward);
	up = getNormalized(glm::cross(forward, Haxis));
}
glm::vec3 QuatCamera::rotate(glm::vec3 source, GLfloat angle, glm::vec3 axis) {
	GLfloat sinHalfAngle = sin(glm::radians(angle) / 2);
	GLfloat cosHalfAngle = cos(glm::radians(angle) / 2);

	glm::quat temp;
	temp.x = axis.x * sinHalfAngle;
	temp.y = axis.y * sinHalfAngle;
	temp.z = axis.z * sinHalfAngle;
	temp.w = cosHalfAngle;

	glm::quat result = vectorMulQuat(temp, source) * glm::conjugate(temp);
	return glm::vec3{ result.x, result.y, result.z };
}
glm::quat QuatCamera::vectorMulQuat(glm::quat q, glm::vec3 r)
{
	GLfloat w_ = -q.x * r.x - q.y * r.y - q.z * r.z;
	GLfloat x_ = q.w * r.x + q.y * r.z - q.z * r.y;
	GLfloat y_ = q.w * r.y + q.z * r.x - q.x * r.z;
	GLfloat z_ = q.w * r.z + q.x * r.y - q.y * r.x;

	return glm::quat(w_, x_, y_, z_);
}
glm::vec3 QuatCamera::getNormalized(glm::vec3 V) {
	GLfloat length = glm::length(V);
	if (length < 0.0000001) return glm::vec3{ 0.0f, 0.0f, 0.0f };
	else return normalize(V);
}