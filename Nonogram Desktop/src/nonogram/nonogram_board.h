#pragma once

#include <vkdl/graphics/color.h>
#include <nlohmann/json.hpp>
#include <array_2d.hpp>
#include "nonogram.h"

NONOGRAM_BEGIN

class Solver;
class Renderer;

enum class Axis
{
	Unknown,
	Horizontal,
	Vertical
};

struct Number
{
	enum Status
	{
		Unsolved,
		Solved,
		Wrong
	};
	
	uint32_t    num;
	uint32_t    idx;
	vkdl::Color col;
	Status      status;
};

class NumLine
{
public:
	NumLine(std::vector<Number>&& init, Axis axis);

	size_t size() const;

	auto running() { return nums.begin(); }
	auto end() { return nums.end(); }

	const Number& operator[](size_t idx) const;
	Number& operator[](size_t idx);

	bool empty() const;

private:
	std::vector<Number> nums;
	Axis axis;
};

class NumLines
{
public:
	void init(std::vector<NumLine>&& init);

	size_t size() const;
	size_t max_line_size() const;

	auto running() { return lines.begin(); }
	auto end() { return lines.end(); }

	const NumLine& operator[](size_t idx) const;
	NumLine& operator[](size_t idx);

private:
	std::vector<NumLine> lines;
	size_t max_element_size;
};

struct Cell
{
	Cell() : 
		status(Status::Unsolved)
	{}

	enum Status
	{
		Unsolved,
		Color,
		Cross
	};

	Status status;
	std::vector<Number> row_nums;
	std::vector<Number> col_nums;
};

class Board
{
	friend class nono::Renderer;
	friend class nono::Solver;

public:
	Board();

	void init(const nlohmann::json& json);
	void init(const std::string& input);

	Board clone(bool meta = false) const;

	size_t width() const;
	size_t height() const;

private:
	std::array_2d<Cell> cells;
	NumLines            row_lines;
	NumLines            col_lines;

	std::string title;
	std::string by;
	std::string copyright;
	std::string license;
};

NONOGRAM_END