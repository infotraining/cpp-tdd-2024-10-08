#ifndef SNAKE_HPP
#define SNAKE_HPP

#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <vector>

struct Point
{
    Point(int x, int y)
        : x{x}
        , y{y}
    {
    }

    int x, y;

    bool operator==(const Point& other) const
    {
        return x == other.x && y == other.y;
    }

    friend std::ostream& operator<<(std::ostream& out, const Point& pt)
    {
        out << "Point{" << pt.x << ", " << pt.y << "}";
        return out;
    }
};

enum class Direction
{
    Up
};

class Snake
{
private:
    std::vector<Point> segments_;

public:
    explicit Snake(Point head)
        : segments_{head}
    {
    }

    Snake(std::initializer_list<Point> segments)
        : segments_{segments}
    {
    }

    const Point& head() const
    {
        return segments_.front();
    }

    const std::vector<Point>& segments() const
    {
        return segments_;
    }

    Direction direction() const
    {
        return Direction::Up;
    }

    bool operator==(const Snake& other) const
    {
        return segments_ == other.segments_;
    }
};

class SnakeGame
{
private:
    int width_;
    int height_;
    Snake snake_;

public:
    SnakeGame(int width, int height)
        : snake_{Point(width_ / 2, height_ / 2)}
        , width_{width}
        , height_{height}
    {
        if (width <= 0 || height <= 0)
            throw std::invalid_argument("Invalid dimensions of the board");
    }

    const Snake& snake() const
    {
        return snake_;
    }
};

#endif