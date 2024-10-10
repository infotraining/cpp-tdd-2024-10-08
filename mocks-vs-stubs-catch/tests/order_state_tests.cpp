#include "order.hpp"
#include "warehouse.hpp"

#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <string>
#include <unordered_map>

using namespace std;

class WarehouseImpl : public Warehouse
{
    unordered_map<string, size_t> inventory_;

public:
    bool has_inventory(const std::string& product, size_t quantity) const override
    {
        return inventory_.at(product) >= quantity;
    }

    void add(const std::string& name, size_t count) override
    {
        inventory_.insert(make_pair(name, count));
    }

    size_t get_inventory(const std::string& name) const override
    {
        return inventory_.at(name);
    }

    void remove(const std::string& product, size_t quantity) override
    {
        inventory_.at(product) -= quantity;
    }
};

namespace 
{
    const string talisker = "Talisker";
    const string highland_park = "Highland Park";
}

SCENARIO("Filling order - state verification")
{
    WarehouseImpl warehouse;
    warehouse.add(talisker, 50);
    warehouse.add(highland_park, 25);

    GIVEN("Enough stock in warehouse")
    {
        Order order{talisker, 50};

        WHEN("Filling the order")
        {
            order.fill(warehouse);

            THEN("Order is filled")
            {
                REQUIRE(order.is_filled());
            }

            THEN("Stock is transferred from warehouse")
            {
                REQUIRE(warehouse.get_inventory(talisker) == 0);
            }
        }
    }

    GIVEN("Not enough stock in warehouse")
    {
        Order order{talisker, 51};

        WHEN("Filling the order")
        {
            order.fill(warehouse);

            THEN("Order is not filled")
            {
                REQUIRE_FALSE(order.is_filled());
            }

            AND_THEN("Stock is not transferred from warehouse")
            {
                REQUIRE(warehouse.get_inventory(talisker) == 50);
            }
        }
    }
}