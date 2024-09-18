#pragma once

#include <vkdl/core/drawable.h>
#include "ui_element.h"
#include "../animation/animation_handler.h"

UI_BEGIN

class Button : public UIElement, public EventSender
{
	PROPERTY_INIT(Button);

public:
	Button();
	~Button();

	PROPERTY {
		PROPERTY_DEFAULT_GET_SET(std::string, Text);
		PROPERTY_DEFAULT_GET_SET(vkdl::vec4, Round);
		PROPERTY_DEFAULT_GET_SET(vkdl::Color, BackgroundColor);
		PROPERTY_DEFAULT_GET_SET(vkdl::Color, TextColor);
		PROPERTY_GET(bool, Hovered);
	};

	AnimationHandler AnimationOnButtonHovered;
	AnimationHandler AnimationOnButtonPressed;
	AnimationHandler AnimationOnButtonReleased;

	EventHandler OnButtonHovered;
	EventHandler OnButtonClicked;

	void eventProc(const vkdl::WindowEvent& e) override;
	void draw(const vkdl::RenderTarget& target, vkdl::DrawList2D& drawlist) const override;

private:
	PROPERTY_DECL_GET(Hovered);

protected:
	AnimationTrigger trriger_on_hovered;
	AnimationTrigger trriger_on_pressed;
	AnimationTrigger trriger_on_released;

	bool hovered;
};

UI_END