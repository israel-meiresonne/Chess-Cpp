#ifndef PIECES_TEST_HPP
#define PIECES_TEST_HPP

#include <gtest/gtest.h>

#include <model/pieces/pieces.hpp>

namespace Tests {
    namespace Pieces {
        class MockPiece1 : public ::Pieces::Piece {
          public:
            MockPiece1()
                : Piece() {}
            MockPiece1(const Position &position)
                : Piece(position) {}

            std::unordered_map<Position, ::Pieces::Move> &
            _genMovesInDirection(std::unordered_map<Position, ::Pieces::Move> &moves, Position end,
                                 ::Pieces::Move::Direction direction,
                                 std::unordered_map<Position, ::Pieces::Move> &captures) {
                return genMovesInDirection(moves, end, direction, captures);
            }

            std::unordered_map<Position, ::Pieces::Move> &
            _genCapturesInDirection(std::unordered_map<Position, ::Pieces::Move> &moves,
                                    Position end, ::Pieces::Move::Direction direction) {
                return genCapturesInDirection(moves, end, direction);
            }

            const std::unordered_map<Position, ::Pieces::Piece> &callFriendlies() {
                return friendlies();
            };
            void
            callSetFriendlies(const std::unordered_map<Position, ::Pieces::Piece> &friendlies) {
                this->friendlies(friendlies);
            };

            const std::unordered_map<Position, ::Pieces::Piece> &callOpponents() {
                return opponents();
            };
            void callSetOpponents(const std::unordered_map<Position, ::Pieces::Piece> &opponents) {
                this->opponents(opponents);
            };

          protected:
            std::unordered_map<Position, ::Pieces::Move> &
            _moves(std::unordered_map<Position, ::Pieces::Move> &moves, int &nRow,
                   int &nColumn) override {
                return moves;
            }
        };

        class MockPiece2 : public MockPiece1 {
          public:
            MockPiece2(const Position &position)
                : MockPiece1(position) {}
        };

        class MockPiece3 : public MockPiece1 {
          public:
            MockPiece3(const Position &position)
                : MockPiece1(position) {}
        };

        class PiecesTest : public ::testing::Test {
          protected:
            std::pair<int, int> boundaries;
            int nRow;
            int nColumn;
            ::Position initialPosition;
            std::unordered_map<::Position, ::Pieces::Move> moves;
            std::unordered_map<::Position, ::Pieces::Piece> friendlies;
            std::unordered_map<::Position, ::Pieces::Piece> opponents;

            void SetUp() override {
                boundaries = {8, 8};
                nRow = boundaries.first;
                nColumn = boundaries.second;
                initialPosition = Position(3, 3);
            }

            void TearDown() override {
                friendlies.clear();
                opponents.clear();
                moves.clear();
            }

            void addOpponentAt(::Position position) { opponents[position] = MockPiece1(position); }

            void addFriendlyAt(::Position position) { friendlies[position] = MockPiece1(position); }
        };

    } // namespace Pieces

} // namespace Tests

#endif // PIECES_TEST_HPP
