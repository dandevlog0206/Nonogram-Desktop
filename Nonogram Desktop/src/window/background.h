#pragma once

#include <queue>
#include <vkdl/core/drawable.h>
#include <vkdl/graphics/drawlist_2d.h>
#include "../nonogram/nonogram_renderer.h"
#include "../nonogram/nonogram_solver.h"
#include "../animation/animation_group.h"
#include "../util/direction.h"

GAME_BEGIN

class Background : public vkdl::Drawable
{
public:
	Background();

	void beginTransition(Direction dir);
	bool isTransitionRunning() const;
	float getTransitionTime() const;

	void update();

	bool showRandomBoard();

private:
	void draw(vkdl::RenderTarget& target, vkdl::RenderStates& states, const vkdl::RenderOptions& options) const override;

private:
	std::vector<size_t>         board_order;
	std::vector<nono::Renderer> boards;
	ui::AnimationGroup          floating_animations;
	ui::AnimationGroup          transition_animations;
	nono::Solver                solver;
	size_t                      active_renderers;
};

GAME_END