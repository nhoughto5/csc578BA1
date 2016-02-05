#ifndef LAB01_INCLUDE_SIMPLE_GL_SCENE_HPP
#define LAB01_INCLUDE_SIMPLE_GL_SCENE_HPP

#pragma once

#include <atlas/utils/Scene.hpp>
#include "Spring.hpp"
#include "angularSpring.hpp"

class SpringScene : public atlas::utils::Scene {

public:
	SpringScene();
	~SpringScene();

	void renderScene() override;
	void updateScene(double time) override;

private:
	Spring mSpring;
	angularSpring angularSpr;
	GLfloat force, displacement;
};


#endif