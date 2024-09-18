#include "application.h"

#include <vkdl/core/context.h>
#include "window/title_window.h"
#include "window/settings_window.h"
#include "window/game_window.h"

TIME_IMPLEMENTATION

using namespace vkdl;

GAME_BEGIN

static Application* app_instance = nullptr;

Application::Application() :
	platform_window(INITIAL_WIDTH, INITIAL_HEIGHT, APPLICATION_TITLE, PlatformWindowStyle::Default),
	title_font(DEFALUT_FONT_PATH), //"font\\SWITCH STRETCH.ttf"),
	font(DEFALUT_FONT_PATH)
{
	app_instance = this;
	platform_window.ClearColor = Colors::White;
	time.setFps(INITIAL_FPS);
	
	window = std::make_unique<TitleWindow>();
}

Application& Application::get()
{
	return *app_instance;
}

int Application::run(int argc, const char* argv[])
{
	while (!platform_window.IsClosed) {
		time.newFrame();

		WindowEvent e;
		while (platform_window.pollEvent(e)) {
			eventProc(e);
			window->eventProc(e);
		}

		background.update();
		window->update();

		platform_window.render(background);
		platform_window.render(*window);

		platform_window.display();
		vkdl::Context::get().device.waitIdle();
		time.waitForFPS();
	}

	window.reset();

	return 0;
}

void Application::eventProc(const WindowEvent& e)
{
	switch (e.type) {
	case WindowEvent::KeyPressed:
		switch (e.keyboard.key) {
		case Key::Left:   background.beginTransition(Direction::Left); break;
		case Key::Right:  background.beginTransition(Direction::Right); break;
		case Key::Up:     background.beginTransition(Direction::Up); break;
		case Key::Down:   background.beginTransition(Direction::Down); break;
		case Key::RShift: background.beginTransition(Direction::Unknown); break;
		}
		break;
	case WindowEvent::Close:
		platform_window.close();
		break;
	}
}

GAME_END