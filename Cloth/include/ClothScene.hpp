#ifndef SCENE_HPP
#define SCENE_HPP
#pragma once

#include <atlas\utils\Scene.hpp>
#include "ClothCamera.hpp"
#include "Grid.hpp"
#include "Spline.h"
#include "Ball.hpp"
//#include "Cloth.hpp"
class ClothScene : public atlas::utils::Scene {
public:
	ClothScene();
	~ClothScene();

	void mousePressEvent(int button, int action, int modifiers,	double xPos, double yPos) override;
	void mouseMoveEvent(double xPos, double yPos) override;
	void keyPressEvent(int key, int scancode, int action, int mods) override;
	void screenResizeEvent(int width, int height) override;
	void renderScene() override;
	void updateScene(double time) override;
private:
	bool mIsDragging;
	bool mIsPlaying;

	float mLastTime;
	float mFPS;
	float mTick;

	float mAnimTime;
	float mAnimLength;

	ClothCamera mCamera;
	Grid mGrid;
	Spline mSpline;
	Ball mBall;
};

#endif