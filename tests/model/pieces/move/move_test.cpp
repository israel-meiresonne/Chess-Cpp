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
