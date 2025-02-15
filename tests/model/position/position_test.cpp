#include <gtest/gtest.h>

#include <model/position/position.hpp>

// Test Default Constructor
TEST(PositionTest, DefaultConstructor) {
    Position pos;
    EXPECT_EQ(pos.row(), 0);
    EXPECT_EQ(pos.column(), 0);
}

// Test Parameterized Constructor
TEST(PositionTest, ParameterizedConstructor) {
    Position pos(3, 5);
    EXPECT_EQ(pos.row(), 3);
    EXPECT_EQ(pos.column(), 5);
}

// Test Row Getter
TEST(PositionTest, RowGetter) {
    Position pos(7, 2);
    EXPECT_EQ(pos.row(), 7);
}

// Test Column Getter
TEST(PositionTest, ColumnGetter) {
    Position pos(4, 9);
    EXPECT_EQ(pos.column(), 9);
}

// Test Equality Operator
TEST(PositionTest, EqualityOperator) {
    Position pos1(1, 2);
    Position pos2(1, 2);
    Position pos3(3, 4);
    EXPECT_TRUE(pos1 == pos2);
    EXPECT_FALSE(pos1 == pos3);
}
