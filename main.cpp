#include "helper/scene.h"
#include "helper/scenerunner.h"
//#include "scene_soft.h"
//#include "scene_paint.h"
//#include "scene_night.h"
//#include "scene_pbr.h"
//#include "scene_particles.h"
//#include "scene_transparency.h"
#include "scene_rustMetal.h"


int main(int argc, char* argv[])
{
	SceneRunner runner("Shader_Basics");

	std::unique_ptr<Scene> scene;

	//scene = std::unique_ptr<Scene>(new Scene_Pbr());
	//scene = std::unique_ptr<Scene>(new Scene_Particles());
	//scene = std::unique_ptr<Scene>(new Scene_Transparency());
	scene = std::unique_ptr<Scene>(new Scene_RustMetal());
	
	return runner.run(*scene);
}