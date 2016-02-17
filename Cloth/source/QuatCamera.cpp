#include "QuatCamera.h"
#include <iostream>
#include <glm\gtx\transform.hpp>

QuatCamera::QuatCamera(glm::vec3 pos, glm::vec3 up_, glm::vec3 forward_) :
	yAxis{ 0.0f, 1.0f, 0.0f },
	position{ pos },
	forward{ forward_ },
	up{ up_ },
	MOVEMENT_SPEED(0.1f), ROTATION_SPEED(0.0025f), SPIN_SPEED(40.0f),
	slerping(false)
{
	up = getNormalized(up);
	forward = getNormalized(forward);
}
QuatCamera::QuatCamera():
	yAxis{ 0.0f, 1.0f, 0.0f },
	position{ 7.0f, 4.0f, 20.0f },
	forward{ 0.0f, 0.0f, -1.0f },
	up{ 0.0f, 1.0f, 0.0f },
	MOVEMENT_SPEED(0.1f), ROTATION_SPEED(0.0025f), SPIN_SPEED(40.0f),
	slerping(false)
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
	target = loc;
	forward = getNormalized(loc - position);
}
void QuatCamera::setTarget(glm::vec3 loc) {
	target = loc;
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
	orientation = result;
	std::cout << "Orientation: {" << orientation.w << ", " << orientation.x << ", " << orientation.y << ", " << orientation.z << "}\n";
	return glm::vec3{ result.x, result.y, result.z };
}
glm::vec3 QuatCamera::rotate(glm::vec3 source, glm::quat q) {
	glm::quat conj = glm::conjugate(q);
	glm::quat w = vectorMulQuat(q, source) * conj;
	return glm::vec3{w.x, w.y, w.z};
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
void QuatCamera::LookAt(glm::vec3 lookVector) {
	assert(lookVector != position);

	glm::vec3 direction = glm::normalize(lookVector - position);
	float dot = glm::dot(glm::vec3(0, 0, 1), direction);
	if (fabs(dot - (-1.0f)) < 0.000001f) {
		rotation = glm::angleAxis((GLfloat)glm::degrees(M_PI), glm::vec3(0, 1, 0));
		return;
	}
	else if (fabs(dot - (1.0f)) < 0.000001f) {
		rotation = glm::quat();
		return;
	}

	float angle = -glm::degrees(acosf(dot));

	glm::vec3 cross = glm::normalize(glm::cross(glm::vec3(0, 0, 1), direction));
	rotation = glm::normalize(glm::angleAxis(angle, cross));
}
void QuatCamera::doSlerp(atlas::utils::Time const& t) {
	GLfloat EPSILON = 0.0001f;
	GLfloat cos = glm::dot(start, rotation);
	if (cos < 0.00000000000f) {
		rotation = -rotation;
		cos = -cos;
	}
	if (cos > 1.00000000000f) {

	}
	GLfloat theta = acos(cos);

	glm::quat result = (sin(1.0f - (slerpProgress * theta)) * start) + (sin(slerpProgress * theta) * rotation);
	result *= 1.0f / (sin(theta));
	current = glm::slerp(current, rotation, t.deltaTime / SLERP_DURATION);
	//current = result;
	forward = rotate(forward, current);
	up = rotate(up, current);
}
void QuatCamera::updateSlerp(atlas::utils::Time const& t) {
	slerpProgress += t.deltaTime / SLERP_DURATION;
	//Slerping Complete
	if ((slerpProgress >= 1.0f) || equals(getNormalized(target - position), forward)) {
		slerping = false;
		slerpProgress = 0.0000000000f;
	}
	else {
		doSlerp(t);
	}
}
void QuatCamera::startSlerp(GLfloat duration) {
	slerping = true;
	SLERP_DURATION = duration;
	slerpProgress = 0.0000000000f;
	start = glm::quat(); //Create start quaternion
	LookAt(target);
}
glm::quat QuatCamera::makeQuat(GLfloat angle, glm::vec3 axis) {
	GLfloat sinHalfAngle = sin(glm::radians(angle) / 2);
	GLfloat cosHalfAngle = cos(glm::radians(angle) / 2);

	glm::quat temp;
	temp.x = axis.x * sinHalfAngle;
	temp.y = axis.y * sinHalfAngle;
	temp.z = axis.z * sinHalfAngle;
	temp.w = cosHalfAngle;
	return temp;
}
glm::quat QuatCamera::getCurrentQuaternion() {
	GLfloat similar = 0.001f;

	if (forward.length() < similar) {
		return glm::quat{1,0, 0, 0};
	}
	return matrix4ToQuaternion(getRotationMatrix(forward, up));
}
glm::quat QuatCamera::getTargetQuaternion() {
	GLfloat similar = 0.001f;

	if (forward.length() < similar) {
		return glm::quat{1,0, 0, 0 };
	}
	return matrix4ToQuaternion(getRotationMatrix(getNormalized(target - position), up));
}
bool QuatCamera::isSlerping() {
	return slerping;
}









glm::mat4 QuatCamera::getRotationMatrix(glm::vec3 forward_, glm::vec3 up_) {
	glm::vec3 f = getNormalized(forward_);
	glm::vec3 r = getNormalized(up_);
	r = glm::cross(r, f);

	glm::vec3 u = glm::cross(f, r);

	glm::mat4 rotMat;


	rotMat[0][0] = r.x;		rotMat[0][1] = r.y;		rotMat[0][2] = r.z;		rotMat[0][3] = 0;
	rotMat[1][0] = u.x;		rotMat[1][1] = u.y;		rotMat[1][2] = u.z;		rotMat[1][3] = 0;
	rotMat[2][0] = f.x;		rotMat[2][1] = f.y;		rotMat[2][2] = f.z;		rotMat[2][3] = 0;
	rotMat[3][0] = 0;		rotMat[3][1] = 0;		rotMat[3][2] = 0;		rotMat[3][3] = 1;
	return rotMat;
}
glm::quat QuatCamera::matrix4ToQuaternion(glm::mat4 m) {
	const GLfloat fourXSquaredMinus1 = m[0][0] - m[1][1] - m[2][2];
	const GLfloat fourYSquaredMinus1 = m[1][1] - m[0][0] - m[2][2];
	const GLfloat fourZSquaredMinus1 = m[2][2] - m[0][0] - m[1][1];
	const GLfloat fourWSquaredMinus1 = m[0][0] + m[1][1] + m[2][2];

	GLuint biggestIndex = 0;
	GLfloat fourBiggestSquaredMinus1{ fourWSquaredMinus1 };
	if (fourXSquaredMinus1 > fourBiggestSquaredMinus1)
	{
		fourBiggestSquaredMinus1 = fourXSquaredMinus1;
		biggestIndex = 1;
	}
	if (fourYSquaredMinus1 > fourBiggestSquaredMinus1)
	{
		fourBiggestSquaredMinus1 = fourYSquaredMinus1;
		biggestIndex = 2;
	}
	if (fourZSquaredMinus1 > fourBiggestSquaredMinus1)
	{
		fourBiggestSquaredMinus1 = fourZSquaredMinus1;
		biggestIndex = 3;
	}

	const GLfloat biggestVal = glm::sqrt(fourBiggestSquaredMinus1 + 1.0f) * 0.5f;
	const GLfloat mult = 0.25f / biggestVal;

	glm::quat q = glm::quat{ 1,0, 0, 0 };

	switch (biggestIndex)
	{
	case 0:
	{
		q.w = biggestVal;
		q.x = (m[1][2] - m[2][1]) * mult;
		q.y = (m[2][0] - m[0][2]) * mult;
		q.z = (m[0][1] - m[1][0]) * mult;
	}
	break;
	case 1:
	{
		q.w = (m[1][2] - m[2][1]) * mult;
		q.x = biggestVal;
		q.y = (m[0][1] + m[1][0]) * mult;
		q.z = (m[2][0] + m[0][2]) * mult;
	}
	break;
	case 2:
	{
		q.w = (m[2][0] - m[0][2]) * mult;
		q.x = (m[0][1] + m[1][0]) * mult;
		q.y = biggestVal;
		q.z = (m[1][2] + m[2][1]) * mult;
	}
	break;
	case 3:
	{
		q.w = (m[0][1] - m[1][0]) * mult;
		q.x = (m[2][0] + m[0][2]) * mult;
		q.y = (m[1][2] + m[2][1]) * mult;
		q.z = biggestVal;
	}
	break;
	default: // Should never actually get here. Just for sanities sake.
	{
		assert(false && "How did I get here?!");
	}
	break;
	}

	return q;
}
glm::mat4 QuatCamera::quaternionToMatrix4(const glm::quat& q)
{
	glm::mat4 mat = glm::mat4(1.0f);
	const glm::quat a = normalize(q);

	const GLfloat xx = a.x * a.x;
	const GLfloat yy = a.y * a.y;
	const GLfloat zz = a.z * a.z;
	const GLfloat xy = a.x * a.y;
	const GLfloat xz = a.x * a.z;
	const GLfloat yz = a.y * a.z;
	const GLfloat wx = a.w * a.x;
	const GLfloat wy = a.w * a.y;
	const GLfloat wz = a.w * a.z;

	mat[0][0] = 1.0f - 2.0f * (yy + zz);
	mat[0][1] = 2.0f * (xy + wz);
	mat[0][2] = 2.0f * (xz - wy);

	mat[1][0] = 2.0f * (xy - wz);
	mat[1][1] = 1.0f - 2.0f * (xx + zz);
	mat[1][2] = 2.0f * (yz + wx);

	mat[2][0] = 2.0f * (xz + wy);
	mat[2][1] = 2.0f * (yz - wx);
	mat[2][2] = 1.0f - 2.0f * (xx + yy);

	return mat;
}
bool QuatCamera::equals(glm::vec3 a, glm::vec3 b) {
	GLfloat ep = 0.0001f;
	if ((abs(a.x - b.x) < ep) && (abs(a.y - b.y) < ep) && (abs(a.z - b.z) < ep)) {
		return true;
	}
	else return false;
}