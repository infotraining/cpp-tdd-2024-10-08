#include "snake/snake.hpp"

#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/trompeloeil.hpp>
#include <ranges>
#include <trompeloeil.hpp>

using namespace std;

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
    Point head_;

public:
    Snake(Point);

    const Point& head() const
    {
        return head_;
    }

    size_t segments() const
    {
        return 1;
    }

    Direction direction() const
    {
        return Direction::Up;
    }
};

Snake::Snake(Point head)
    : head_{head}
{
}

class SnakeGame
{
private:
    int width_;
    int height_;
    Snake snake_;

public:
    SnakeGame(int width, int height)
        : snake_(Point(width_ / 2, height_ / 2))
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

CATCH_REGISTER_ENUM(Direction, Direction::Up);

TEST_CASE("Snake", "[Snake][Construction]")
{
    SECTION("constructed with Point")
    {
        Snake snake(Point(10, 5));

        SECTION("has head at specified location")
        {
            REQUIRE(snake.head() == Point(10, 5));
        }

        SECTION("has one segment")
        {
            REQUIRE(snake.segments() == 1);
        }
    }
}

TEST_CASE("Starting the game", "[SnakeGame][Start]")
{
    SECTION("When game starts with valid dimensions of board")
    {
        constexpr int width = 30;
        constexpr int height = 20;

        SnakeGame game(width, height);

        SECTION("the snake is at the center of the board")
        {
            REQUIRE(game.snake().head() == Point(15, 10));
        }

        SECTION("the snake has one segment")
        {
            REQUIRE(game.snake().segments() == 1);
        }

        SECTION("snake's directions is Up")
        {
            REQUIRE(game.snake().direction() == Direction::Up);
        }
    }

    SECTION("When game starts with invalid dimensions")
    {
        constexpr int width = 30;
        constexpr int height = 0;

        SECTION("throws invalid argument")
        {
            REQUIRE_THROWS_AS(SnakeGame(width, height), std::invalid_argument);
        }
    }
}