#pragma once

#include "../ui/window_ui.h"
#include "../ui/button.h"
#include "../animation/animation.h"

GAME_BEGIN

class TitleWindow : public WindowUI
{
public:
	TitleWindow();

	void update() override;
	void eventProc(const vkdl::WindowEvent& e) override;

	void onTitleButtonClicked(EventSender& sender, EventArgs& args);

private:
	void draw(vkdl::RenderTarget& target, vkdl::RenderStates& states, const vkdl::RenderOptions& options) const override;

private:
	ui::Button play_button;
	ui::Button settings_button;
	ui::Button exit_button;

	ui::FadingAnimation fading_animation;

	bool need_close;

	mutable vkdl::DrawList2D drawlist;
};

GAME_END