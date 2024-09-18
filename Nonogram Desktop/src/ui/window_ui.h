#pragma once

#include <vkdl/core/drawable.h>
#include <vkdl/system/platform_window.h>
#include "../config.h"

GAME_BEGIN

class WindowUI abstract : public vkdl::Drawable
{
public:
	virtual ~WindowUI() {};

	virtual void update() = 0;
	virtual void eventProc(const vkdl::WindowEvent& e) = 0;
};

GAME_END