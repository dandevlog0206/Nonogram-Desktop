#include "title_window.h"

#include "../application.h"

using namespace vkdl;

GAME_BEGIN

TitleWindow::TitleWindow() :
	need_close(false)
{
	play_button.Name            = "play button";
	play_button.Position        = vec2(INITIAL_WIDTH / 2.f, 760);
	play_button.Size            = vec2(400, 100);
	play_button.Origin          = vec2(200, 50);
	play_button.Text            = "Play";
	play_button.Round           = vec4(20.f);
	play_button.BackgroundColor = Colors::CadetBlue;
	play_button.TextColor       = Colors::White;
	play_button.OnButtonClicked += make_delegate(this, &TitleWindow::onTitleButtonClicked);

	settings_button.Name            = "settings button";
	settings_button.Position        = vec2(INITIAL_WIDTH / 2.f, 880);
	settings_button.Size            = vec2(400, 100);
	settings_button.Origin          = vec2(200, 50);
	settings_button.Text            = "Settings";
	settings_button.Round           = vec4(20.f);
	settings_button.BackgroundColor = Colors::CadetBlue;
	settings_button.TextColor       = Colors::White;
	settings_button.OnButtonClicked += make_delegate(this, &TitleWindow::onTitleButtonClicked);

	exit_button.Name            = "exit button";
	exit_button.Position        = vec2(INITIAL_WIDTH / 2.f, 1000);
	exit_button.Size            = vec2(400, 100);
	exit_button.Origin          = vec2(200, 50);
	exit_button.Text            = "Exit";
	exit_button.Round           = vec4(20.f);
	exit_button.BackgroundColor = Colors::CadetBlue;
	exit_button.TextColor       = Colors::White;
	exit_button.OnButtonClicked += make_delegate(this, &TitleWindow::onTitleButtonClicked);
}

void TitleWindow::update()
{
	auto& app = Application::get();

	if (need_close && !app.background.isTransitionRunning()) {
		app.platform_window.close();
	}

	fading_animation.update(Time::dt());
}

void TitleWindow::eventProc(const WindowEvent& e)
{
	play_button.eventProc(e);
	settings_button.eventProc(e);
	exit_button.eventProc(e);
}

void TitleWindow::onTitleButtonClicked(EventSender& sender, EventArgs& args)
{
	auto& app    = Application::get();
	auto& button = static_cast<ui::Button&>(sender);

	if (button == play_button) {
		app.background.beginTransition(Direction::Down);
	} else if (button == settings_button) {
		app.background.beginTransition(Direction::Right);
	} else if (button == exit_button) {
		app.background.beginTransition(Direction::Up);
		need_close = true;

		fading_animation.FadeEffect = ui::FadingAnimation::Increase;
		fading_animation.MinValue   = 0.f;
		fading_animation.MaxValue   = 1.f;
		fading_animation.TotalTime  = app.background.getTransitionTime() + 0.2f;
		fading_animation.start();
	}
}

void TitleWindow::draw(vkdl::RenderTarget& target, vkdl::RenderStates& states, const vkdl::RenderOptions& options) const
{
	auto fb_size = target.getFrameBufferSize();

	drawlist.clear();

	TextStyle style;
	style.font           = &game::Application::get().title_font;
	style.character_size = 100;
	style.fill_color     = Colors::Black;
	style.align_h        = 0.5f;
	style.align_v        = 0.5f;
	style.bold           = true;

	drawlist.addText(vec2(fb_size.x / 2.f, 300), "Sample Title", style);

	play_button.draw(target, drawlist);
	settings_button.draw(target, drawlist);
	exit_button.draw(target, drawlist);

	if (fading_animation.is_running())
		drawlist.addFilledRect(vec2(0.f, 0.f), (vec2)fb_size, Color(0.f, 0.f, 0.f, fading_animation.value()));
	
	target.render(drawlist);
}

GAME_END