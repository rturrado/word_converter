#include <gtest/gtest.h>
#include <iostream>  // cout


void print_usage(std::ostream& os) {
    os << "Usage:\n";
    os << "\tword_converter_test\n";
}


int main_impl(int argc, char** argv, std::ostream& os) {
    ::testing::InitGoogleTest(&argc, argv);

    if (argc != 1) {
        print_usage(os);
        return 0;
    }

    return RUN_ALL_TESTS();
}


int main(int argc, char** argv) {
    return main_impl(argc, argv, std::cout);
}
