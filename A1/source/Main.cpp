#include <atlas/utils/Application.hpp>
#include "SpringScene.hpp"
int main()
{
    // Create a new window (only one per application for now).

	APPLICATION.createWindow(500, 500, "Hootin");
	APPLICATION.addScene(new SpringScene);
	APPLICATION.runApplication();

    return 0;
}