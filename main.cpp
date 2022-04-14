#include "helper/scene.h"
#include "helper/scenerunner.h"
//#include "scenebasic_uniform.h"
//#include "scene_shadowmap.h"
#include "scene_pcf.h"


int main(int argc, char* argv[])
{
	SceneRunner runner("Shader_Basics");

	std::unique_ptr<Scene> scene;

	//scene = std::unique_ptr<Scene>(new SceneBasic_Uniform());
	//scene = std::unique_ptr<Scene>(new Scene_Shadowmap());
	scene = std::unique_ptr<Scene>(new Scene_Pcf());
	return runner.run(*scene);
}