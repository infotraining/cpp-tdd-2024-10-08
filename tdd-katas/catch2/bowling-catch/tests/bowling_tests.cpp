#include <catch2/catch_test_macros.hpp>
#include "bowling.hpp"

using namespace std;

class BowlingGame
{
public:
    int score() const
    {
        return 0;
    }
};

TEST_CASE("When game starts score is zero")
{
    BowlingGame game;

    REQUIRE(game.score() == 0);
}

