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
    Point(int x = 0, int y = 0)
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
    Up,
    Down,
    Left,
    Right
};

class Board
{
private:
    int w_;
    int h_;

public:
    Board(int w, int h)
        : w_{w}
        , h_{h}
    {
    }

    bool is_hitting_wall(Point head)
    {
        return head.x == 0 || head.y == 0 || head.x == w_ || head.y == h_;
    }
};

class Snake
{
private:
    std::vector<Point> segments_;
    Direction direction_ = Direction::Up;
    Board* board_{};
    bool is_alive_ = true;

public:
    explicit Snake(Point head)
        : segments_{head}
    {
    }

    Snake(std::initializer_list<Point> segments)
        : segments_{segments}
    {
    }

    void set_board(Board& board)
    {
        board_ = &board;
    }

    bool is_alive() const
    {
        return is_alive_;
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
        return direction_;
    }

    bool operator==(const Snake& other) const
    {
        return segments_ == other.segments_;
    }

    void move(Direction direction)
    {
        direction_ = direction;

        Point new_head = calculate_new_head(direction);

        segments_.insert(segments_.begin(), new_head);
        segments_.pop_back();

        if (board_ && board_->is_hitting_wall(new_head))
        {
            is_alive_ = false;
        }
    }

    friend std::ostream& operator<<(std::ostream& out, const Snake& snake)
    {
        out << "Snake{ ";

        for (const auto& segment : snake.segments_)
            out << segment << " ";

        out << "}";
        return out;
    }

private:
    Point calculate_new_head(Direction direction)
    {
        const Point& old_head = segments_.front();
        Point new_head{};

        switch (direction)
        {
        case Direction::Up:
            new_head = {old_head.x, old_head.y - 1};
            break;
        case Direction::Down:
            new_head = {old_head.x, old_head.y + 1};
            break;
        case Direction::Left:
            new_head = {old_head.x - 1, old_head.y};
            break;
        case Direction::Right:
            new_head = {old_head.x + 1, old_head.y};
            break;

        default:
            break;
        }

        return new_head;
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