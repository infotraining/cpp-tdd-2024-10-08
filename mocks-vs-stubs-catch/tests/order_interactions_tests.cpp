#include "order.hpp"
#include "warehouse.hpp"

#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <catch2/trompeloeil.hpp>
#include <map>
#include <memory>
#include <string>
#include <trompeloeil.hpp>
#include <unordered_map>

using namespace std;

struct MockWarehouse : Warehouse
{
    MAKE_MOCK2(has_inventory, bool(const std::string&, size_t), const override);
    MAKE_MOCK2(add, void(const std::string&, size_t), override);
    MAKE_MOCK2(remove, void(const std::string&, size_t), override);
    MAKE_MOCK1(get_inventory, size_t(const std::string&), const override);
};

namespace
{
    const string talisker = "Talisker";
    const string highland_park = "Highland Park";
}

TEST_CASE("Filling order - behaviour verification")
{
    using trompeloeil::_;
    MockWarehouse warehouse;

    SECTION("Enough stock in warehouse")
    {
        Order order{talisker, 50};

        ALLOW_CALL(warehouse, has_inventory(talisker, 50)).RETURN(true);
        ALLOW_CALL(warehouse, remove(_, _));

        SECTION("When filling the orderOrder")
        {
            SECTION("Order is filled")
            {
                order.fill(warehouse);

                REQUIRE(order.is_filled());
            }

            SECTION("Stock is transferred from warehouse")
            {
                REQUIRE_CALL(warehouse, remove(talisker, 50)).TIMES(1);

                order.fill(warehouse);
            }
        }
    }

    GIVEN("Not enough stock in warehouse")
    {
        Order order{talisker, 51};

        ALLOW_CALL(warehouse, has_inventory(talisker, 51)).RETURN(false);

        WHEN("Order is filled")
        {
            THEN("Order is not filled")
            {
                order.fill(warehouse);
                REQUIRE_FALSE(order.is_filled());
            }

            AND_THEN("Stock is not transferred from warehouse")
            {
                REQUIRE_CALL(warehouse, remove(talisker, 51)).TIMES(0);
                order.fill(warehouse);
            }
        }
    }
}
