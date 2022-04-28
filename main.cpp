#include "helper/scene.h"
#include "helper/scenerunner.h"
//#include "scenebasic_uniform.h"
//#include "scene_soft.h"
//#include "scene_cloud.h"
#include "woodgrain_scene.h"


int main(int argc, char* argv[])
{
	SceneRunner runner("Shader_Basics");

	std::unique_ptr<Scene> scene;

	//scene = std::unique_ptr<Scene>(new Scene_Soft());
	//scene = std::unique_ptr<Scene>(new Scene_Cloud());
	scene = std::unique_ptr<Scene>(new WoodGrain_Scene());
	return runner.run(*scene);
}