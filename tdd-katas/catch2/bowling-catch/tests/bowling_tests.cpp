#include <catch2/catch_test_macros.hpp>
#include "bowling.hpp"
#include <array>

using namespace std;

class BowlingGame
{
    static constexpr size_t max_pins_in_frame = 10;

private:
    std::array<unsigned int, 20> pins_{};
    
    size_t roll_index_{};

    size_t spare_bonus(size_t roll_index) const
    {
        return pins_[roll_index + 2];
    }

    size_t frame_score(size_t roll_index) const
    {
        return pins_[roll_index] + pins_[roll_index+1];
    }

    bool is_spare(size_t roll_index) const
    {
        return frame_score(roll_index) == max_pins_in_frame;
    }

public:
    int score() const
    {
        size_t result{};

        for(size_t i = 0; i < 20; i += 2)
        {
            if (is_spare(i))
                result += spare_bonus(i); 
            result += frame_score(i) ;
        }

        return result;
    }

    void roll(int pins)
    {
        pins_[roll_index_] = pins;
        ++roll_index_;
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

TEST_CASE("When rolls are not zero score is sum of pins")
{
    BowlingGame game;

    roll_many(game, 20, 1);

    REQUIRE(game.score() == 20);
}

TEST_CASE("When spare next roll is counted twice")
{
    BowlingGame game;

    game.roll(5);
    game.roll(5);

    roll_many(game, 18, 1);

    REQUIRE(game.score() == 29);
}