#include <catch2/catch_test_macros.hpp>
#include "snake_game_builder.hpp"

TEST_CASE("SnakeGameBuilder")
{
    SECTION("Default values")
    {
        SnakeGame game = SnakeGameBuilder().build();
        REQUIRE(game.board().width() == 10);
        REQUIRE(game.board().height() == 10);
        REQUIRE(game.snake() == Snake{Point(5, 5)});
        REQUIRE(game.snake().direction() == Direction::Up);
    }

    SECTION("Custom values")
    {
        Snake snake{Point(3, 3)};
        SnakeGame game = SnakeGameBuilder{}
                             .with_board(20, 30)
                             .with_snake(snake)
                             .with_snakes_direction(Direction::Down)
                             .build();

        REQUIRE(game.board().width() == 20);
        REQUIRE(game.board().height() == 30);
        REQUIRE(game.snake() == snake);
        REQUIRE(game.snake().direction() == Direction::Down);
    }
}