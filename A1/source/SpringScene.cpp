#include "SpringScene.hpp"

SpringScene::SpringScene()
{
    // Initialize the matrices to identities.
	mProjection = atlas::math::Matrix4(1.0f);
	mView = atlas::math::Matrix4(1.0f);
}

SpringScene::~SpringScene()
{ }

void SpringScene::renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);
	mSpring.renderGeometry(mProjection, mView);
	//angularSpr.renderGeometry(mProjection, mView);
}

void SpringScene::updateScene(double time) {
	// Handle the timing stuff here.
	mTime.deltaTime = (float)time - mTime.currentTime;
	mTime.totalTime += (float)time;
	mTime.currentTime = (float)time;

	mSpring.updateGeometry(mTime);
	//angularSpr.updateGeometry(mTime);
	
}