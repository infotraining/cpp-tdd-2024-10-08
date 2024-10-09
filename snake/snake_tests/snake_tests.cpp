#include "snake/snake.hpp"

#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/trompeloeil.hpp>
#include <ranges>
#include <trompeloeil.hpp>

using namespace std;

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