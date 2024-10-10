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
    std::vector<Point> fruits_;

public:
    Board(int w, int h)
        : w_{w}
        , h_{h}
    {
        if (w <= 0 || h <= 0)
            throw std::invalid_argument("Invalid dimensions of the board");
    }

    int width() const
    {
        return w_;
    }

    int height() const
    {
        return h_;
    }

    bool is_hitting_wall(Point head)
    {
        return head.x == 0 || head.y == 0 || head.x == w_ || head.y == h_;
    }

    void add_fruit(Point point)
    {
        fruits_.push_back(point);
    }

    const std::vector<Point>& fruits()
    {
        return fruits_;
    }

    [[nodiscard]] bool try_eat_fruit(Point fruit)
    {
        auto it = std::find(fruits_.begin(), fruits_.end(), fruit);
        if (it != fruits_.end())
        {
            fruits_.erase(it);
            return true;
        }
        return false;
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

        if (board_ && !board_->try_eat_fruit(new_head))
            segments_.pop_back();

        if (is_eating_itself(new_head) || (board_ && board_->is_hitting_wall(new_head)))
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

    bool is_eating_itself(Point new_head)
    {
        return std::count(segments_.begin(), segments_.end(), new_head) > 1;
    }
};

class Terminal
{
public:
    enum class Key
    {
        Up, Down, Left, Right, Ctrl_Q
    };

    virtual void render_snake(const Snake& snake) = 0;
    virtual void render_fruits(const std::vector<Point>& fruits) = 0;
    virtual Key read_key() = 0;
    virtual ~Terminal() { }
};

class SnakeGame
{
private:
    Board board_;
    Snake snake_;
    Terminal* terminal_;

public:
    SnakeGame(int width, int height)
        : board_{width, height}
        , snake_{Point(board_.width() / 2, board_.height() / 2)}
    {
        snake_.set_board(board_);
    }

    SnakeGame(Board board)
        : board_{std::move(board)}
        , snake_{Point(board_.width() / 2, board_.height() / 2)}
    {
        snake_.set_board(board_);
    }

    void set_terminal(Terminal& terminal)
    {
        terminal_ = &terminal;
    }

    const Snake& snake() const
    {
        return snake_;
    }

    void run()
    {
        assert(terminal_);

        while (true)
        {
            terminal_->render_snake(snake_);
            terminal_->render_fruits(board_.fruits());

            Terminal::Key key = terminal_->read_key();

            switch(key)
            {
                case Terminal::Key::Left: 
                    snake_.move(Direction::Left);
                    break;
                case Terminal::Key::Right: 
                    snake_.move(Direction::Right);
                    break;
                case Terminal::Key::Up:
                    snake_.move(Direction::Up);
                    break;
                case Terminal::Key::Down:
                    snake_.move(Direction::Down);
                    break;
                case Terminal::Key::Ctrl_Q:
                    return;                
            }
        }
    }
};

#endif