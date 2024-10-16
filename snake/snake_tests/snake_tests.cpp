#include "snake/snake.hpp"
#include "snake_game_builder.hpp"

#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/trompeloeil.hpp>
#include <ranges>
#include <trompeloeil.hpp>

using namespace std;

CATCH_REGISTER_ENUM(Direction, Direction::Up, Direction::Down, Direction::Left, Direction::Right);

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

TEST_CASE("Snake's move", "[Snake][Move]")
{
    Snake snake = {Point(5, 5)};
    Board board(100, 100);
    snake.set_board(board);

    auto params = GENERATE(table<Direction, Snake>({{Direction::Up, Snake{Point(5, 4)}},
        {Direction::Down, Snake{Point(5, 6)}},
        {Direction::Left, Snake{Point(4, 5)}},
        {Direction::Right, Snake{Point(6, 5)}}}));

    auto [direction, expected_snake] = params;

    DYNAMIC_SECTION("move in " << Catch::StringMaker<Direction>::convert(direction) << " direction")
    {
        snake.move(direction);

        SECTION("segments are moved")
        {
            REQUIRE(snake == expected_snake);
        }

        SECTION("direction is set")
        {
            REQUIRE(snake.direction() == direction);
        }
    }
}

TEST_CASE("Hitting a wall", "[Snake][Board]")
{
    Board board(10, 10);

    auto params = GENERATE(table<Direction, Snake>({{Direction::Up, Snake{Point(5, 1)}},
        {Direction::Down, Snake{Point(5, 9)}},
        {Direction::Left, Snake{Point(1, 5)}},
        {Direction::Right, Snake{Point(9, 5)}}}));

    auto [direction, snake] = params;

    snake.set_board(board);
    CHECK(snake.is_alive());

    DYNAMIC_SECTION("moving in " << Catch::StringMaker<Direction>::convert(direction) << " direction")
    {
        SECTION("when snake hits the wall")
        {
            snake.move(direction);

            SECTION("snake dies")
            {
                REQUIRE(snake.is_alive() == false);
            }
        }
    }
}

TEST_CASE("Snake eats itself", "[Snake]")
{
    Snake snake = {Point(5, 6), Point(6, 6), Point(7, 6), Point(7, 5), Point(6, 5), Point(5, 5), Point(4, 5)};
    CHECK(snake.is_alive());

    snake.move(Direction::Up);

    REQUIRE(snake.is_alive() == false);
}

TEST_CASE("Eating apple", "[Snake][Board]")
{
    Snake snake = {Point(10, 12), Point(10, 11), Point(10, 10)};
    Board board(20, 20);
    snake.set_board(board);

    board.add_fruit(Point(10, 13));
    CHECK(board.fruits().size() == 1); // guard

    SECTION("when snake eats apple")
    {
        snake.move(Direction::Down);

        SECTION("apple disappears from board")
        {
            REQUIRE(board.fruits().empty());
        }

        SECTION("snake grows by one segment")
        {
            REQUIRE(snake == Snake{Point(10, 13), Point(10, 12), Point(10, 11), Point(10, 10)});
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

class MockTerminal : public Terminal
{
public:
    MAKE_MOCK1(render_snake, void(const Snake&), override);
    MAKE_MOCK1(render_fruits, void(const std::vector<Point>& fruits), override);
    MAKE_MOCK3(render_text, void(int x, int y, const std::string& text), override);
    MAKE_MOCK0(read_key, Terminal::Key(), override);
    MAKE_MOCK0(flush, void(), override);
};

TEST_CASE("Rendering the snake", "[ConsoleGame][Terminal][Run]")
{
    MockTerminal terminal;

    SnakeGame game = SnakeGameBuilder{}
                         .with_board(20, 20)
                         .with_snake(Snake{Point(15, 10)})
                         .with_terminal(terminal)
                         .build();

    // requirements
    ALLOW_CALL(terminal, render_fruits(trompeloeil::_));
    ALLOW_CALL(terminal, flush());
    ALLOW_CALL(terminal, read_key()).RETURN(Terminal::Key::Ctrl_Q);
    REQUIRE_CALL(terminal, render_snake(Snake{Point{15, 10}}));

    game.run();
} // validation of requirements (assert on mock)

TEST_CASE("Rendering apples", "[ConsoleGame][Terminal][Run]")
{
    Board board(20, 20);
    board.add_fruit({2, 2});
    board.add_fruit({3, 3});

    MockTerminal terminal;

    SnakeGame game = SnakeGameBuilder{}
                         .with_board(board)
                         .with_snake(Snake{Point(15, 10)})
                         .with_terminal(terminal)
                         .build();

    ALLOW_CALL(terminal, render_snake(trompeloeil::_));
    ALLOW_CALL(terminal, flush());
    ALLOW_CALL(terminal, read_key()).RETURN(Terminal::Key::Ctrl_Q);
    REQUIRE_CALL(terminal, render_fruits(std::vector{Point{2, 2}, Point{3, 3}}));
    game.run();
}

TEST_CASE("Run reads key", "[ConsoleGame][Terminal][Run]")
{
    MockTerminal terminal;

    SnakeGame game = SnakeGameBuilder{}
                         .with_terminal(terminal)
                         .build();

    ALLOW_CALL(terminal, render_fruits(trompeloeil::_));
    ALLOW_CALL(terminal, render_snake(trompeloeil::_));
    ALLOW_CALL(terminal, flush());
    REQUIRE_CALL(terminal, read_key()).RETURN(Terminal::Key::Ctrl_Q);
    game.run();
}

TEST_CASE("Run Loops until Ctrl-Q", "[ConsoleGame][Terminal][Run][Loop]")
{
    Board board(20, 20);
    SnakeGame game(board);
    MockTerminal terminal;
    game.set_terminal(terminal);

    using trompeloeil::_;
    ALLOW_CALL(terminal, render_snake(_));
    ALLOW_CALL(terminal, render_fruits(_));
    ALLOW_CALL(terminal, flush());
    ALLOW_CALL(terminal, read_key()).RETURN(Terminal::Key::Ctrl_Q);

    trompeloeil::sequence seq;
    REQUIRE_CALL(terminal, render_snake(_)).TIMES(2);
    REQUIRE_CALL(terminal, render_fruits(_)).TIMES(2);
    REQUIRE_CALL(terminal, flush()).TIMES(2);
    REQUIRE_CALL(terminal, read_key()).TIMES(2).RETURN(Terminal::Key::Up);

    game.run();
}

TEST_CASE("When key is pressed - snake moves in given direction", "[SnakeGame][Run][Move]")
{
    auto params = GENERATE(table<Terminal::Key, Snake>({{Terminal::Key::Up, Snake{Point(5, 4)}},
        {Terminal::Key::Down, Snake{Point(5, 6)}},
        {Terminal::Key::Left, Snake{Point(4, 5)}},
        {Terminal::Key::Right, Snake{Point(6, 5)}}}));

    auto [key, expected_snake] = params;

    MockTerminal terminal;

    SnakeGame game = SnakeGameBuilder{}
                         .with_board(10, 10)
                         .with_snakes_direction(Direction::Up)
                         .with_terminal(terminal)
                         .build();

    using trompeloeil::_;
    ALLOW_CALL(terminal, render_snake(_));
    ALLOW_CALL(terminal, render_fruits(_));
    ALLOW_CALL(terminal, flush());
    ALLOW_CALL(terminal, read_key()).RETURN(Terminal::Key::Ctrl_Q);

    REQUIRE_CALL(terminal, read_key()).RETURN(key);

    game.run();

    REQUIRE(game.snake() == expected_snake);
}

TEST_CASE("SnakeGame - when snake hits the wall - the game is over", "[SnakeGame][Run][GameOver]")
{
    MockTerminal terminal;
    SnakeGame game = SnakeGameBuilder{}
                         .with_terminal(terminal)
                         .build();

    using trompeloeil::_;
    ALLOW_CALL(terminal, render_snake(_));
    ALLOW_CALL(terminal, render_fruits(_));
    ALLOW_CALL(terminal, render_text(_, _, _));
    ALLOW_CALL(terminal, flush());
    ALLOW_CALL(terminal, read_key()).RETURN(Terminal::Key::Up);

    REQUIRE_CALL(terminal, read_key()).RETURN(Terminal::Key::Left).TIMES(5);

    game.run();

    REQUIRE(game.is_over() == true);
}

TEST_CASE("SnakeGame - when game end - GAME OVER ! is rendered", "[SnakeGame][Run][GameOver]")
{
    MockTerminal terminal;
    SnakeGame game = SnakeGameBuilder{}
                         .with_board(30, 20)
                         .with_terminal(terminal)
                         .build();

    using trompeloeil::_;
    ALLOW_CALL(terminal, render_snake(_));
    ALLOW_CALL(terminal, render_fruits(_));
    ALLOW_CALL(terminal, flush());
    ALLOW_CALL(terminal, read_key()).RETURN(Terminal::Key::Up);

    REQUIRE_CALL(terminal, render_text(10, 10, "GAME OVER !")).TIMES(1);

    game.run();

    CHECK(game.is_over() == true);
}


TEST_CASE("Populate the game with fruits", "[SnakeGame]")
{
    SnakeGame game = SnakeGameBuilder{}
                    .with_board(20, 20)
                    .build();

    game.populate_with_fruits(5);

    REQUIRE(game.board().fruits().size() == 5);
}