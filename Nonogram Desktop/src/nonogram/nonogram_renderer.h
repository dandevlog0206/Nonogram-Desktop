#pragma once

#include <vkdl/graphics/drawlist_2d.h>
#include <vkdl/graphics/transformable_2d.h>
#include "nonogram_board.h"

NONOGRAM_BEGIN

class Renderer : public vkdl::Drawable, public vkdl::Transformable2D
{
	PROPERTY_INIT(Renderer);

public:
	Renderer();
	~Renderer();

	PROPERTY {
		PROPERTY_DEFAULT_GET_SET(const Board*, BoardPtr);
		PROPERTY_DEFAULT_GET_SET(bool, OwnBoardPtr);
		PROPERTY_DEFAULT_GET_SET(bool, Numbers);
		PROPERTY_DEFAULT_GET_SET(bool, Isometric);
		PROPERTY_DEFAULT_GET_SET(bool, Show);
		PROPERTY_DEFAULT_GET_SET(vkdl::Color, BackgroundColor);
		PROPERTY_DEFAULT_GET_SET(vkdl::Color, GridColor);
		PROPERTY_DEFAULT_GET_SET(vkdl::Color, NumberColor);
	};

private:
	void draw(vkdl::RenderTarget& target, vkdl::RenderStates& states, const vkdl::RenderOptions& options) const override;

private:
	mutable vkdl::DrawList2D drawlist;
};

NONOGRAM_END