#ifndef SPLINEMANAGER_HPP
#define SPLINEMANAGER_HPP
#pragma once
#include <atlas/utils/Geometry.hpp>
#include "Spline.h"
class SplineManager : public atlas::utils::Geometry {
public:
	SplineManager();
	~SplineManager();

	atlas::math::Point getSplinePosition();

private:
	std::vector<Spline> mSplines;
};
#endif