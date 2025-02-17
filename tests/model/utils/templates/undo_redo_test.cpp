#include <gtest/gtest.h>

#include <model/utils/templates.hpp>

class UndoRedoTest : public ::testing::Test {
  protected:
    Utils::Templates::UndoRedo<int> undoRedo;
};

TEST_F(UndoRedoTest, DoAddsElement) {
    undoRedo.do_(5);
    EXPECT_EQ(undoRedo.peek(), 5);
}

TEST_F(UndoRedoTest, UndoMovesElementToUndone) {
    undoRedo.do_(10);
    int undone = undoRedo.undo();
    EXPECT_EQ(undone, 10);
    EXPECT_FALSE(undoRedo.canUndo());
    EXPECT_TRUE(undoRedo.canRedo());
}

TEST_F(UndoRedoTest, RedoMovesElementBackToDone) {
    undoRedo.do_(20);
    undoRedo.undo();
    int redone = undoRedo.redo();
    EXPECT_EQ(redone, 20);
    EXPECT_TRUE(undoRedo.canUndo());
    EXPECT_FALSE(undoRedo.canRedo());
}

TEST_F(UndoRedoTest, PeekReturnsLastDoneElement) {
    undoRedo.do_(30);
    EXPECT_EQ(undoRedo.peek(), 30);
    undoRedo.do_(40);
    EXPECT_EQ(undoRedo.peek(), 40);
}

TEST_F(UndoRedoTest, UndoRedoWorksWithMultipleOperations) {
    undoRedo.do_(1);
    undoRedo.do_(2);
    undoRedo.do_(3);

    EXPECT_EQ(undoRedo.undo(), 3);
    EXPECT_EQ(undoRedo.undo(), 2);
    EXPECT_EQ(undoRedo.redo(), 2);
    EXPECT_EQ(undoRedo.redo(), 3);
}

TEST_F(UndoRedoTest, UndoThrowsWhenEmpty) { EXPECT_THROW(undoRedo.undo(), std::runtime_error); }

TEST_F(UndoRedoTest, RedoThrowsWhenEmpty) { EXPECT_THROW(undoRedo.redo(), std::runtime_error); }

TEST_F(UndoRedoTest, PeekThrowsWhenEmpty) { EXPECT_THROW(undoRedo.peek(), std::runtime_error); }
