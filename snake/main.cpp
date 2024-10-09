#include <iostream>
#include "snake.hpp"

using namespace std;

int main()
{
    Term::Screen term_size = Term::screen_size();
    const size_t width = term_size.columns();
    const size_t height = term_size.rows();
    ConsoleTerminal terminal(width, height);
    SnakeGame snake_game(width, height);
    snake_game.set_terminal(terminal);
    snake_game.run();
}