#pragma once

#include "../ui/window_ui.h"

GAME_BEGIN

class GameWindow : public WindowUI
{
public:
	GameWindow();

	void update() override;
	void eventProc(const vkdl::WindowEvent& e) override;

private:
	void draw(vkdl::RenderTarget& target, vkdl::RenderStates& states, const vkdl::RenderOptions& options) const override;

};

GAME_END