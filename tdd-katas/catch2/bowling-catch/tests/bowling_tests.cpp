#include <catch2/catch_test_macros.hpp>
#include "bowling.hpp"

using namespace std;

class BowlingGame
{
private:
    int score_val = 0;

public:
    int score() const
    {
        return score_val;
    }

    void roll(int pins)
    {
        score_val += pins;
    }
};

void roll_many(BowlingGame& game, int count, int pins)
{
    for(int i = 0; i < count; ++i)
        game.roll(pins);
}

TEST_CASE("When game starts score is zero")
{
    BowlingGame game;

    REQUIRE(game.score() == 0);
}

TEST_CASE("When all rolls are zero score is zero")
{
    BowlingGame game;

    roll_many(game, 20, 0);

    REQUIRE(game.score() == 0);
}

TEST_CASE("When rolls are not zero score is valid")
{
    BowlingGame game;

    roll_many(game, 20, 1);

    REQUIRE(game.score() == 20);
}