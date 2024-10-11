#ifndef SNAKE_GAME_BUILDER_HPP
#define SNAKE_GAME_BUILDER_HPP

#include <optional>
#include <snake/snake.hpp>

class SnakeGameBuilder
{
    Board board_{10, 10};
    std::optional<Direction> direction_{};
    std::optional<Snake> snake_{};
    Terminal* terminal_{};

public:
    SnakeGameBuilder& with_board(int width, int height)
    {
        board_ = Board(width, height);
        return *this;
    }

    SnakeGameBuilder& with_board(Board board)
    {
        board_ = board;
        return *this;
    }

    SnakeGameBuilder& with_snake(Snake snake)
    {
        snake_ = snake;
        return *this;
    }

    SnakeGameBuilder& with_snakes_direction(Direction direction)
    {
        direction_ = direction;
        return *this;
    }

    SnakeGameBuilder& with_terminal(Terminal& terminal)
    {
        terminal_ = &terminal;
        return *this;
    }

    SnakeGame build()
    {
        Snake snake = snake_.value_or(Snake{Point(board_.width() / 2, board_.height() / 2)});
        snake.set_direction(direction_.value_or(Direction::Up));
        SnakeGame game(board_, snake);

        if (terminal_)
        {
            game.set_terminal(*terminal_);
        }

        return game;
    }
};

#endif // SNAKE_GAME_BUILDER_HPP