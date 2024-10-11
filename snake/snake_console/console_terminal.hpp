#ifndef SNAKE_CONSOLE_TERMINAL_HPP
#define SNAKE_CONSOLE_TERMINAL_HPP

#include <cpp-terminal/input.hpp>
#include <cpp-terminal/iostream.hpp>
#include <cpp-terminal/key.hpp>
#include <cpp-terminal/terminal.hpp>
#include <cpp-terminal/window.hpp>
#include <snake/snake.hpp>

class ConsoleTerminal : public Terminal
{
    Term::Terminal term_;
    Term::Window wnd_;
    std::atomic<Key> key_{Key::Up};
    std::jthread thd_event_loop_;

public:
    ConsoleTerminal(size_t w, size_t h)
        : wnd_(w, h)
    {
        Term::terminal.setOptions(Term::Option::ClearScreen, Term::Option::NoSignalKeys, Term::Option::NoCursor, Term::Option::Raw);

        thd_event_loop_ = std::jthread([this] {
            while (true)
            {
                Term::Key key = Term::read_event();

                switch (key)
                {
                case Term::Key::ArrowUp:
                    key_ = Key::Up;
                    break;
                case Term::Key::ArrowDown:
                    key_ = Key::Down;
                    break;
                case Term::Key::ArrowLeft:
                    key_ = Key::Left;
                    break;
                case Term::Key::ArrowRight:
                    key_ = Key::Right;
                    break;
                case Term::Key::Ctrl_Q:
                    key_ = Key::Ctrl_Q;
                    return;
                }
            }
        });
    }

    ConsoleTerminal(const ConsoleTerminal&) = delete;
    ConsoleTerminal& operator=(const ConsoleTerminal&) = delete;
    ConsoleTerminal(ConsoleTerminal&&) = delete;
    ConsoleTerminal& operator=(ConsoleTerminal&&) = delete;

    Key read_key() override
    {
        return key_.load(); // atomic load
    }

    void render_snake(const Snake& snake) override
    {        
        wnd_.print_border();

        for (const auto& segment : snake.segments())
        {
            wnd_.set_char(static_cast<size_t>(segment.x), static_cast<size_t>(segment.y), '#');
        }
    }

    void render_fruits(const std::vector<Point>& fruits) override
    {
        for (const auto& fruit : fruits)
        {
            wnd_.set_char(static_cast<size_t>(fruit.x), static_cast<size_t>(fruit.y), '@');
        }
    }

    void render_text(int x, int y, const std::string& message) override
    {
        wnd_.print_str(wnd_.get_w() / 2 - message.size() / 2, wnd_.get_h() / 2, message);
    }

    void flush() override
    {
        Term::cout << wnd_.render(1, 1, true) << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        wnd_.clear();
    }
};

#endif // SNAKE_CONSOLE_TERMINAL_HPP