#include "helper/scene.h"
#include "helper/scenerunner.h"
//#include "scenebasic_uniform.h"
//#include "scene_soft.h"
//#include "scene_cloud.h"
//#include "woodgrain_scene.h"
//#include "scene_shadowvolume.h"
//#include "scene_disintegration.h"
//#include "scene_paint.h"
//#include "scene_night.h"
#include "scene_hdr.h"


int main(int argc, char* argv[])
{
	SceneRunner runner("Shader_Basics");

	std::unique_ptr<Scene> scene;

	//scene = std::unique_ptr<Scene>(new Scene_Soft());
	//scene = std::unique_ptr<Scene>(new Scene_Cloud());
	//scene = std::unique_ptr<Scene>(new WoodGrain_Scene());
	//scene = std::unique_ptr<Scene>(new Scene_ShadowVolume());
	//scene = std::unique_ptr<Scene>(new Scene_Disintegration());
	//scene = std::unique_ptr<Scene>(new Scene_Paint());
	//scene = std::unique_ptr<Scene>(new Scene_Night());
	scene = std::unique_ptr<Scene>(new Scene_Hdr());
	return runner.run(*scene);
}