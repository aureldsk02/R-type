#include <gtest/gtest.h>

#include "../include/systems.hpp"

using namespace std;

class RegistrySystemTest : public testing::Test {
  protected:
    registry reg;
    entity e1 = reg.spawn_entity();
    entity e2 = reg.spawn_entity();

    void SetUp() override {
        reg.toggleMovement();
        reg.togglePhysics();
        reg.add_component<component::position>(e1, {100, 50});
        reg.add_component<component::velocity>(e1, {0, 0});
        reg.add_component<component::hurtbox>(e1, {500, 1, 32, 32});
        reg.add_component<component::controllable>(e1, {});

        reg.add_component<component::position>(e2, {100, 100});
        reg.add_component<component::velocity>(e2, {0, 0});
        reg.add_component<component::hitbox>(e2, {5, 1, 32, 32});
        reg.add_component<component::controllable>(
            e2, {false, true, false, true, true}
        );
    }
};

TEST_F(RegistrySystemTest, PositionSystem) {
    auto& pos_table = reg.get_components<component::position>();
    auto& vel_table = reg.get_components<component::velocity>();

    // Running the position system
    position_system(1, reg, pos_table, vel_table);

    // Checking updated positions
    ASSERT_EQ(pos_table[e1].value().x, 100);
    ASSERT_EQ(pos_table[e1].value().y, 50);

    ASSERT_EQ(pos_table[e2].value().x, 100);
    ASSERT_EQ(pos_table[e2].value().y, 100);

    reg.add_component<component::velocity>(e1, {5, -5});
    reg.add_component<component::velocity>(e2, {-6, 7});

    position_system(1, reg, pos_table, vel_table);

    ASSERT_EQ(pos_table[e1].value().x, 105);
    ASSERT_EQ(pos_table[e1].value().y, 45);

    ASSERT_EQ(pos_table[e2].value().x, 94);
    ASSERT_EQ(pos_table[e2].value().y, 107);
}

TEST_F(RegistrySystemTest, CollisionSystem) {
    testing::internal::CaptureStdout();

    std::string output;

    reg.run_systems(1);

    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "");

    testing::internal::CaptureStdout();
    reg.add_component<component::velocity>(e2, {0, -35});

    reg.run_systems(1);

    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "COLLISION 0 1\n");
}
