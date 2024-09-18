#pragma once

enum class Direction
{
	Unknown,
	Left,
	Right,
	Up,
	Down
};

static Direction rotateCW(Direction dir)
{
	switch (dir)
	{
	case Direction::Unknown: return Direction::Unknown;
	case Direction::Left:	 return Direction::Up;
	case Direction::Right:	 return Direction::Down;
	case Direction::Up:		 return Direction::Right;
	case Direction::Down:	 return Direction::Left;
	}
}

static Direction rotateCCW(Direction dir)
{
	switch (dir)
	{
	case Direction::Unknown: return Direction::Unknown;
	case Direction::Left:	 return Direction::Down;
	case Direction::Right:	 return Direction::Up;
	case Direction::Up:		 return Direction::Left;
	case Direction::Down:	 return Direction::Right;
	}
}