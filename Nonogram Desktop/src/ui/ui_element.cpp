#include "ui_element.h"

UI_BEGIN

bool UIElement::operator==(const UIElement& rhs) const
{
	return this == std::addressof(rhs);
}

bool UIElement::operator!=(const UIElement& rhs) const
{
	return this != std::addressof(rhs);
}

UI_END