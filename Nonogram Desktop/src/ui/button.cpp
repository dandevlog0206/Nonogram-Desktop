#include "button.h"

#include "../application.h"

using namespace vkdl;

UI_BEGIN

Button::Button() :
	AnimationOnButtonHovered(&trriger_on_hovered),
	AnimationOnButtonPressed(&trriger_on_pressed),
	AnimationOnButtonReleased(&trriger_on_released),
	hovered(false)
{
}

Button::~Button()
{
}

PROPERTY_IMPL_GET(Button, Hovered)
{
	return hovered;
}

void Button::eventProc(const WindowEvent& e)
{
	rect bound((vec2)Position - (vec2)Origin, Size);

	switch (e.type) {
	case WindowEvent::MouseMoved: {
		auto pos = e.mouseMove.pos;
		if (bound.contain(pos)) {
			EventArgs args;
			hovered = true;
			trriger_on_hovered.start();
			OnButtonHovered.invoke(*this, args);
		} else {
			hovered = false;
		}
	} break;
	case WindowEvent::MousePressed: {
		auto pos = e.mouseButton.pos;
		if (bound.contain(pos)) {
			EventArgs args;
			trriger_on_pressed.start();
			OnButtonClicked.invoke(*this, args);
		}
	} break;
	case WindowEvent::MouseReleased: {
		auto pos = e.mouseButton.pos;
		if (bound.contain(pos)) {
			trriger_on_released.start();
		}
	} break;
	}
}

void Button::draw(const RenderTarget& target, DrawList2D& drawlist) const
{
	static const Color transparent_white(1.f, 1.f, 1.f, 0.2f);

	TextStyle style;
	style.font           = &game::Application::get().font;
	style.character_size = 50;
	style.fill_color     = TextColor;
	style.align_h        = 0.5f;
	style.align_v        = 0.5f;

	drawlist.addFilledRoundRect((vec2)Position - (vec2)Origin, Size, (vec4)Round, BackgroundColor);
	if (hovered)
		drawlist.addFilledRoundRect((vec2)Position - (vec2)Origin, Size, (vec4)Round, transparent_white);
	drawlist.addText((vec2)Position, Text, style);
}

UI_END
