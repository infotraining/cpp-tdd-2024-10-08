#include "snake/snake.hpp"

#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/trompeloeil.hpp>
#include <ranges>
#include <trompeloeil.hpp>

using namespace std;

struct Point {
    Point(int x , int y) : x{x}, y{y}
    {}
    
    int x, y;    

    bool operator==(const Point& other) const
    {
        return x == other.x && y == other.y;
    }

    friend std::ostream& operator<<(std::ostream& out, const Point& pt)
    {
        out << "Point{" << pt.x << ", " << pt.y << "}";
        return out;
    }
};

class Snake
{
private:
    Point head_;
public:
    Snake(Point);
    ~Snake();

    Point head() {
        return head_;
    }

    size_t segments() const
    {
        return 1;
    }    
};

Snake::Snake(Point head) : head_{head}
{
}

Snake::~Snake()
{
}

TEST_CASE("Snake")
{
    SECTION("constructed with Point")
    {
        Snake snake(Point(10, 5));

        SECTION("has head at specified location")
        {
            REQUIRE(snake.head() == Point(10, 5));
        }

        SECTION("has one segment")
        {
            REQUIRE(snake.segments() == 1);
        }
    }
}