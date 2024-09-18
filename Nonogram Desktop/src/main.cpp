#include <vkdl/core/context.h>

#include "config.h"
#include "application.h"

using namespace std;

int main(int argc, const char* argv[])
{
	vk::ApplicationInfo app_info = {};
	app_info.apiVersion = VK_API_VERSION_1_3;
	app_info.pApplicationName = "Nonogram Classic";
	app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.pEngineName = "VKDL";
	app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);

	auto ctx = vkdl::ContextCreator()
		.setApplicationInfo(app_info)
		.setPhysicalDeviceType(vk::PhysicalDeviceType::eDiscreteGpu)
#ifdef _DEBUG
		.enableDebug(5)
#endif
		.create();

	game::Application app;

	return app.run(argc, argv);
}