#include <catch2/catch_test_macros.hpp>
#include "bowling.hpp"

using namespace std;

class BowlingGame
{
private:
    //int score_val = 0;

public:
    int score() const
    {
        return 0;
    }

    void roll(int val)
    {
        //score_val += val;
    }
};

TEST_CASE("When game starts score is zero")
{
    BowlingGame game;

    REQUIRE(game.score() == 0);
}

TEST_CASE("When all rolls are zero score is zero")
{
    BowlingGame game;

    for(int i = 0; i < 20; ++i)
        game.roll(0);

    REQUIRE(game.score() == 0);
}