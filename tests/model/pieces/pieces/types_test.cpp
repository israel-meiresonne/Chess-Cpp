#include <gtest/gtest.h>

#include <model/pieces/pieces.hpp>

TEST(PiecesTypesTest, StringConversion) {
    EXPECT_EQ(std::string(Pieces::Types::KING), "KING");
    EXPECT_EQ(std::string(Pieces::Types::QUEEN), "QUEEN");
    EXPECT_EQ(std::string(Pieces::Types::ROOK), "ROOK");
    EXPECT_EQ(std::string(Pieces::Types::BISHOP), "BISHOP");
    EXPECT_EQ(std::string(Pieces::Types::KNIGHT), "KNIGHT");
    EXPECT_EQ(std::string(Pieces::Types::PAWN), "PAWN");
    EXPECT_EQ(std::string(Pieces::Types::UNDEFINED), "UNDEFINED");
}

TEST(PiecesTypesTest, EqualityOperator) {
    EXPECT_TRUE(Pieces::Types::KING == Pieces::Types(Pieces::Types::_Types::KING));
    EXPECT_TRUE(Pieces::Types::QUEEN == Pieces::Types(Pieces::Types::_Types::QUEEN));
    EXPECT_FALSE(Pieces::Types::KING == Pieces::Types::QUEEN);
}

TEST(PiecesTypesTest, InequalityOperator) {
    EXPECT_FALSE(Pieces::Types::KING != Pieces::Types(Pieces::Types::_Types::KING));
    EXPECT_FALSE(Pieces::Types::QUEEN != Pieces::Types(Pieces::Types::_Types::QUEEN));
    EXPECT_TRUE(Pieces::Types::KING != Pieces::Types::QUEEN);
}

TEST(PiecesTypesTest, HashFunction) {
    EXPECT_EQ(Pieces::Types::KING.hash(), static_cast<int>(Pieces::Types::_Types::KING) << 1);
    EXPECT_EQ(Pieces::Types::QUEEN.hash(), static_cast<int>(Pieces::Types::_Types::QUEEN) << 1);
    EXPECT_NE(Pieces::Types::KING.hash(), Pieces::Types::QUEEN.hash());
}

TEST(PiecesTypesTest, InvalidTypeThrowsException) {
    Pieces::Types invalidType(static_cast<Pieces::Types::_Types>(999));
    EXPECT_THROW(({ auto s = static_cast<std::string>(invalidType); }), std::runtime_error);
}
