#include <atlas/utils/Application.hpp>
#include <atlas/gl/ErrorCheck.hpp>
#include "ClothScene.hpp"

int main()
{
	APPLICATION.createWindow(400, 400, "Cloth_Quaternion_Spline");
	APPLICATION.addScene(new ClothScene);
	APPLICATION.runApplication();

	return 0;
}