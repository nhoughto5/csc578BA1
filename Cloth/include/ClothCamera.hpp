#ifndef CLOTHCAMERA_HPP
#define CLOTHCAMERA_HPP
#pragma once
#include <atlas/utils/Camera.hpp>
#include <atlas/utils/Geometry.hpp>
#include <string>
class ClothCamera : public atlas::utils::Camera {
public:
	ClothCamera();
	~ClothCamera();

	glm::mat4 getCameraMatrix() override;
	void strafeCamera(int direction);
	void mouseUpdate(const glm::vec2& nowMousePosition);
	void newPosition(glm::vec3 newPosition);
	void lookAt(glm::vec3 target);
private:
	glm::vec3 position;
	glm::vec3 viewDirection;
	glm::vec3 upDirection, strafeDirection;
	glm::vec3 target;
	glm::vec2 oldMousePosition;
	GLfloat thresholdJump;
	GLfloat MOVEMENT_SPEED, ROTATION_SPEED, SPIN_SPEED;
};

#endif // !CLOTHCAMERA_HPP
