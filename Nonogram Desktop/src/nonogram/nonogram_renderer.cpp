#include "nonogram_renderer.h"

#include "../application.h"

using namespace vkdl;

NONOGRAM_BEGIN

Renderer::Renderer() :
	BoardPtr(nullptr),
	OwnBoardPtr(false),
	Numbers(true),
	Isometric(false),
	Show(true),
	BackgroundColor(Colors::White),
	GridColor(Colors::Black),
	NumberColor(Colors::Black)
{}

Renderer::~Renderer()
{
	if (OwnBoardPtr)
		delete BoardPtr;
}

void Renderer::draw(vkdl::RenderTarget& target, vkdl::RenderStates& states, const vkdl::RenderOptions& options) const
{
	if (!Show) return;

	static const auto isometric_transform = Transform2D()
		.rotate(to_radian(30))
		.shearX(to_radian(-30))
		.scale(1.f, 0.5f * sqrtf(3.f));

	const vkdl::vec2 offset(0, 0);
	const float grid_size = 20;

	const auto& board = *BoardPtr.get();

	const size_t   row_line_size     = board.row_lines.size();
	const size_t   col_line_size     = board.col_lines.size();
	const uint32_t max_row_num_count = board.row_lines.max_line_size();
	const uint32_t max_col_num_count = board.col_lines.max_line_size();
	const float    row_line_px       = grid_size * row_line_size;
	const float    col_line_px       = grid_size * col_line_size;
	
	const vec2 bound = grid_size * (vec2(col_line_size, row_line_size) + (Numbers ? vec2(max_row_num_count, max_col_num_count) : vec2()));

	drawlist.clear();
	if (Isometric) {
		drawlist.pushTransform(
			getTransform()
			.rotate(to_radian(30))
			.shearX(to_radian(-30))
			.scale(1.f, 0.5f * sqrtf(3.f))
			.translate(-0.5f * bound));
	} else {
		drawlist.pushTransform(
			getTransform()
			.translate(-0.5f * bound));
	}

	if (BackgroundColor.get().a != 0) {
		vec2 v0(0.f, 0.f);
		vec2 v1(bound.x, 0.f);
		vec2 v2(bound.x, bound.y);
		vec2 v3(0.f, bound.y);

		drawlist.addFilledTriangle(v0, v1, v2, BackgroundColor);
		drawlist.addFilledTriangle(v0, v2, v3, BackgroundColor);
	}

	if (Numbers) {
		for (int i = 0; i < col_line_size + 1; ++i) {
			vec2 p0((i + max_row_num_count) * grid_size, 0);
			vec2 p1((i + max_row_num_count) * grid_size, row_line_px + max_col_num_count * grid_size);

			drawlist.addLine(offset + p0, offset + p1, i % 5 == 0 ? 3.f : 1.f, GridColor);
		}

		for (int i = 0; i < row_line_size + 1; ++i) {
			vec2 p0(0, (i + max_col_num_count) * grid_size);
			vec2 p1(col_line_px + max_row_num_count * grid_size, (i + max_col_num_count) * grid_size);

			drawlist.addLine(offset + p0, offset + p1, i % 5 == 0 ? 3.f : 1.f, GridColor);
		}

		TextStyle style;
		style.font                  = &game::Application::get().font;
		style.character_size        = (int)(grid_size * 0.9f);
		style.align_h               = 0.5f;
		style.align_v               = 0.5f;
		style.fill_color            = NumberColor;
		style.letter_spacing_factor = 0.2f; 

		for (int i = 0; i < col_line_size; ++i) {
			const auto& col_line = board.col_lines[i];

			for (int n = 0; n < col_line.size(); ++n) {
				vec2 pos(offset + grid_size * vec2(i + max_row_num_count + 0.5f, max_col_num_count - n - 0.5f));
				//drawlist.addDot(pos, 20, vkdl::Colors::Red);
				drawlist.addText(pos, std::to_string(col_line[n].num), style);
			}
		}

		for (int i = 0; i < row_line_size; ++i) {
			const auto& row_line = board.row_lines[i];
			
			for (int n = 0; n < row_line.size(); ++n) {
				vec2 pos(offset + grid_size * vec2(max_row_num_count - n - 0.5f, max_col_num_count + i + 0.5f));
				//drawlist.addDot(pos, 20, Colors::Red);
				drawlist.addText(pos, std::to_string(row_line[n].num), style);
			}
		}
	} else {
		for (int i = 0; i < col_line_size + 1; ++i) {
			vec2 p0(i * grid_size, 0);
			vec2 p1(i * grid_size, row_line_px);

			drawlist.addLine(offset + p0, offset + p1, i % 5 == 0 ? 3.f : 1.f, GridColor);
		}

		for (int i = 0; i < row_line_size + 1; ++i) {
			vec2 p0(0, i * grid_size);
			vec2 p1(col_line_px, i * grid_size);

			drawlist.addLine(offset + p0, offset + p1, i % 5 == 0 ? 3.f : 1.f, GridColor);
		}
	}

	target.render(drawlist, {});
}

NONOGRAM_END