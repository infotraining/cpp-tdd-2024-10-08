#ifndef BOWLING_HPP
#define BOWLING_HPP

#include <algorithm>
#include <string>
#include <vector>
#include <array>

namespace TDD
{
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
}

#endif