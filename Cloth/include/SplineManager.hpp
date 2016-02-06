#ifndef SPLINEMANAGER_HPP
#define SPLINEMANAGER_HPP
#pragma once
#include <atlas/utils/Geometry.hpp>
#include "Spline.h"
USING_ATLAS_MATH_NS;
USING_ATLAS_GL_NS;
class SplineManager : public atlas::utils::Geometry {
public:
	SplineManager(int totalFrames);
	~SplineManager();

	atlas::math::Point getSplinePosition();
	void addSplines();
	void showSpline();
	void showControlPoints();
	void showCage();
	void showSplinePoints();
	bool doneInterpolation();
	void updateGeometry(atlas::utils::Time const& t) override;
	void renderGeometry(atlas::math::Matrix4 projection,atlas::math::Matrix4 view) override;
private:
	std::vector<Spline> mSplines;
	std::vector<atlas::math::Point> mControlPoints;
	int mTotalFrames;
	int currentSpline;
	bool finishedAllSplines;
};
#endif