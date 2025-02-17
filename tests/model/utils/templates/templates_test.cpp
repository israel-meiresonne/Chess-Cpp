#include <gtest/gtest.h>
#include <vector>

#include <model/utils/templates.hpp>

class MockObject {
  public:
    int hash() const {
        return 42; // Simple mock hash value for testing
    }
};

TEST(UtilsTemplatesTest, HashTest) {
    MockObject obj;
    EXPECT_EQ(Utils::Templates::hash(obj), 42);
}

TEST(UtilsTemplatesTest, HashVectorTest) {
    std::vector<MockObject> vec(3);

    size_t expected = 3; // The size of the vector is 3.
    for (size_t i = 0; i < 3; ++i) {
        expected ^= 42 + 0x9e3779b9 + (expected << 6) + (expected >> 2);
    }

    EXPECT_EQ(Utils::Templates::hash_vector(vec), static_cast<int>(expected));
}

TEST(UtilsTemplatesTest, HashVectorEmptyTest) {
    std::vector<MockObject> vec;
    EXPECT_EQ(Utils::Templates::hash_vector(vec), 0);
}
