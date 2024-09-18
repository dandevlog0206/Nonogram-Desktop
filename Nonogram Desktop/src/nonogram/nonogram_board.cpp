#include "nonogram_board.h"

#include <sstream>
#include <vkdl/core/builtin_objects.h>
#include <vkdl/core/render_target.h>
#include <vkdl/core/render_options.h>

using namespace vkdl;

NONOGRAM_BEGIN

NumLine::NumLine(std::vector<Number>&& init, Axis axis) :
	nums(std::move(init)),
	axis(axis)
{}

size_t NumLine::size() const
{
	return nums.size();
}

const Number& NumLine::operator[](size_t idx) const
{
	return nums[idx];
}

Number& NumLine::operator[](size_t idx)
{
	return nums[idx];
}

bool NumLine::empty() const
{
	return false;
}

void NumLines::init(std::vector<NumLine>&& init) {
	lines            = std::move(init);
	max_element_size = std::max_element(lines.begin(), lines.end(),
		[](const NumLine& a, const NumLine& b) {
			return a.size() < b.size();
		})->size();
}

size_t NumLines::size() const
{
	return lines.size();
}

size_t NumLines::max_line_size() const
{
	return max_element_size;
}

const NumLine& NumLines::operator[](size_t idx) const
{
	return lines[idx];
}

NumLine& NumLines::operator[](size_t idx)
{
	return lines[idx];
}

Board::Board()
{
}

void Board::init(const nlohmann::json& json)
{
	title     = json["title"];
	by        = json["by"];
	copyright = json["copyright"];
	license   = json["license"];

	size_t width  = json["width"];
	size_t height = json["height"];

	auto parse_numbers = [](const nlohmann::json& json, Axis axis) {
		std::vector<NumLine> number_lines;

		for (const auto& nums : json) {
			std::vector<Number> numbers;
			numbers.reserve(nums.size());
			
			for (const auto& num : nums) {
				Number number;
				number.num    = num;
				number.idx    = numbers.size();
				number.col    = Colors::Black;
				number.status = Number::Unsolved;

				numbers.push_back(number);
			}

			number_lines.emplace_back(std::move(numbers), axis);
		}

		return number_lines;
	};

	row_lines.init(parse_numbers(json["row_numbers"], Axis::Horizontal));
	col_lines.init(parse_numbers(json["column_numbers"], Axis::Vertical));
}

void Board::init(const std::string& input)
{
	//std::istringstream input_ss(input);
	//std::string line;
	//char buf[32] = {};

	//auto axis = Axis::Unknown;

	//while (std::getline(input_ss, line)) {
	//	if (line.empty()) continue;

	//	if (line[0] == 'r') {
	//		axis = Axis::Horizontal;
	//	} else if (line[0] == 'c') {
	//		axis = Axis::Vertical;
	//	} else {
	//		auto& v = axis == Axis::Horizontal ? row_lines : col_lines;

	//		auto& numbers = v.emplace_back();

	//		std::stringstream ss(line);

	//		for (int n, i = 0; ss >> n; ++i) {
	//			NonogramNumber number = {};
	//			number.num = n;
	//			number.idx = i;
	//			number.status = NonogramNumber::Unsolved;

	//			numbers.nums.push_back(number);

	//			if (ss.peek() == ',') ss.ignore();
	//		}
	//	}
	//}
}

Board Board::clone(bool meta) const
{
	return Board();
}

size_t Board::width() const
{
	return col_lines.size();
}

size_t Board::height() const
{
	return row_lines.size();
}

NONOGRAM_END
