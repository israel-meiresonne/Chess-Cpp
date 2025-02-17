#include <gtest/gtest.h>

#include <model/position/position.hpp>

TEST(PositionTest, DefaultConstructor) {
    Position pos;
    EXPECT_EQ(pos.row(), 0);
    EXPECT_EQ(pos.column(), 0);
}

TEST(PositionTest, ParameterizedConstructor) {
    Position pos(3, 5);
    EXPECT_EQ(pos.row(), 3);
    EXPECT_EQ(pos.column(), 5);
}

TEST(PositionTest, RowGetter) {
    Position pos(7, 2);
    EXPECT_EQ(pos.row(), 7);
}

TEST(PositionTest, ColumnGetter) {
    Position pos(4, 9);
    EXPECT_EQ(pos.column(), 9);
}

TEST(PositionTest, EqualityOperator) {
    Position pos1(1, 2);
    Position pos2(1, 2);
    Position pos3(3, 4);
    EXPECT_TRUE(pos1 == pos2);
    EXPECT_FALSE(pos1 == pos3);
}

TEST(PositionTest, Hash) {
    Position pos1(1, 2);
    Position pos2(1, 2);
    Position pos3(3, 4);

    EXPECT_EQ(pos1.hash(), pos2.hash());
    EXPECT_NE(pos1.hash(), pos3.hash());
}
