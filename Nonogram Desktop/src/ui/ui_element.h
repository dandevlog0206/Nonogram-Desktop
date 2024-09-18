#pragma once

#include <string>
#include <vkdl/math/vector_type.h>
#include <vkdl/graphics/drawlist_2d.h>
#include <vkdl/util/property.hpp>
#include <vkdl/util/event.hpp>
#include <vkdl/system/window_event.h>
#include "../config.h"

UI_BEGIN

class UIElement
{
	PROPERTY_INIT(UIElement);

public:
	PROPERTY{
		PROPERTY_DEFAULT_GET_SET(std::string, Name);
		PROPERTY_DEFAULT_GET_SET(vkdl::vec2, Position);
		PROPERTY_DEFAULT_GET_SET(vkdl::vec2, Size);
		PROPERTY_DEFAULT_GET_SET(vkdl::vec2, Origin);
	};

	virtual void eventProc(const vkdl::WindowEvent& e) = 0;
	virtual void draw(const vkdl::RenderTarget& target, vkdl::DrawList2D& drawlist) const = 0;

	bool operator==(const UIElement& rhs) const;
	bool operator!=(const UIElement& rhs) const;
};

UI_END