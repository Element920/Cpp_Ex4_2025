//israelmor555@gmail.com

//
// Created by israel on 6/9/25.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "MyContainer.hpp"
#include <sstream>
#include <string>

using namespace ariel;

TEST_CASE("MyContainer - Basic Operations") {
    MyContainer<int> container;

    SUBCASE("Empty container") {
        CHECK(container.size() == 0);
        CHECK(container.empty() == true);
    }

    SUBCASE("Adding elements") {
        container.add(5);
        CHECK(container.size() == 1);
        CHECK(container.empty() == false);

        container.add(10);
        container.add(3);
        CHECK(container.size() == 3);
    }
}
TEST_CASE("MyContainer - Add and Remove Interleaved") {
    MyContainer<int> container;

    container.add(10);
    container.add(20);
    container.remove(10);
    container.add(30);
    container.add(40);
    container.remove(20);
    container.add(50);

    std::vector<int> expected = {30, 40, 50};
    std::vector<int> actual;

    for (auto it = container.begin_order(); it != container.end_order(); ++it) {
        actual.push_back(*it);
    }

    CHECK(actual == expected);
    CHECK(container.size() == 3);
}
TEST_CASE("MyContainer - Copy Constructor") {
    MyContainer<int> original;
    original.add(1);
    original.add(2);
    original.add(3);

    MyContainer<int> copy = original;
    original.remove(2);

    std::vector<int> expected_original = {1, 3};
    std::vector<int> expected_copy = {1, 2, 3};

    std::vector<int> actual_original;
    std::vector<int> actual_copy;

    for (auto it = original.begin_ascending_order(); it != original.end_ascending_order(); ++it) {
        actual_original.push_back(*it);
    }

    for (auto it = copy.begin_ascending_order(); it != copy.end_ascending_order(); ++it) {
        actual_copy.push_back(*it);
    }

    CHECK(actual_original == expected_original);
    CHECK(actual_copy == expected_copy);
}


TEST_CASE("MyContainer - Char container") {
    MyContainer<char> chars;
    chars.add('c');
    chars.add('a');
    chars.add('b');

    std::vector<char> expected = {'a', 'b', 'c'};
    std::vector<char> actual;

    for (auto it = chars.begin_ascending_order(); it != chars.end_ascending_order(); ++it) {
        actual.push_back(*it);
    }

    CHECK(actual == expected);
}

TEST_CASE("MyContainer - Iterator on Empty Container") {
    MyContainer<int> empty;

    auto it = empty.begin_ascending_order();
    auto end = empty.end_ascending_order();

    CHECK(it == end); // בדיקה שה־iterator לא יוצר בעיות
}


TEST_CASE("MyContainer - Remove Operations") {
    MyContainer<int> container;
    container.add(7);
    container.add(15);
    container.add(6);
    container.add(15); // duplicate
    container.add(1);

    SUBCASE("Remove existing element") {
        container.remove(15);
        CHECK(container.size() == 3); // Both instances of 15 should be removed

        // Verify 15 is no longer in container by checking all iterators
        bool found_15 = false;
        for (auto it = container.begin_order(); it != container.end_order(); ++it) {
            if (*it == 15) {
                found_15 = true;
                break;
            }
        }
        CHECK(found_15 == false);
    }

    SUBCASE("Remove non-existing element") {
        CHECK_THROWS_AS(container.remove(99), std::runtime_error);
        CHECK(container.size() == 5); // Size should remain unchanged
    }

    SUBCASE("Remove from empty container") {
        MyContainer<int> empty_container;
        CHECK_THROWS_AS(empty_container.remove(5), std::runtime_error);
    }
}

TEST_CASE("MyContainer - AscendingOrder Iterator") {
    MyContainer<int> container;
    container.add(7);
    container.add(15);
    container.add(6);
    container.add(1);
    container.add(2);

    std::vector<int> expected = {1, 2, 6, 7, 15};
    std::vector<int> actual;

    for (auto it = container.begin_ascending_order(); it != container.end_ascending_order(); ++it) {
        actual.push_back(*it);
    }

    CHECK(actual == expected);
}

TEST_CASE("MyContainer - DescendingOrder Iterator") {
    MyContainer<int> container;
    container.add(7);
    container.add(15);
    container.add(6);
    container.add(1);
    container.add(2);

    std::vector<int> expected = {15, 7, 6, 2, 1};
    std::vector<int> actual;

    for (auto it = container.begin_descending_order(); it != container.end_descending_order(); ++it) {
        actual.push_back(*it);
    }

    CHECK(actual == expected);
}

TEST_CASE("MyContainer - SideCrossOrder Iterator") {
    MyContainer<int> container;
    container.add(7);
    container.add(15);
    container.add(6);
    container.add(1);
    container.add(2);

    // Expected: sorted = [1, 2, 6, 7, 15]
    // Side cross: 1, 15, 2, 7, 6
    std::vector<int> expected = {1, 15, 2, 7, 6};
    std::vector<int> actual;

    for (auto it = container.begin_side_cross_order(); it != container.end_side_cross_order(); ++it) {
        actual.push_back(*it);
    }

    CHECK(actual == expected);
}

TEST_CASE("MyContainer - ReverseOrder Iterator") {
    MyContainer<int> container;
    container.add(7);
    container.add(15);
    container.add(6);
    container.add(1);
    container.add(2);

    std::vector<int> expected = {2, 1, 6, 15, 7};
    std::vector<int> actual;

    for (auto it = container.begin_reverse_order(); it != container.end_reverse_order(); ++it) {
        actual.push_back(*it);
    }

    CHECK(actual == expected);
}

TEST_CASE("MyContainer - Order Iterator") {
    MyContainer<int> container;
    container.add(7);
    container.add(15);
    container.add(6);
    container.add(1);
    container.add(2);

    std::vector<int> expected = {7, 15, 6, 1, 2};
    std::vector<int> actual;

    for (auto it = container.begin_order(); it != container.end_order(); ++it) {
        actual.push_back(*it);
    }

    CHECK(actual == expected);
}

TEST_CASE("MyContainer - MiddleOutOrder Iterator") {
    MyContainer<int> container;
    container.add(7);
    container.add(15);
    container.add(6);
    container.add(1);
    container.add(2);

    // Original order: [7, 15, 6, 1, 2]
    // Middle index = 2, so start with 6
    // Then alternate: 15 (left), 1 (right), 7 (left), 2 (right)
    // Expected: 6, 15, 1, 7, 2
    std::vector<int> expected = {6, 15, 1, 7, 2};
    std::vector<int> actual;

    for (auto it = container.begin_middle_out_order(); it != container.end_middle_out_order(); ++it) {
        actual.push_back(*it);
    }

    CHECK(actual == expected);
}

TEST_CASE("MyContainer - Different Data Types") {
    SUBCASE("String container") {
        MyContainer<std::string> str_container;
        str_container.add("hello");
        str_container.add("world");
        str_container.add("apple");

        CHECK(str_container.size() == 3);

        std::vector<std::string> expected = {"apple", "hello", "world"};
        std::vector<std::string> actual;

        for (auto it = str_container.begin_ascending_order(); it != str_container.end_ascending_order(); ++it) {
            actual.push_back(*it);
        }

        CHECK(actual == expected);
    }

    SUBCASE("Double container") {
        MyContainer<double> double_container;
        double_container.add(3.14);
        double_container.add(2.71);
        double_container.add(1.41);

        CHECK(double_container.size() == 3);

        std::vector<double> expected = {1.41, 2.71, 3.14};
        std::vector<double> actual;

        for (auto it = double_container.begin_ascending_order(); it != double_container.end_ascending_order(); ++it) {
            actual.push_back(*it);
        }

        CHECK(actual == expected);
    }
}

TEST_CASE("MyContainer - Edge Cases") {
    SUBCASE("Single element") {
        MyContainer<int> container;
        container.add(42);

        CHECK(container.size() == 1);

        // Test all iterators with single element
        for (auto it = container.begin_ascending_order(); it != container.end_ascending_order(); ++it) {
            CHECK(*it == 42);
        }

        for (auto it = container.begin_descending_order(); it != container.end_descending_order(); ++it) {
            CHECK(*it == 42);
        }

        for (auto it = container.begin_middle_out_order(); it != container.end_middle_out_order(); ++it) {
            CHECK(*it == 42);
        }
    }

    SUBCASE("Two elements") {
        MyContainer<int> container;
        container.add(10);
        container.add(5);

        std::vector<int> asc_expected = {5, 10};
        std::vector<int> asc_actual;

        for (auto it = container.begin_ascending_order(); it != container.end_ascending_order(); ++it) {
            asc_actual.push_back(*it);
        }

        CHECK(asc_actual == asc_expected);

        std::vector<int> side_expected = {5, 10}; // smallest, largest
        std::vector<int> side_actual;

        for (auto it = container.begin_side_cross_order(); it != container.end_side_cross_order(); ++it) {
            side_actual.push_back(*it);
        }

        CHECK(side_actual == side_expected);
    }

    SUBCASE("Duplicate elements") {
        MyContainer<int> container;
        container.add(5);
        container.add(5);
        container.add(5);

        CHECK(container.size() == 3);

        std::vector<int> expected = {5, 5, 5};
        std::vector<int> actual;

        for (auto it = container.begin_ascending_order(); it != container.end_ascending_order(); ++it) {
            actual.push_back(*it);
        }

        CHECK(actual == expected);
    }
}

TEST_CASE("MyContainer - Output Stream Operator") {
    MyContainer<int> container;
    container.add(1);
    container.add(2);
    container.add(3);

    std::ostringstream oss;
    oss << container;

    std::string output = oss.str();
    CHECK(output.find("1") != std::string::npos);
    CHECK(output.find("2") != std::string::npos);
    CHECK(output.find("3") != std::string::npos);
}

TEST_CASE("MyContainer - Iterator Consistency") {
    MyContainer<int> container;
    container.add(10);
    container.add(20);
    container.add(30);

    SUBCASE("Multiple iterations should be consistent") {
        std::vector<int> first_iteration;
        std::vector<int> second_iteration;

        // First iteration
        for (auto it = container.begin_ascending_order(); it != container.end_ascending_order(); ++it) {
            first_iteration.push_back(*it);
        }

        // Second iteration
        for (auto it = container.begin_ascending_order(); it != container.end_ascending_order(); ++it) {
            second_iteration.push_back(*it);
        }

        CHECK(first_iteration == second_iteration);
    }
}