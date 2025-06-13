//israelmor555@gmail.com
#ifndef MYCONTAINER_HPP
#define MYCONTAINER_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <stdexcept>

namespace ariel {
template<typename T>
class MyContainer {
    std::vector<T> data;

public:
    void add(const T& value) {
        data.push_back(value);
    }

    // Remove method - removes all instances of the value
    void remove(const T& value) {
        auto it = std::find(data.begin(), data.end(), value);
        if (it == data.end()) {
            throw std::runtime_error("Element not found in container");
        }

        // Remove all instances of the value
        data.erase(std::remove(data.begin(), data.end(), value), data.end());
    }

    size_t size() const {
        return data.size();
    }

    bool empty() const {
        return data.empty();
    }

    // Output stream operator
    friend std::ostream& operator<<(std::ostream& os, const MyContainer<T>& container) {
        os << "[";
        for (size_t i = 0; i < container.data.size(); ++i) {
            os << container.data[i];
            if (i < container.data.size() - 1) {
                os << ", ";
            }
        }
        os << "]";
        return os;
    }

    // --- Order Iterator ---
    class OrderIterator {
        typename std::vector<T>::iterator it;
    public:
        OrderIterator(typename std::vector<T>::iterator it) : it(it) {}
        T& operator*() { return *it; }
        OrderIterator& operator++() { ++it; return *this; }
        bool operator!=(const OrderIterator& other) const { return it != other.it; }
        bool operator==(const OrderIterator& other) const { return it == other.it; }
    };
    OrderIterator begin_order() { return OrderIterator(data.begin()); }
    OrderIterator end_order() { return OrderIterator(data.end()); }

    // --- Ascending Order Iterator ---
    class AscendingOrderIterator {
        std::vector<T> sorted;
        size_t index;
    public:
        AscendingOrderIterator(const std::vector<T>& original, bool is_end = false) {
            sorted = original;
            std::sort(sorted.begin(), sorted.end());
            index = is_end ? sorted.size() : 0;
        }
        T& operator*() {
            if (index >= sorted.size()) {
                throw std::out_of_range("Iterator out of range");
            }
            return sorted[index];
        }
        AscendingOrderIterator& operator++() { ++index; return *this; }
        bool operator!=(const AscendingOrderIterator& other) const { return index != other.index; }
        bool operator==(const AscendingOrderIterator& other) const { return index == other.index; }
    };
    AscendingOrderIterator begin_ascending_order() { return AscendingOrderIterator(data); }
    AscendingOrderIterator end_ascending_order() { return AscendingOrderIterator(data, true); }

    // --- Descending Order Iterator ---
    class DescendingOrderIterator {
        std::vector<T> sorted;
        size_t index;
    public:
        DescendingOrderIterator(const std::vector<T>& original, bool is_end = false) {
            sorted = original;
            std::sort(sorted.begin(), sorted.end(), std::greater<T>());
            index = is_end ? sorted.size() : 0;
        }
        T& operator*() {
            if (index >= sorted.size()) {
                throw std::out_of_range("Iterator out of range");
            }
            return sorted[index];
        }
        DescendingOrderIterator& operator++() { ++index; return *this; }
        bool operator!=(const DescendingOrderIterator& other) const { return index != other.index; }
        bool operator==(const DescendingOrderIterator& other) const { return index == other.index; }
    };
    DescendingOrderIterator begin_descending_order() { return DescendingOrderIterator(data); }
    DescendingOrderIterator end_descending_order() { return DescendingOrderIterator(data, true); }

    // --- SideCrossOrder Iterator - FIXED LOGIC ---
    class SideCrossOrderIterator {
        std::vector<T> order;
        size_t index;
    public:
        SideCrossOrderIterator(const std::vector<T>& original, bool is_end = false) {
            if (original.empty()) {
                index = 0;
                return;
            }

            std::vector<T> sorted = original;
            std::sort(sorted.begin(), sorted.end());

            size_t left = 0;
            size_t right = sorted.size() - 1;

            while (left <= right) {
                order.push_back(sorted[left]);
                if (left != right) {  // Only add right if it's different from left
                    order.push_back(sorted[right]);
                }
                ++left;
                if (right == 0) break;  // Prevent underflow
                --right;
            }

            index = is_end ? order.size() : 0;
        }
        T& operator*() {
            if (index >= order.size()) {
                throw std::out_of_range("Iterator out of range");
            }
            return order[index];
        }
        SideCrossOrderIterator& operator++() { ++index; return *this; }
        bool operator!=(const SideCrossOrderIterator& other) const { return index != other.index; }
        bool operator==(const SideCrossOrderIterator& other) const { return index == other.index; }
    };
    SideCrossOrderIterator begin_side_cross_order() { return SideCrossOrderIterator(data); }
    SideCrossOrderIterator end_side_cross_order() { return SideCrossOrderIterator(data, true); }

    // --- ReverseOrder Iterator ---
    class ReverseOrderIterator {
        std::vector<T> reversed;
        size_t index;
    public:
        ReverseOrderIterator(const std::vector<T>& original, bool is_end = false) {
            reversed = original;
            std::reverse(reversed.begin(), reversed.end());
            index = is_end ? reversed.size() : 0;
        }
        T& operator*() {
            if (index >= reversed.size()) {
                throw std::out_of_range("Iterator out of range");
            }
            return reversed[index];
        }
        ReverseOrderIterator& operator++() { ++index; return *this; }
        bool operator!=(const ReverseOrderIterator& other) const { return index != other.index; }
        bool operator==(const ReverseOrderIterator& other) const { return index == other.index; }
    };
    ReverseOrderIterator begin_reverse_order() { return ReverseOrderIterator(data); }
    ReverseOrderIterator end_reverse_order() { return ReverseOrderIterator(data, true); }

    // --- MiddleOutOrder Iterator - FIXED LOGIC ---
    class MiddleOutOrderIterator {
        std::vector<T> order;
        size_t index;
    public:
        MiddleOutOrderIterator(const std::vector<T>& original, bool is_end = false) {
            if (original.empty()) {
                index = 0;
                return;
            }

            size_t mid = original.size() / 2;
            std::vector<T> temp = original;
            order.push_back(temp[mid]);

            // Use signed integers to handle the alternating pattern correctly
            int left = static_cast<int>(mid) - 1;
            int right = static_cast<int>(mid) + 1;

            bool go_left = true;

            while (left >= 0 || right < static_cast<int>(temp.size())) {
                if (go_left && left >= 0) {
                    order.push_back(temp[left]);
                    --left;
                } else if (!go_left && right < static_cast<int>(temp.size())) {
                    order.push_back(temp[right]);
                    ++right;
                }
                go_left = !go_left;
            }

            index = is_end ? order.size() : 0;
        }
        T& operator*() {
            if (index >= order.size()) {
                throw std::out_of_range("Iterator out of range");
            }
            return order[index];
        }
        MiddleOutOrderIterator& operator++() { ++index; return *this; }
        bool operator!=(const MiddleOutOrderIterator& other) const { return index != other.index; }
        bool operator==(const MiddleOutOrderIterator& other) const { return index == other.index; }
    };
    MiddleOutOrderIterator begin_middle_out_order() { return MiddleOutOrderIterator(data); }
    MiddleOutOrderIterator end_middle_out_order() { return MiddleOutOrderIterator(data, true); }
};

}
#endif // MYCONTAINER_HPP