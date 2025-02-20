#include <gtest/gtest.h>

#include "model/pieces/pieces.hpp"
#include "model/position/position.hpp"

class MockPiece : public Pieces::Piece {
  public:
    MockPiece(const Position &position)
        : Pieces::Piece(position) {}

  protected:
    std::unordered_set<Pieces::Move> &_moves(std::unordered_set<Pieces::Move> &moves, int &nRow,
                                             int &nColumn) {
        return moves;
    }
};

TEST(ActionTest, DefaultConstructor) {
    Pieces::Action action;

    EXPECT_EQ(action.piece(), nullptr);
    EXPECT_EQ(typeid(action.initial()), typeid(Position));
    EXPECT_EQ(typeid(action.final()), typeid(Position));
}

TEST(ActionTest, ParameterizedConstructor) {
    Position initial(1, 2);
    Position final(3, 4);
    MockPiece piece(initial);
    Pieces::Action action(&piece, initial, final);

    EXPECT_EQ(action.piece(), &piece);
    EXPECT_EQ(action.initial(), initial);
    EXPECT_EQ(action.final(), final);
}

TEST(ActionTest, EqualityOperator) {
    Position initial(1, 2);
    Position final(3, 4);
    MockPiece piece(initial);
    Pieces::Action action1(&piece, initial, final);
    Pieces::Action action2(&piece, initial, final);
    Pieces::Action action3(&piece, final, initial);

    EXPECT_TRUE(action1 == action2);
    EXPECT_FALSE(action1 == action3);
}

TEST(ActionTest, Hash) {
    Position initial(1, 2);
    Position final(3, 4);
    MockPiece piece(initial);
    Pieces::Action action1;
    Pieces::Action action2(&piece, initial, final);
    Pieces::Action action3(&piece, initial, final);

    EXPECT_NE(action1.hash(), action2.hash());
    EXPECT_EQ(action2.hash(), action3.hash());
}
