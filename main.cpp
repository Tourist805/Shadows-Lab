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
#include "scene_pbr.h"
//#include "scene_hdr.h"
//#include "scene_particles.h"
//#include "scene_transparency.h"
#include "scene_rustMetal.h"
#include "InputController.h"


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
	scene = std::unique_ptr<Scene>(new Scene_Pbr());
	//scene = std::unique_ptr<Scene>(new Scene_Hdr());
	//scene = std::unique_ptr<Scene>(new Scene_Particles());
	/*while (true)
	{
		if (runner.getCurrentSceneIndex() == 2)
		{
			scene = std::unique_ptr<Scene>(new Scene_Hdr());
			runner.run(*scene);
		}
		if (runner.getCurrentSceneIndex() == 1)
		{
			scene = std::unique_ptr<Scene>(new Scene_Particles());
			runner.run(*scene);
		}
		if (runner.getCurrentSceneIndex() == 0)
		{
			scene = std::unique_ptr<Scene>(new Scene_Transparency());
			runner.run(*scene);
		}
	}*/
	//scene = std::unique_ptr<Scene>(new Scene_RustMetal());
	
	
	return runner.run(*scene);
}