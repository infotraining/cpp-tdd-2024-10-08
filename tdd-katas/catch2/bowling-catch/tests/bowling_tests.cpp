#include "bowling.hpp"

#include <array>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_all.hpp>

using namespace std;

using TDD::BowlingGame;

void roll_many(BowlingGame& game, int count, int pins)
{
    for (int i = 0; i < count; ++i)
        game.roll(pins);
}

TEST_CASE("When game starts score is zero", "[Start#1][BowlingGame]")
{
    BowlingGame game;

    REQUIRE(game.score() == 0);
}

TEST_CASE("When all rolls are zero score is zero", "[RegularGame#2][BowlingGame]")
{
    BowlingGame game;

    roll_many(game, 20, 0);

    REQUIRE(game.score() == 0);
}

TEST_CASE("When rolls are not zero score is sum of pins")
{
    BowlingGame game;

    roll_many(game, 20, 1);

    REQUIRE(game.score() == 20);
}

TEST_CASE("When spare next roll is counted twice")
{
    BowlingGame game;

    auto params = GENERATE(table<int, int>({{5, 5}, {3, 7}, {8, 2}, {1, 9}}));

    auto [roll_1, roll_2] = params;

    DYNAMIC_SECTION("rolls for spare (" << roll_1 << ", " << roll_2 << ")")
    {
        game.roll(roll_1);
        game.roll(roll_2);

        roll_many(game, 18, 1);

        REQUIRE(game.score() == 29);
    }
}

TEST_CASE("When strike two next rolls are counted twice")
{
    BowlingGame game;

    game.roll(10); // strike

    roll_many(game, 18, 1);

    REQUIRE(game.score() == 30);
}

TEST_CASE("Last frame")
{
    // Arrange SUT
    BowlingGame game;
    roll_many(game, 18, 1);

    SECTION("spare - extra roll is allowed")
    {
        game.roll(5);
        game.roll(5);

        game.roll(6); // extra roll

        REQUIRE(game.score() == 34);
    }

    SECTION("strike - two extra rolls are allowed")
    {
        game.roll(10);

        game.roll(6); // extra roll
        game.roll(6); // extra roll

        REQUIRE(game.score() == 40);
    }
}

TEST_CASE("Perfect game")
{
    BowlingGame game;

    roll_many(game, 12, 10);

    REQUIRE(game.score() == 300);
}