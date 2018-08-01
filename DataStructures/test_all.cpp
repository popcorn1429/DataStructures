#include "test_all.h"

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    int i = RUN_ALL_TESTS();
    system("pause");
    return i;
}