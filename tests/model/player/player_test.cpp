#include <gtest/gtest.h>

#include <model/player/player.hpp>

class MockPiece1 : public Pieces::Piece {
  public:
    MockPiece1(Position pos)
        : Piece(pos) {}
};

TEST(PlayerTest, DefaultConstructor) {
    Player player;
    EXPECT_EQ(player.name(), "");
    EXPECT_TRUE(player.pieces().empty());
}

TEST(PlayerTest, ParameterizedConstructor) {
    Player player("Alice");
    EXPECT_EQ(player.name(), "Alice");
    EXPECT_TRUE(player.pieces().empty());
}

TEST(PlayerTest, NameGetter) {
    Player player("Bob");
    EXPECT_EQ(player.name(), "Bob");
}

TEST(PlayerTest, EqualityOperator) {
    Player player1("Alice");
    Player player2("Alice");
    Player player3("Bob");

    EXPECT_TRUE(player1 == player2);
    EXPECT_FALSE(player1 == player3);
}

TEST(PlayerTest, Hash) {
    Player player1("Alice");
    Player player2("Alice");
    Player player3("Bob");

    EXPECT_EQ(player1.hash(), player2.hash());
    EXPECT_NE(player1.hash(), player3.hash());
}

TEST(PlayerTest, StreamOperator) {
    Player player("Henry");
    std::ostringstream os;
    os << player;
    EXPECT_EQ(os.str(), "Player(Henry)");
}
