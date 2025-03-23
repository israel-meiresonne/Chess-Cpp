#include <gtest/gtest.h>

#include <model/pieces/pieces.hpp>

class MockPiece1 : public Pieces::Piece {
  public:
    MockPiece1(Position pos)
        : Piece(pos) {}
};

TEST(PlayerTest, DefaultConstructor) {
    Pieces::Player player;
    EXPECT_EQ(player.name(), "");
}

TEST(PlayerTest, ParameterizedConstructor) {
    Pieces::Player player("Alice");
    EXPECT_EQ(player.name(), "Alice");
}

TEST(PlayerTest, NameGetter) {
    Pieces::Player player("Bob");
    EXPECT_EQ(player.name(), "Bob");
}

TEST(PlayerTest, EqualityOperator) {
    Pieces::Player player1("Alice");
    Pieces::Player player2("Alice");
    Pieces::Player player3("Bob");

    EXPECT_TRUE(player1 == player2);
    EXPECT_FALSE(player1 == player3);
}

TEST(PlayerTest, Hash) {
    Pieces::Player player1("Alice");
    Pieces::Player player2("Alice");
    Pieces::Player player3("Bob");

    EXPECT_EQ(player1.hash(), player2.hash());
    EXPECT_NE(player1.hash(), player3.hash());
}

TEST(PlayerTest, StringConversion) {
    Pieces::Player player("Alice");
    std::string playerStr = player;

    EXPECT_EQ(playerStr, "Player(Alice)");
}

TEST(PlayerTest, StreamOperator) {
    Pieces::Player player("Alice");
    std::ostringstream os;
    os << player;

    EXPECT_EQ(os.str(), "Player(Alice)");
}
