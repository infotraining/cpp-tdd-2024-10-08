#include <catch2/catch_test_macros.hpp>
#include <catch2/trompeloeil.hpp>
#include <map>
#include <trompeloeil.hpp>

class Interface
{
public:
    virtual ~Interface() = default;

    virtual int generate() = 0;
    virtual std::string& get_name() = 0;
    virtual bool save_value(int k, const std::string& v) = 0;
    virtual std::vector<int> get_data(const std::string& filename) const = 0;
    virtual bool compare(const std::string& lhs, const std::string& rhs) const = 0;
};

struct MockInterface : Interface
{
    MAKE_MOCK0(generate, int(), override);
    MAKE_MOCK0(get_name, std::string&(), override);
    MAKE_MOCK2(save_value, bool(int, const std::string&), override);
    MAKE_MOCK1(get_data, std::vector<int>(const std::string&), const override);
    MAKE_MOCK2(compare, bool(const std::string&, const std::string&), const override);
};

class Dictionary
{
public:
    using id_t = size_t;
    MAKE_CONST_MOCK1(lookup, const std::string&(id_t));
};

TEST_CASE("Mocks demo")
{
    using trompeloeil::_;

    SECTION("Returning values & references")
    {
        MockInterface mock;

        std::string name = "jan";
        std::vector<int> data = {665};

        ALLOW_CALL(mock, generate()).RETURN(42);
        ALLOW_CALL(mock, get_name()).LR_RETURN((name));
        ALLOW_CALL(mock, get_data(trompeloeil::_)).RETURN(data);
        ALLOW_CALL(mock, get_data("data.txt")).RETURN(std::vector{1, 2, 3});

        REQUIRE(mock.generate() == 42);
        REQUIRE(mock.get_name() == "jan");
        REQUIRE(mock.get_data("text") == std::vector{665});
        REQUIRE(mock.get_data("data.txt") == std::vector<int>{1, 2, 3});
    }

    SECTION("Expectations")
    {
        MockInterface mock;

        using ::trompeloeil::ne;

        ALLOW_CALL(mock, get_data(_)).RETURN(std::vector{1, 2, 3});
        ALLOW_CALL(mock, get_data(ne(""))).RETURN(std::vector{3, 2, 1});
        ALLOW_CALL(mock, get_data("fib.txt")).RETURN(std::vector{1, 1, 2, 3});
        REQUIRE_CALL(mock, get_data("data.txt")).RETURN(std::vector{665}).TIMES(1);

        REQUIRE(mock.get_data("fib.txt") == std::vector{1, 1, 2, 3});
        REQUIRE(mock.get_data("data.txt") == std::vector{665});
        REQUIRE(mock.get_data("data.txt") == std::vector{3, 2, 1});
        REQUIRE(mock.get_data("") == std::vector{1, 2, 3});

        ALLOW_CALL(mock, compare(_, _)).RETURN(false);
        ALLOW_CALL(mock, compare(_, _)).WITH(_1 == _2).RETURN(true);
        REQUIRE(mock.compare("a", "a"));
        REQUIRE_FALSE(mock.compare("a", "b"));
    }

    SECTION("Side effects")
    {
        std::map<int, std::string> spy_values;

        MockInterface mock;

        ALLOW_CALL(mock, save_value(_, _)).LR_SIDE_EFFECT(spy_values.emplace(_1, _2)).RETURN(true);

        mock.save_value(1, "a");
        mock.save_value(2, "b");
        mock.save_value(3, "c");

        REQUIRE(std::map<int, std::string>{{1, "a"}, {2, "b"}, {3, "c"}} == spy_values);
    }

    SECTION("Throwing exceptions")
    {
        Dictionary d;
        std::vector<std::string> dict{"one", "two", "three"};

        ALLOW_CALL(d, lookup(_))
            .LR_WITH(_1 >= dict.size())
            .THROW(std::out_of_range("index too large for dictionary"));

        ALLOW_CALL(d, lookup(_))
            .LR_WITH(_1 < dict.size())
            .LR_RETURN(dict[_1]);

        REQUIRE_THROWS_AS(d.lookup(3), std::out_of_range);
    }
}

////////////////////////////////////////////////////////////////////////////////////////
//

struct User
{
    int id;
    std::string name;
    std::string pwd;

    bool operator==(const User& other) const = default;

    friend std::ostream& operator<<(std::ostream& out, const User& user)
    {
        out << "User{id=" << user.id << ", name=" << user.name << ", pwd=" << user.pwd << "}";
        return out;
    }
};

namespace Classic_DI
{
    class UsersDatabase
    {
    public:
        virtual std::vector<User> get_all_users() = 0;
        virtual User get_user(int id) = 0;
        virtual void update_user(User user) = 0;
        virtual ~UsersDatabase() { }
    };

    struct MockUsersDatabase : UsersDatabase
    {
        MAKE_MOCK0(get_all_users, std::vector<User>(), override);
        MAKE_MOCK1(get_user, User(int), override);
        MAKE_MOCK1(update_user, void(User), override);
    };

    class UserService
    {
        std::unique_ptr<UsersDatabase> db_;

    public:
        UserService(std::unique_ptr<UsersDatabase> db)
            : db_(std::move(db))
        {
        }

        void set_new_password(int user_id, const std::string& new_pwd)
        {
            auto user = db_->get_user(user_id);
            user.pwd = new_pwd;
            db_->update_user(user);
        }
    };
}

TEST_CASE("UserService#1 - changing password for user")
{
    using namespace Classic_DI;

    auto user_db = std::make_unique<MockUsersDatabase>();
    ALLOW_CALL(*user_db, get_user(42)).RETURN(User{42, "Kowalski", "123"});
    REQUIRE_CALL(*user_db, update_user(User{42, "Kowalski", "1111"}));

    UserService user_srv(std::move(user_db));

    user_srv.set_new_password(42, "1111");
}

namespace HighPerf_DI
{
    template <typename TUsersDatabase>
    class UserService
    {
        TUsersDatabase& db_;

    public:
        UserService(TUsersDatabase& db)
            : db_(db)
        {
        }

        void set_new_password(int user_id, const std::string& new_pwd)
        {
            auto user = db_.get_user(user_id);
            user.pwd = new_pwd;
            db_.update_user(user);
        }
    };
}

struct MockUsersDatabase
{
    MAKE_MOCK0(get_all_users, std::vector<User>());
    MAKE_MOCK1(get_user, User(int));
    MAKE_MOCK1(update_user, void(User));
};

TEST_CASE("UserService#2 - changing password for user")
{
    using namespace HighPerf_DI;

    MockUsersDatabase mock_db;
    ALLOW_CALL(mock_db, get_user(42)).RETURN(User{42, "Kowalski", "123"});
    REQUIRE_CALL(mock_db, update_user(User{42, "Kowalski", "1111"}));

    UserService user_srv(mock_db); 

    user_srv.set_new_password(42, "1111");
}