#pragma once

#include <vkdl/graphics/font.h>
#include "ui/window_ui.h"
#include "util/time.h"
#include "window/background.h"
#include "config.h"

GAME_BEGIN

class Application
{
public:
	static Application& get();

	Application();

	int run(int argc, const char* argv[]);

	void eventProc(const vkdl::WindowEvent& e);

public:
	vkdl::PlatformWindow platform_window;
	vkdl::Font           title_font;
	vkdl::Font           font;

	Background                background;
	std::unique_ptr<WindowUI> window;

	Time time;
};

GAME_END