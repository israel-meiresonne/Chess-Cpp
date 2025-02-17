#include <gtest/gtest.h>

#include <model/pieces/pieces.hpp>
#include <model/position/position.hpp>

TEST(MoveTest, DefaultConstructor) {
    Pieces::Move move;
    EXPECT_EQ(move.actions().size(), 0);
}

TEST(MoveTest, ParameterizedConstructor) {
    Player player("Alice");
    Pieces::Move move(player);
    EXPECT_EQ(move.author(), player);
    EXPECT_EQ(move.actions().size(), 0);
}

TEST(MoveTest, Getters) {
    Player author("Bob");
    Pieces::Move move(author);
    EXPECT_EQ(move.author(), author);
    EXPECT_TRUE(move.actions().empty());
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

TEST(MoveTest, EqualityOperatorWithEmptyActions) {
    Player player1("Alice");
    Pieces::Move move1(player1);
    Pieces::Move move2(player1);
    Pieces::Move move3(Player("Bob"));

    EXPECT_TRUE(move1 == move2);
    EXPECT_FALSE(move1 == move3);
}

TEST(MoveTest, EqualityOperatorWithNonEmptyActions) {
    Player player("Alice");
    Pieces::Move move1(player);
    Pieces::Move move2(player);
    Pieces::Move move3(player);

    Pieces::King piece(Position(1, 1));
    Position initial(1, 1);
    Position final(2, 2);
    Pieces::Action action1(&piece, initial, final);
    Pieces::Action action2(&piece, final, initial);

    move1.add(action1);
    move1.add(action2);

    move2.add(action1);
    move2.add(action2);

    move3.add(action2);
    move3.add(action1);

    EXPECT_TRUE(move1 == move2);
    EXPECT_FALSE(move2 == move3);
}

TEST(MoveTest, Hash) {
    Player player1("Alice");
    Player player2("Bob");
    Pieces::Action action;

    Pieces::Move move1(player1);
    Pieces::Move move2(player1);
    Pieces::Move move3(player2);
    Pieces::Move move4(player2);
    Pieces::Move move5(player2);

    move4.add(action);
    move5.add(action);

    EXPECT_EQ(move1.hash(), move2.hash());
    EXPECT_NE(move1.hash(), move3.hash());
    EXPECT_NE(move3.hash(), move4.hash());
    EXPECT_EQ(move4.hash(), move5.hash());
}
