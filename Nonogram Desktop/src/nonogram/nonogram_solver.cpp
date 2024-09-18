#include "nonogram_solver.h"

NONOGRAM_BEGIN

Solver::Solver()
{
}

void Solver::init(Board& board)
{

	//cells_cache.resize(board.height(), board.width());
}

bool Solver::solveStep(Solution& result)
{
	
	return false;
}

void Solver::apply(const SolutionGroup& result)
{
	for (const auto& solution : result.solutions)
		apply(solution);
}

void Solver::apply(const Solution& result)
{
	if (result.axis == Axis::Horizontal) {
		for (uint32_t i = result.min; i <= result.max; ++i)
			board->cells[result.axis_idx][i].status = result.fill;
	} else /* result.axis == Axis::Vertical */ {
		for (uint32_t i = result.min; i <= result.max; ++i)
			board->cells[i][result.axis_idx].status = result.fill;
	}
}

bool Solver::checkContradiction()
{
	std::vector<Cell> cache;

	cache.resize(board->row_lines.size());
	for (size_t r = 0; r < board->height(); ++r) {
		std::copy(board->cells.hbegin(r), board->cells.hend(r), cache.begin());
		if (fitLeftMost(cache, board->row_lines[r])) return false;
	}

	cache.resize(board->col_lines.size());
	for (size_t c = 0; c < board->width(); ++c) {
		std::copy(board->cells.vbegin(c), board->cells.vend(c), cache.begin());
		if (fitLeftMost(cache, board->col_lines[c])) return false;
	}

	return false;
}

bool Solver::simpleBox(Solution& result)
{
	return false;
}

bool Solver::fitLeftMost(std::vector<Cell>& line, const NumLine& numline)
{


	return false;
}

bool Solver::fitRightMost(std::vector<Cell>& line, const NumLine& numline)
{
	return false;
}

NONOGRAM_END