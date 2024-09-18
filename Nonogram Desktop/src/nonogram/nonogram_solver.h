#pragma once

#include "nonogram_board.h"

NONOGRAM_BEGIN

struct Solution
{
	enum Strategy
	{
		Overlapping,
		SimpleBox
	};

	Strategy     strategy;
	Axis         axis;
	uint32_t     axis_idx;
	uint32_t     min;
	uint32_t     max;
	Cell::Status fill;
};

class SolutionGroup
{
public:

	std::vector<Solution> solutions;
};

class Solver
{
public:
	Solver();

	void init(Board& board);

	bool solveStep(Solution& result);

	void apply(const SolutionGroup& result);
	void apply(const Solution& result);

private:
	bool checkContradiction();
	bool simpleBox(Solution& result);

	bool fitLeftMost(std::vector<Cell>& line, const NumLine& numline);
	bool fitRightMost(std::vector<Cell>& line, const NumLine& numline);

private:
	Board* board;

	std::array_2d<Cell> cells_cache;
};

NONOGRAM_END