// #include <catch2/catch_test_macros.hpp>
// #include <trompeloeil.hpp>
// #include <catch2/trompeloeil.hpp>


// class Interface
// {
// public:
//     virtual ~Interface() = default;

//     virtual int generate() = 0;
//     virtual std::string& get_name() = 0;
//     virtual bool save_value(int k, const string& v) = 0;
//     virtual std::vector<int> get_data(const std::string& filename) const = 0;
//     virtual bool compare(const std::string& lhs, const std::string& rhs) const = 0;
// };

// struct MockInterface : Interface
// {
//     MAKE_MOCK0(generate, int(), override);
//     MAKE_MOCK0(get_name, std::string&(), override);
//     MAKE_MOCK2(save_value, bool(int, const std::string&), override);
//     MAKE_MOCK1(get_data, std::vector<int>(const std::string&), const override);
//     MAKE_MOCK2(compare, bool(const std::string&, const std::string&), const override);
// };

// class Dictionary
// {
// public:
//     using id_t = size_t;
//     MAKE_CONST_MOCK1(lookup, const std::string&(id_t));
// };

// TEST_CASE("Mocks demo")
// {
//     using trompeloeil::_;

//     SECTION("Returning values & references")
//     {
//         MockInterface mock;

//         std::string name = "jan";
//         std::vector<int> data = {665};

//         ALLOW_CALL(mock, generate()).RETURN(42);
//         ALLOW_CALL(mock, get_name()).LR_RETURN(std::ref(name));
//         ALLOW_CALL(mock, get_data(_)).RETURN(data);
//         ALLOW_CALL(mock, get_data("data.txt")).RETURN(std::vector{1, 2, 3});

//         REQUIRE(mock.generate() == 42);
//         REQUIRE(mock.get_name() == "jan");
//         REQUIRE(mock.get_data("text") == std::vector{665});
//         REQUIRE(mock.get_data("data.txt") == std::vector<int>{1, 2, 3});
//     }

//     SECTION("Expectations")
//     {
//         MockInterface mock;

//         using ::trompeloeil::ne;

//         ALLOW_CALL(mock, get_data(_)).RETURN(std::vector{1, 2, 3});
//         ALLOW_CALL(mock, get_data(ne(""))).RETURN(std::vector{3, 2, 1});
//         ALLOW_CALL(mock, get_data("fib.txt")).RETURN(std::vector{1, 1, 2, 3});
//         REQUIRE_CALL(mock, get_data("data.txt")).RETURN(std::vector{665}).TIMES(1);

//         REQUIRE(mock.get_data("fib.txt") == std::vector{1, 1, 2, 3});
//         REQUIRE(mock.get_data("data.txt") == std::vector{665});
//         REQUIRE(mock.get_data("data.txt") == std::vector{3, 2, 1});
//         REQUIRE(mock.get_data("") == std::vector{1, 2, 3});

//         ALLOW_CALL(mock, compare(_, _)).RETURN(false);
//         ALLOW_CALL(mock, compare(_, _)).WITH(_1 == _2).RETURN(true);
//         REQUIRE(mock.compare("a", "a"));
//         REQUIRE_FALSE(mock.compare("a", "b"));
//     }

//     SECTION("Side effects")
//     {
//         std::map<int, std::string> values;

//         MockInterface mock;

//         ALLOW_CALL(mock, save_value(_, _)).LR_SIDE_EFFECT(values.emplace(_1, _2)).RETURN(true);

//         mock.save_value(1, "a");
//         mock.save_value(2, "b");
//         mock.save_value(3, "c");

//         REQUIRE(map<int, string>{{1, "a"}, {2, "b"}, {3, "c"}} == values);
//     }

//     SECTION("Throwing exceptions")
//     {
//         Dictionary d;
//         std::vector<std::string> dict{"one", "two", "three"};

//         ALLOW_CALL(d, lookup(_))
//             .LR_WITH(_1 >= dict.size())
//             .THROW(std::out_of_range("index too large for dictionary"));

//         ALLOW_CALL(d, lookup(_))
//             .LR_WITH(_1 < dict.size())
//             .LR_RETURN(dict[_1]);

//         REQUIRE_THROWS_AS(d.lookup(3), std::out_of_range);
//     }
// }
