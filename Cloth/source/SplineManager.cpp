#include "SplineManager.hpp"

SplineManager::SplineManager(int totalFrames) :
	finishedAllSplines(false),
	currentSpline(0)
{
	mTotalFrames = totalFrames;
	addSplines();
}
SplineManager::~SplineManager() {

}
void SplineManager::addSplines() {
	mControlPoints = std::vector<Point>
	{
		{ -20, -5, 0 },
		{ -19, 5, -15 },
		{ 12.7f, -5, -1.4f },
		{ 20, 8.2f, 4.4f }
	};
	mSplines.push_back(Spline(mTotalFrames, mControlPoints));
}
void SplineManager::renderGeometry(atlas::math::Matrix4 projection, atlas::math::Matrix4 view) {
	for (int i = 0; i < mSplines.size(); ++i) {
		mSplines[i].renderGeometry(projection, view);
	}
}
void SplineManager::updateGeometry(atlas::utils::Time const& t) {
	mSplines[currentSpline].updateGeometry(t);
	if (mSplines[currentSpline].doneInterpolation()) {
		++currentSpline;
		if (currentSpline == mSplines.size()) {
			finishedAllSplines = true;
		}
	}
}

atlas::math::Point SplineManager::getSplinePosition() {
	return mSplines[currentSpline].getSplinePosition();
}
void SplineManager::showSpline() {
	for (int i = 0; i < mSplines.size(); ++i) {
		mSplines[i].showSpline();
	}
	
}
void SplineManager::showControlPoints() {
	for (int i = 0; i < mSplines.size(); ++i) {
		mSplines[i].showControlPoints();
	}
}
void SplineManager::showCage() {
	for (int i = 0; i < mSplines.size(); ++i) {
		mSplines[i].showCage();
	}
}
void SplineManager::showSplinePoints() {
	for (int i = 0; i < mSplines.size(); ++i) {
		mSplines[i].showSplinePoints();
	}
}
bool SplineManager::doneInterpolation() {
	return finishedAllSplines;
}