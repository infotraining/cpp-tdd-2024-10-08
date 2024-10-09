#include "source.hpp"

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <deque>

using namespace std;

TEST_CASE("Arrange / Act / Assert")
{
    // Arrange
    vector<int> vec{1, 2, 3};

    // Act
    vec.push_back(4);

    // Assert
    REQUIRE(vec[3] == 4);
}

TEST_CASE("pushing items to vector", "[vector][push]")
{
    // Arange
    vector<int> vec;
    vec.push_back(3);

    // guard assertion
    CHECK(vec.size() == 1);
    CHECK(vec.capacity() == 1);

    SECTION("push_back")
    {
        // Act
        vec.push_back(1);

        // Assert
        SECTION("size is increased")
        {
            REQUIRE(vec.size() == 2);
        }

        SECTION("capacity can be increased")
        {
            REQUIRE(vec.capacity() >= 2);
        }

        SECTION("item is pushed on back")
        {
            REQUIRE(vec == std::vector{3, 1});
        }
    }
}

SCENARIO("pushing items to vector", "[vector][push]")
{
    GIVEN("vector with data") // Arrange
    {
        vector<int> vec = {1, 2, 3};
        auto prev_size = vec.size();

        WHEN("push_back() an item") // Act
        {
            vec.push_back(4);

            THEN("size is increased") // Assert
            {
                REQUIRE(vec.size() - prev_size == 1);
            }
        }
    }
}

TEMPLATE_TEST_CASE("vectors can be sized and resized", "[vector][template]", int, std::string, (std::tuple<int, float>))
{
    std::vector<TestType> v(5);

    REQUIRE(v.size() == 5);
    REQUIRE(v.capacity() >= 5);

    SECTION("resizing bigger changes size and capacity")
    {
        v.resize(10);

        REQUIRE(v.size() == 10);
        REQUIRE(v.capacity() >= 10);
    }

    SECTION("resizing smaller changes size but not capacity")
    {
        v.resize(0);

        REQUIRE(v.size() == 0);
        REQUIRE(v.capacity() >= 5);

        SECTION("We can use the 'swap trick' to reset the capacity")
        {
            std::vector<TestType> empty;
            empty.swap(v);

            REQUIRE(v.capacity() == 0);
        }
    }

    SECTION("reserving smaller does not change size or capacity")
    {
        v.reserve(0);

        REQUIRE(v.size() == 5);
        REQUIRE(v.capacity() >= 5);
    }
}

struct Point
{
    int x, y;

    bool operator==(const Point& other) const = default;

    friend std::ostream& operator<<(std::ostream& out, const Point& pt)
    {
        out << "Point{" << pt.x << ", " << pt.y << "}";
        return out;
    }
};

// namespace Catch
// {
//     template<>
//     struct StringMaker<T> {
//         static std::string convert( T const& value ) {
//             return convertMyTypeToString( value );
//         }
//     };
// }

enum class AsUInt8 : uint8_t
{
};

template <typename T, typename = std::enable_if<std::is_integral_v<T>>>
bool operator==(AsUInt8 left, T right)
{
    return static_cast<T>(left) == right;
}

std::ostream& operator<<(std::ostream& out, AsUInt8 value)
{
    out << static_cast<int>(value);
    return out;
}

TEST_CASE("uint8_t not treated as char")
{
    uint8_t value = 65;
    REQUIRE(AsUInt8{value} == 77);
}

enum class Fruits
{
    Banana,
    Apple,
    Mango
};

CATCH_REGISTER_ENUM(Fruits, Fruits::Banana, Fruits::Apple, Fruits::Mango)

TEST_CASE("enum")
{
    REQUIRE(Fruits::Mango == Fruits::Apple);
}

TEST_CASE("Point")
{
    Point pt1{1, 2};
    Point pt2{1, 3};

    REQUIRE(pt1 == pt2);
}

class RecentlyUsedList
{
    std::deque<std::string> items_;

public:
    void add(std::string item)
    {
        if (item == "")
        {
            throw std::invalid_argument("Empty string are not allowed");
        }

        items_.push_front(std::move(item));
    }

    std::string& recent()
    {
        return items_.front();
    }

    const std::string& recent() const
    {
        return items_.front();
    }

    const std::string& operator[](size_t index)
    {
        return items_[index];
    }
};

TEST_CASE("adding empty string throws exception")
{
    RecentlyUsedList rul;

    REQUIRE_THROWS_AS(rul.add(""), std::invalid_argument);
}

TEST_CASE("recent returns last inserted item")
{
    RecentlyUsedList rul;

    rul.add("item#0");
    rul.add("item#1");

    REQUIRE(rul.recent() == "item#1");
}

void add_many(RecentlyUsedList& rul, int count)
{
    for(int i = 0; i < count; ++i)
        rul.add("item#" + std::to_string(i));
}

void add_many(RecentlyUsedList& rul, std::initializer_list<std::string> items)
{
    for(const auto& item : items)
        rul.add(item);
}

TEST_CASE("modify recent item")
{
    RecentlyUsedList rul;

    // rul.add("item#0");
    // rul.add("item#1");
    // rul.add("item#2");

    add_many(rul, 3);

    rul.recent() = "new-item";

    REQUIRE(rul.recent() == "new_item");
}

TEST_CASE("indexing is in LIFO order")
{
    RecentlyUsedList rul;

    add_many(rul, { "item#1", "item#2" });

    REQUIRE(rul[0] == "item#2");
    REQUIRE(rul[1] == "item#1");
}
