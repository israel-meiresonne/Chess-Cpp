#include <gtest/gtest.h>

#include <model/pieces/pieces.hpp>
#include <model/position/position.hpp>

class MockPiece : public Pieces::Piece {
  public:
    MockPiece(Position pos)
        : Piece(pos) {}

  protected:
    std::unordered_set<Pieces::Move> &_moves(std::unordered_set<Pieces::Move> &moves, int &nRow,
                                             int &nColumn) {
        return moves;
    }
};

TEST(MoveTest, DefaultConstructor) {
    Pieces::Move move;
    EXPECT_EQ(move.actions().size(), 0);
    EXPECT_EQ(typeid(move.author()), typeid(Player));
    EXPECT_EQ(move.type(), Pieces::Move::Type::DISPLACEMENT);
}

TEST(MoveTest, PlayerConstructor) {
    Player player("Alice");
    Pieces::Move move(player, Pieces::Move::Type::CAPTURE);
    EXPECT_EQ(move.author(), player);
    EXPECT_EQ(move.actions().size(), 0);
    EXPECT_EQ(move.type(), Pieces::Move::Type::CAPTURE);
}

TEST(MoveTest, TypeConstructor) {
    Player player("Alice");
    Pieces::Move move(Pieces::Move::Type::SWAP, player);
    EXPECT_EQ(move.author(), player);
    EXPECT_EQ(move.actions().size(), 0);
    EXPECT_EQ(move.type(), Pieces::Move::Type::SWAP);
}

TEST(MoveTest, Getters) {
    Player author("Bob");
    Pieces::Move move(author, Pieces::Move::Type::SWAP);
    EXPECT_EQ(move.author(), author);
    EXPECT_TRUE(move.actions().empty());
    EXPECT_EQ(move.type(), Pieces::Move::Type::SWAP);
}

TEST(MoveTest, AddAction) {
    Player player("Bob");
    Pieces::Move move(player);
    Pieces::King piece(Position(1, 1));
    Position initial(1, 1);
    Position final(2, 2);
    Pieces::Action action1(&piece, initial, final);
    Pieces::Action action2(&piece, final, initial);

    move.add(action1);

    EXPECT_EQ(move.actions().size(), 1);
    EXPECT_EQ(move.actions()[0], action1);

    move.add(action2);

    EXPECT_EQ(move.actions().size(), 2);
    EXPECT_EQ(move.actions()[0], action1);
    EXPECT_EQ(move.actions()[1], action2);
}

TEST(MoveTest, EqualityOperator) {
    Player player1("Alice");
    Player player2("Bob");

    Pieces::King piece(Position(1, 1));

    Position initial(1, 1);
    Position final(2, 2);

    Pieces::Action action1(&piece, initial, final);
    Pieces::Action action2(&piece, final, initial);

    Pieces::Move move1;
    Pieces::Move move2;
    Pieces::Move move3(player1);
    Pieces::Move move4(player1);
    Pieces::Move move5(player1);
    move5.add(action1);
    move5.add(action2);
    Pieces::Move move6(player1);
    move6.add(action1);
    move6.add(action2);
    Pieces::Move move7(player1);
    move7.add(action2);
    move7.add(action1);
    Pieces::Move move8(player1);
    move8.add(action2);
    move8.add(action1);
    Pieces::Move move9(player1, Pieces::Move::Type::SWAP);
    move9.add(action2);
    move9.add(action1);
    Pieces::Move move10(player1, Pieces::Move::Type::SWAP);
    move10.add(action2);
    move10.add(action1);

    EXPECT_TRUE(move1 == move2);
    EXPECT_FALSE(move2 == move3);
    EXPECT_TRUE(move3 == move4);
    EXPECT_FALSE(move4 == move5);
    EXPECT_TRUE(move5 == move6);
    EXPECT_FALSE(move6 == move7);
    EXPECT_TRUE(move7 == move8);
    EXPECT_FALSE(move8 == move9);
    EXPECT_TRUE(move9 == move10);
}

TEST(MoveTest, Hash) {
    Player player1("Alice");
    Player player2("Bob");

    Pieces::King piece(Position(1, 1));

    Position initial(1, 1);
    Position final(2, 2);

    Pieces::Action action1(&piece, initial, final);
    Pieces::Action action2(&piece, final, initial);

    Pieces::Move move1;
    Pieces::Move move2;
    Pieces::Move move3(player1);
    Pieces::Move move4(player1);
    Pieces::Move move5(player1);
    move5.add(action1);
    move5.add(action2);
    Pieces::Move move6(player1);
    move6.add(action1);
    move6.add(action2);
    Pieces::Move move7(player1);
    move7.add(action2);
    move7.add(action1);
    Pieces::Move move8(player1);
    move8.add(action2);
    move8.add(action1);
    Pieces::Move move9(player1, Pieces::Move::Type::SWAP);
    move9.add(action2);
    move9.add(action1);
    Pieces::Move move10(player1, Pieces::Move::Type::SWAP);
    move10.add(action2);
    move10.add(action1);

    EXPECT_EQ(move1.hash(), move2.hash());
    EXPECT_NE(move2.hash(), move3.hash());
    EXPECT_EQ(move3.hash(), move4.hash());
    EXPECT_NE(move4.hash(), move5.hash());
    EXPECT_EQ(move5.hash(), move6.hash());
    EXPECT_NE(move6.hash(), move7.hash());
    EXPECT_EQ(move7.hash(), move8.hash());
    EXPECT_NE(move8.hash(), move9.hash());
    EXPECT_EQ(move9.hash(), move10.hash());
}

TEST(MoveTest, CreateMove) {
    MockPiece piece1(Position(1, 1));
    MockPiece piece2(Position(2, 2));
    Position initial(1, 1);
    Position final(2, 2);

    Pieces::Move move1 = Pieces::Move::createMove(&piece1, initial, final);
    Pieces::Move move2 =
        Pieces::Move::createMove(&piece2, initial, final, Pieces::Move::Type::CAPTURE);

    EXPECT_EQ(move1.type(), Pieces::Move::Type::DISPLACEMENT);
    EXPECT_EQ(move2.type(), Pieces::Move::Type::CAPTURE);

    ASSERT_FALSE(move1.actions().empty());
    ASSERT_FALSE(move2.actions().empty());

    Pieces::Action action = move1.actions().front();
    EXPECT_EQ(*action.piece(), piece1);
    EXPECT_EQ(action.initial(), initial);
    EXPECT_EQ(action.final(), final);

    action = move2.actions().front();
    EXPECT_EQ(*action.piece(), piece2);
    EXPECT_EQ(action.initial(), initial);
    EXPECT_EQ(action.final(), final);
}
