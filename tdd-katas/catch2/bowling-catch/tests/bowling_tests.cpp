#include "bowling.hpp"

#include <array>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_all.hpp>

using namespace std;

class BowlingGame
{
    static constexpr size_t max_pins_in_frame = 10;
    static constexpr size_t frames_count = 10;

private:
    std::array<unsigned int, 21> pins_{};

    size_t roll_index_{};

    size_t frame_score(size_t roll_index) const
    {
        return pins_[roll_index] + pins_[roll_index + 1];
    }

    bool is_spare(size_t roll_index) const
    {
        return frame_score(roll_index) == max_pins_in_frame;
    }

    size_t spare_bonus(size_t roll_index) const
    {
        return pins_[roll_index + 2];
    }

    bool is_strike(size_t roll_index) const
    {
        return pins_[roll_index] == max_pins_in_frame;
    }

    size_t strike_bonus(size_t roll_index) const
    {
        return pins_[roll_index + 1] + pins_[roll_index + 2];
    }

public:
    int score() const
    {
        size_t result{};
        size_t roll_index{};

        for (size_t frame_index = 0; frame_index < frames_count; ++frame_index)
        {
            if (is_strike(roll_index))
            {
                result += max_pins_in_frame + strike_bonus(roll_index);
                ++roll_index;
            }
            else
            {
                result += frame_score(roll_index);
                if (is_spare(roll_index))
                    result += spare_bonus(roll_index);
                roll_index += 2;
            }
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
    for (int i = 0; i < count; ++i)
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

TEST_CASE("When spare in last frame extra roll is allowed")
{
    BowlingGame game;

    roll_many(game, 18, 1);

    // spare
    game.roll(5);
    game.roll(5);

    game.roll(6); // extra roll

    REQUIRE(game.score() == 34);
}

TEST_CASE("When strike in last frame two extra rolls are allowed")
{
    BowlingGame game;

    roll_many(game, 18, 1);

    // strike
    game.roll(10);
    
    game.roll(6); // extra roll
    game.roll(6); // extra roll

    REQUIRE(game.score() == 40);
}