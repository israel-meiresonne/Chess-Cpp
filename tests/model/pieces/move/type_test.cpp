#include <gtest/gtest.h>

#include <model/pieces/pieces.hpp>

TEST(MoveTypeTest, EnumValues) {
    EXPECT_EQ(static_cast<int>(Pieces::Move::Type::DISPLACEMENT), 0);
    EXPECT_EQ(static_cast<int>(Pieces::Move::Type::CAPTURE), 1);
    EXPECT_EQ(static_cast<int>(Pieces::Move::Type::SWAP), 2);
}

TEST(MoveTypeTest, EnumValuesAreDistinct) {
    EXPECT_NE(Pieces::Move::Type::DISPLACEMENT, Pieces::Move::Type::CAPTURE);
    EXPECT_NE(Pieces::Move::Type::DISPLACEMENT, Pieces::Move::Type::SWAP);
    EXPECT_NE(Pieces::Move::Type::CAPTURE, Pieces::Move::Type::SWAP);
}
