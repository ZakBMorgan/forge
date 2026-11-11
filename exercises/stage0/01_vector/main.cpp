#include <iostream>
#include <vector>
#include <cstddef>
#include <limits>
#include <stdexcept>
#include <cassert>
#include <utility>

std::size_t validated_element_count(
    std::size_t rows,
    std::size_t cols
);

std::size_t index(
    std::size_t row,
    std::size_t col,
    std::size_t num_rows,
    std::size_t num_cols
);

class Tensor2D {
private:
    std::size_t rows = 0;
    std::size_t cols = 0;
    std::vector<float> data;

public:
    Tensor2D(std::size_t rows, std::size_t cols)
        : rows(rows), cols(cols), data(validated_element_count(rows, cols)) {}
    
    Tensor2D(const Tensor2D&) = default;

    Tensor2D& operator=(const Tensor2D&) = default;
    
    Tensor2D(Tensor2D&& other) noexcept
        : rows(other.rows), cols(other.cols), data(std::move(other.data)) {
            other.rows = 0;
            other.cols = 0;
    }
    
    Tensor2D& operator=(Tensor2D&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        data = std::move(other.data);

        rows = other.rows;
        cols = other.cols;

        other.rows = 0;
        other.cols = 0;

        return *this;
    }
    
    std::size_t size() const {
        return data.size();
    }

    std::size_t num_rows() const {
        return rows;
    }

    std::size_t num_cols() const {
        return cols;
    }

    float& at(std::size_t row, std::size_t col) {
        return data[index(row, col, rows, cols)];
    }

    const float& at(std::size_t row, std::size_t col) const {
        return data[index(row, col, rows, cols)];
    }

    
};

std::size_t validated_element_count(std::size_t rows, std::size_t cols) {
    if (rows == 0 || cols == 0) {
        throw std::invalid_argument("Dimensions must be positive");
    }

    if (rows > std::numeric_limits<std::size_t>::max() / cols) {
        throw std::overflow_error("Tensor size overflow");
    }

    std::size_t count = rows * cols;

    return count;
}

std::size_t index(
    std::size_t row,
    std::size_t col,
    std::size_t num_rows,
    std::size_t num_cols
) {
    if (row >= num_rows || col >= num_cols) {
        throw std::out_of_range("Tensor index out of bounds");
    }

    std::size_t offset = row * num_cols + col;

    return offset;
}

int main() {
    
    assert(index(0, 0, 2, 3) == 0);
    assert(index(1, 1, 2, 3) == 4);
    assert(index(1, 2, 2, 3) == 5);

    try {
        index(2, 0, 2, 3);
        std::cout << "ERROR: exception was not thrown\n";
        return 1;
    }
    catch (const std::out_of_range& e) {
        std::cout << "PASS: invalid index rejected\n";
    }

    try {
        index(0, 3, 2, 3);
        std::cout << "ERROR: exception was not thrown\n";
        return 1;
    }
    catch (const std::out_of_range& e) {
        std::cout << "PASS: invalid index rejected\n";
    }

    Tensor2D tensor(2, 3);

    assert(tensor.size() == 6);
    assert(tensor.num_rows() == 2);
    assert(tensor.num_cols() == 3);

    assert(Tensor2D(2, 3).size() == 6);
    assert(Tensor2D(1, 1).size() == 1);

    try {
        Tensor2D tensor(0, 3);
        std::cout << "ERROR: exception was not thrown\n";
        return 1;
    }
    catch (const std::invalid_argument& e) {
        std::cout << "PASS: zero rows rejected\n";
    }

    try {
        Tensor2D tensor(3, 0);
        std::cout << "ERROR: exception was not thrown\n";
        return 1;
    }
    catch (const std::invalid_argument& e) {
        std::cout << "PASS: zero columns rejected\n";
    }

    try {
        Tensor2D tensor(std::numeric_limits<std::size_t>::max(), 2);
        std::cout << "ERROR: exception was not thrown\n";
        return 1;
    }
    catch (const std::overflow_error& e) {
        std::cout << "PASS: tensor size overflow rejected\n";
    }

    tensor.at(1, 2) = 42.0f;

    assert(tensor.at(1, 2) == 42.0f);
    assert(tensor.at(0, 0) == 0.0f);
    
    try {
        Tensor2D tensor(2, 3);
        (void)tensor.at(2,0);
        std::cout << "ERROR: exception was not thrown\n";
        return 1;
    }
    catch (const std::out_of_range& e) {
        std::cout << "PASS: invalid index rejected\n";
    }

    const Tensor2D tensor2(2, 3);

    assert(tensor2.at(0, 0) == 0.0f);
    assert(tensor2.at(1, 2) == 0.0f);

    // Move constructor test
    {
        Tensor2D a(2, 3);
        a.at(0, 0) = 10.0f;

        Tensor2D b = std::move(a);

        assert(b.at(0, 0) == 10.0f);

        std::cout << "a rows: " << a.num_rows() << '\n';
        std::cout << "a cols: " << a.num_cols() << '\n';
        std::cout << "a size: " << a.size() << '\n';

        assert(b.num_rows() == 2);
        assert(b.num_cols() == 3);
        assert(b.size() == 6);
        assert(b.at(0, 0) == 10.0f);

        assert(a.num_rows() == 0);
        assert(a.num_cols() == 0);
        assert(a.size() == 0);
    }

    Tensor2D original(2, 3);
    original.at(0, 0) = 7.0f;

    Tensor2D copied = original;
    copied.at(0, 0) = 99.0f;

    assert(original.at(0, 0) == 7.0f);
    assert(copied.at(0, 0) == 99.0f);

    // Move self-assignment test
    {
        Tensor2D a(2, 3);
        a.at(0, 0) = 42.0f;

        a = std::move(a);

        assert(a.num_rows() == 2);
        assert(a.num_cols() == 3);
        assert(a.size() == 6);
        assert(a.at(0, 0) == 42.0f);
    }

    {
        Tensor2D a(2, 3);
        a.at(0, 0) = 10.0f;

        Tensor2D b(4, 5);
        b = a;

        assert(b.num_rows() == 2);
        assert(b.num_cols() == 3);
        assert(b.size() == 6);
        assert(b.at(0, 0) == 10.0f);

        b.at(0, 0) = 99.0f;
        assert(a.at(0, 0) == 10.0f);
    }

    {
        Tensor2D a(2, 3);
        Tensor2D b(4, 5);

        a.at(0, 0) = 17.0f;

        b = std::move(a);

        assert(b.num_rows() == 2);
        assert(b.num_cols() == 3);
        assert(b.size() == 6);
        assert(b.at(0, 0) == 17.0f);

        assert(a.num_rows() == 0);
        assert(a.num_cols() == 0);
        assert(a.size() == 0);
    }

    return 0;
}