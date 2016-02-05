#include "ClothScene.hpp"
#include <atlas/core/GLFW.hpp>
#include <atlas/core/Log.hpp>
#include <atlas/core/Macros.hpp>
#include <atlas/core/Float.hpp>

ClothScene::ClothScene() : 
	mIsPlaying(false),
	mLastTime(0.0f),
	mFPS(60.0f),
	mTick(1.0f / mFPS),
	mAnimTime(0.0f),
	mAnimLength(10.0f),
	mSpline(int(mAnimLength * mFPS))
{
	glEnable(GL_DEPTH_TEST);
}

ClothScene::~ClothScene() {

}

void ClothScene::mousePressEvent(int button, int action, int modifiers, double xPos, double yPos) {
	USING_ATLAS_MATH_NS;

	if (button == GLFW_MOUSE_BUTTON_LEFT && modifiers == GLFW_MOD_ALT)
	{
		if (action == GLFW_PRESS)
		{
			mIsDragging = true;
			//Camera tilt up and down or turn left, right
			mCamera.mouseDown(Point2(xPos, yPos), ClothCamera::CameraMovements::TUMBLE);
		}
		else
		{
			mIsDragging = false;
			mCamera.mouseUp();
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE && modifiers == GLFW_MOD_ALT)
	{
		if (action == GLFW_PRESS)
		{
			mIsDragging = true;
			//Camera move left, right, up, down
			mCamera.mouseDown(Point2(xPos, yPos), ClothCamera::CameraMovements::TRACK);
		}
		else
		{
			mIsDragging = false;
			mCamera.mouseUp();
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && modifiers == GLFW_MOD_ALT)
	{
		if (action == GLFW_PRESS)
		{
			// first click.
			mIsDragging = true;
			//Camera move back and forth
			mCamera.mouseDown(Point2(xPos, yPos), ClothCamera::CameraMovements::DOLLY);
		}
		else
		{
			mIsDragging = false;
			mCamera.mouseUp();
		}
	}
	else if (action != GLFW_PRESS)
	{
		mIsDragging = false;
		mCamera.mouseUp();
	}
}

void ClothScene::mouseMoveEvent(double xPos, double yPos) {
	//if (mIsDragging)
	//{
	//	mCamera.mouseDrag(atlas::math::Point2(xPos, yPos));
	//}
	mCamera.mouseUpdate(glm::vec2(xPos, yPos));
}
void ClothScene::keyPressEvent(int key, int scancode, int action, int mods) {
	UNUSED(scancode);
	UNUSED(mods);

	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_T:
			mCamera.resetCamera();
			break;
		case GLFW_KEY_W:
			mCamera.strafeCamera(0);
			break;
		case GLFW_KEY_S:
			mCamera.strafeCamera(1);
			break;
		case GLFW_KEY_A:
			mCamera.strafeCamera(2);
			break;
		case GLFW_KEY_D:
			mCamera.strafeCamera(3);
			break;
		case GLFW_KEY_R:
			mCamera.strafeCamera(4);
			break;
		case GLFW_KEY_F:
			mCamera.strafeCamera(5);
			break;
		case GLFW_KEY_Q:
			mCamera.strafeCamera(6);
			break;
		case GLFW_KEY_E:
			mCamera.strafeCamera(7);
			break;
		case GLFW_KEY_C:
			mCamera.newPosition(glm::vec3(0.0f, 3.0f, 0.0f));
			break;
		case GLFW_KEY_U:
			mSpline.showSpline();
			break;
		case GLFW_KEY_I:
			mSpline.showControlPoints();
			break;
		case GLFW_KEY_O:
			mSpline.showCage();
			break;
		case GLFW_KEY_P:
			mSpline.showSplinePoints();
			break;
		case GLFW_KEY_SPACE:
			mIsPlaying = !mIsPlaying;
		default:
			break;
		}
	}
}

void ClothScene::screenResizeEvent(int width, int height) {
	glViewport(0, 0, width, height);
	mProjection = glm::perspective(glm::radians(45.0),(double)width / height, 1.0, 1000.0);
}
void ClothScene::renderScene() {
	float grey = 161.0f / 255.0f;
	glClearColor(grey, grey, grey, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	mView = mCamera.getCameraMatrix();
	mGrid.renderGeometry(mProjection, mView);
	mSpline.renderGeometry(mProjection, mView);
	mBall.renderGeometry(mProjection, mView);
}
void ClothScene::updateScene(double time)
{
	mTime.currentTime = (float)time;
	mTime.totalTime += (float)time;

	if (atlas::core::geq(mTime.currentTime - mLastTime, mTick))
	{
		mLastTime += mTick;
		mTime.deltaTime = mTick;

		if (mIsPlaying)
		{
			mAnimTime += mTick;
			mSpline.updateGeometry(mTime);

			if (mSpline.doneInterpolation())
			{
				mIsPlaying = false;
				return;
			}

			auto point = mSpline.getSplinePosition();
			mCamera.newPosition(point);
			mCamera.lookAt(glm::vec3{ 0.0f, 0.0f, 0.0f });
			auto mat = glm::translate(atlas::math::Matrix4(1.0f), glm::vec3{ 0.0f, 0.0f, 0.0f });
			mBall.transformGeometry(mat);
		}

	}
}
