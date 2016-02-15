//#ifndef CLOTHCAMERA_HPP
//#define CLOTHCAMERA_HPP
//#pragma once
//#include <atlas/utils/Camera.hpp>
//#include <atlas/utils/Geometry.hpp>
//#include <string>
//
//class ClothCamera : public atlas::utils::Camera {
//public:
//	ClothCamera();
//	~ClothCamera();
//
//	glm::mat4 getCameraMatrix() override;
//	void move(CameraDirection dir);
//	void mouseUpdate(const glm::vec2& nowMousePosition);
//	void newPosition(glm::vec3 newPosition);
//	void lookAt(glm::vec3 target);
//	void roll(GLfloat angle);
//private:
//	glm::vec3 position;
//	glm::vec3 forward, right, left;
//	glm::vec3 upDirection;
//	glm::vec3 target;
//	glm::vec2 oldMousePosition;
//	GLfloat thresholdJump;
//	GLfloat MOVEMENT_SPEED, ROTATION_SPEED, SPIN_SPEED;
//
//	glm::quat orientation;
//
//	glm::vec3 getForward();
//	glm::vec3 getUp();
//	glm::vec3 getRight();
//	// Standard reference axes.
//	glm::vec3 X_AXIS;
//	glm::vec3 Y_AXIS; 
//	glm::vec3 Z_AXIS; 
//
//	void updateVectors();
//
//};
//
//#endif // !CLOTHCAMERA_HPP