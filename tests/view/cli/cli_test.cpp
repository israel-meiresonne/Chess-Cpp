#include "view/cli/cli.hpp"
#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include <vector>

class CLITest : public ::testing::Test {
  protected:
    View::CLI cli;
    std::streambuf *cinBackup;
    std::streambuf *coutBackup;
    std::istringstream inputStream;
    std::ostringstream outputStream;

    void SetUp() override {
        cinBackup = std::cin.rdbuf();
        coutBackup = std::cout.rdbuf();
        std::cout.rdbuf(outputStream.rdbuf());
    }

    void TearDown() override {
        std::cin.rdbuf(cinBackup);
        std::cout.rdbuf(coutBackup);
    }

    void mockInput(const std::string &input) {
        inputStream.str(input);
        inputStream.clear();
        std::cin.rdbuf(inputStream.rdbuf());
    }

    std::string getOutput() { return outputStream.str(); }
};

TEST_F(CLITest, InputReadsLine) {
    mockInput("hello\n");
    std::string result = cli.input();
    EXPECT_EQ(result, "hello");
}

TEST_F(CLITest, InputCheckerAcceptsValidInput) {
    mockInput("wrong\nanother\nabc123\n");
    std::string result = cli.inputChecker("^[a-z]{3}[0-9]{3}$", "Invalid format!");
    EXPECT_EQ(result, "abc123");

    std::string output = getOutput();
    EXPECT_NE(output.find("Invalid format!"), std::string::npos);
}

TEST_F(CLITest, MenuReturnsCorrectChoice) {
    mockInput("2\n");
    std::vector<std::string> choices = {"Play", "Quit"};
    std::string result = cli.menu(choices);
    EXPECT_EQ(result, "Quit");
}

TEST_F(CLITest, OutputGridPrintsCorrectly) {
    std::vector<std::vector<std::string>> grid = {{"X", "O"}, {"O", "X"}};
    cli.outputGrid(grid);

    std::string expected = "XO\nOX\n";
    std::string result = getOutput();

    auto isStyleChar = [](char c) {
        return c == '\033' || c == '[' || (c >= '0' && c <= '9') || c == 'm';
    };
    auto lastValidChartPtr = std::remove_if(result.begin(), result.end(), isStyleChar);
    result.erase(lastValidChartPtr, result.end());

    EXPECT_NE(result.find("XO"), std::string::npos);
    EXPECT_NE(result.find("OX"), std::string::npos);
}
