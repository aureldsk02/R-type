#include <gtest/gtest.h>

#include "../include/components.hpp"
#include "../include/registry.hpp"

class registryTest : public testing::Test {
  protected:
    registry reg;
    registry emptyReg;
};

TEST_F(registryTest, initialValues) {
    // Checking the entity number
    ASSERT_EQ(reg.getEntityNum(), 0);
    ASSERT_EQ(emptyReg.getEntityNum(), 0);
}

TEST_F(registryTest, entityHandling) {
    // Spawning an entity
    entity e = reg.spawn_entity();

    // Make sure it was created
    ASSERT_EQ(reg.getEntityNum(), 1);

    // Adding a component to it
    reg.add_component<component::position>(e, {0, 0});

    // Killing an entity
    reg.kill_entity(e);

    // Make sure it was killed
    auto& pos_table = reg.get_components<component::position>();

    ASSERT_EQ(pos_table[e].has_value(), false);

    // Make sure dead ids are reused
    entity new_entity = reg.spawn_entity();

    ASSERT_EQ(reg.getEntityNum(), 1);
    ASSERT_EQ(new_entity, e);

    // Error cases
    EXPECT_THROW(reg.kill_entity((entity)10), NonExistentEntityID);
}

TEST_F(registryTest, componentHandling) {
    vector<optional<component::position>>& pos_table =
        reg.get_components<component::position>();
    vector<optional<component::velocity>>& vel_table =
        reg.get_components<component::velocity>();

    // Check initial size
    ASSERT_EQ(pos_table.size(), 0);
    ASSERT_EQ(vel_table.size(), 0);

    // Registering again the same type of Component
    reg.register_components<component::position>();
    // Make sure the things didn't change
    pos_table = reg.get_components<component::position>();
    vel_table = reg.get_components<component::velocity>();

    ASSERT_EQ(pos_table.size(), 0);
    ASSERT_EQ(vel_table.size(), 0);

    // Spawning entities
    entity e1 = reg.spawn_entity();
    entity e2 = reg.spawn_entity();

    // Adding components only to e2
    reg.add_component<component::position>(e2, {0, 0});
    reg.add_component<component::velocity>(e2, {5, 5});

    // Checking the new sizes
    ASSERT_EQ(pos_table.size(), 2);
    ASSERT_EQ(vel_table.size(), 2);

    // Recuperate the stored values
    auto e1_pos = pos_table[0];
    auto e1_vel = vel_table[0];

    auto e2_pos = pos_table[1];
    auto e2_vel = vel_table[1];

    // Check they are correct
    ASSERT_EQ(e1_pos.has_value(), false);
    ASSERT_EQ(e1_vel.has_value(), false);

    ASSERT_EQ(e2_pos.value().x, 0);
    ASSERT_EQ(e2_pos.value().y, 0);

    ASSERT_EQ(e2_vel.value().vx, 5);
    ASSERT_EQ(e2_vel.value().vy, 5);

    // Deleting a component
    reg.remove_component<component::position>(e2);

    e2_pos = pos_table[1];

    ASSERT_EQ(e2_pos.has_value(), false);

    // Error cases
    EXPECT_THROW(
        reg.get_components<unsigned long int>(), NonExistentComponentType
    );
    EXPECT_THROW(
        reg.add_component<unsigned long int>(e2, 522), NonExistentComponentType
    );
    EXPECT_THROW(
        reg.remove_component<unsigned long int>(e2), NonExistentComponentType
    );

    EXPECT_THROW(
        reg.add_component<component::position>((entity)10, {0, 0}),
        NonExistentEntityID
    );
    EXPECT_THROW(
        reg.remove_component<component::position>((entity)10),
        NonExistentEntityID
    );
}
