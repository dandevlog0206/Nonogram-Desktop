#include "background.h"

#include <fstream>
#include <random>
#include "../application.h"
#include "../util/time.h"

using namespace ui;
using namespace vkdl;

static uint8_t get_opacity(float scale) 
{
	return (255.99f / 0.8f) * (scale - 0.2f);
}

GAME_BEGIN

Background::Background() :
	active_renderers(0)
{
	std::ifstream file(TITLE_PUZZLES_PATH);
	auto puzzles = nlohmann::json::parse(file)["nonogram"];

	boards.reserve(puzzles.size());

	for (const auto& json : puzzles) {
		auto* board = new nono::Board();
		board->init(json);

		auto& renderer = boards.emplace_back();
		renderer.BoardPtr    = board;
		renderer.OwnBoardPtr = true;
		renderer.Numbers     = false;
		renderer.Isometric   = true;
		renderer.Show        = false;
	}

	for (auto& board : boards) {
		auto floating = std::make_unique<FloatingAnimation>();
		floating->registerGetter([&]() { return board.Position.get().y; });
		floating->registerSetter([&](float value) { board.Position = vec2(board.Position.get().x, value); });
		floating->Amplitude = 15.f;

		auto transition = std::make_unique<TransitionAnimation>();
		transition->registerGetter([&]() { return board.Position.get(); });
		transition->registerSetter([&](const vec2& value) { board.Position = value; });

		floating_animations.apend(std::move(floating));
		transition_animations.apend(std::move(transition));
	}
}

void Background::beginTransition(Direction dir)
{
	vec2 direction;
	float dist;
	float accel;

	switch (dir)
	{
	case Direction::Left:  direction = vec2(-1, 0); dist = 2500; accel = 2000; break;
	case Direction::Right: direction = vec2(1, 0);  dist = 2500; accel = 2000; break;
	case Direction::Up:    direction = vec2(0, -1); dist = 1600; accel = 5000; break;
	case Direction::Down:  direction = vec2(0, 1);  dist = 1600; accel = 2000; break;
	}

	for (auto& animation: transition_animations) {
		auto& transition = static_cast<TransitionAnimation&>(animation);

		if (transition.is_running())
			transition.cancel();

		if (dir == Direction::Unknown) {
			transition.cancel();
			continue;
		}

		transition.Distance        = dist;
		transition.Acceleration    = accel;
		transition.DirectionVector = direction;
		transition.start();
	}
}

bool Background::isTransitionRunning() const
{
	return transition_animations[0].is_running();
}

float Background::getTransitionTime() const
{
	return static_cast<const TransitionAnimation&>(transition_animations[0]).eval_play_time();
}

void Background::update()
{
	for (int i = active_renderers; i < 7; ++i)
		showRandomBoard();

	vec2 fb_size = (uvec2)Application::get().platform_window.FrameBufferSize;

	floating_animations.update(Time::dt());
	transition_animations.update(Time::dt());
}

bool Background::showRandomBoard()
{
	uvec2 fb_size = Application::get().platform_window.FrameBufferSize;

	std::random_device rd;
	std::minstd_rand rnd(rd());
	std::uniform_int_distribution<size_t> dist(0, boards.size() - 1);
	std::uniform_int_distribution<uint32_t> dist_width(0, fb_size.x);
	std::uniform_int_distribution<uint32_t> dist_height(0, fb_size.y);
	std::uniform_real_distribution<float> dist_scale(0.2f, 1.f);
	std::uniform_real_distribution<float> dist_offset(0.f, 2.f * 3.141592);

	if (std::all_of(boards.begin(), boards.end(), [](const auto& board) {
		return board.Show;
	})) return false;

	while (true) {
		auto idx = dist(rnd);

		auto& board      = boards[idx];
		auto& floating   = static_cast<FloatingAnimation&>(floating_animations[idx]);
		auto& transition = static_cast<TransitionAnimation&>(transition_animations[idx]);
		
		if (board.Show) continue;

		float scale = dist_scale(rnd);

		board.Show      = true;
		board.Position  = vec2(dist_width(rnd), dist_height(rnd));
		board.Scale     = vec2(scale, scale);
		board.GridColor = Color(0u, 0u, 0u, get_opacity(scale));

		floating.TimeOffset = dist_offset(rnd);
		floating.Amplitude  = 30.f * scale;
		floating.start();

		transition.Distance     = 500.f * scale;
		transition.Acceleration = 3000.f * scale;

		int i = 0;
		for (; i < board_order.size(); ++i)
			if (scale < boards[board_order[i]].Scale.get().x) break;

		board_order.insert(board_order.begin() + i, idx);

		active_renderers++;
		return true;
	}
}

void Background::draw(RenderTarget& target, RenderStates& states, const RenderOptions& options) const
{
	for (size_t idx : board_order)
		target.render(boards[idx]);
}

GAME_END