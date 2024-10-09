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

CATCH_REGISTER_ENUM(Direction, Direction::Up);

TEST_CASE("Snake", "[Snake][Construction]")
{
    SECTION("constructed with Point")
    {
        Snake snake{Point(10, 5)};

        SECTION("has head at specified location")
        {
            REQUIRE(snake.head() == Point(10, 5));
        }

        SECTION("has one segment")
        {
            REQUIRE(snake.segments() == std::vector{Point(10, 5)});
        }
    }

    SECTION("constructed with segments")
    {
        Snake snake = {Point(10, 5), Point(10, 6), Point(10, 7)};

        SECTION("head is at the position of first segment")
        {
            REQUIRE(snake.head() == Point(10, 5));
        }

        SECTION("has multiple segments")
        {
            REQUIRE(snake.segments() == std::vector{Point(10, 5), Point(10, 6), Point(10, 7)});
        }
    }
}

TEST_CASE("Starting the game", "[SnakeGame][Start]")
{
    SECTION("When game starts")
    {
        SECTION("with valid dimensions of board")
        {
            constexpr int width = 30;
            constexpr int height = 20;

            SnakeGame game(width, height);

            SECTION("the snake is at the center of the board")
            {
                REQUIRE(game.snake() == Snake{Point(15, 10)});
            }

            SECTION("snake's directions is Up")
            {
                REQUIRE(game.snake().direction() == Direction::Up);
            }
        }

        SECTION("with invalid dimensions of the board")
        {
            constexpr int width = 30;
            constexpr int height = 0;

            SECTION("throws invalid argument exception")
            {
                REQUIRE_THROWS_AS(SnakeGame(width, height), std::invalid_argument);
            }
        }
    }
}