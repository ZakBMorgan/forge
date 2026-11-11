
#include "tensor.hpp"

#include <cstdlib>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <utility>

void check_impl(bool condition, const char* expression, int line) {
    if (!condition) {
        std::cerr << "FAIL at line " << line
                  << ": " << expression << '\n';
        std::exit(EXIT_FAILURE);
    }
}

#define CHECK(expr) check_impl((expr), #expr, __LINE__)

int main() {

    // 1. Construction and dimensions
    {
        Tensor2D tensor(2, 3);

        CHECK(tensor.size() == 6);
        CHECK(tensor.num_rows() == 2);
        CHECK(tensor.num_cols() == 3);

        CHECK(Tensor2D(1, 1).size() == 1);
    }

    // 2. Element access and indexing
    {
        Tensor2D tensor(2, 3);

        // Write a unique value to every index
        for (std::size_t row = 0; row < tensor.num_rows(); ++row) {
            for (std::size_t col = 0; col < tensor.num_cols(); ++col) {
                tensor.at(row, col) = static_cast<float>(
                    row * tensor.num_cols() + col + 1
                );
            }
        }

        // Verify every index contains the expected value
        for (std::size_t row = 0; row < tensor.num_rows(); ++row) {
            for (std::size_t col = 0; col < tensor.num_cols(); ++col) {
                float expected = static_cast<float>(
                    row * tensor.num_cols() + col + 1
                );

                CHECK(tensor.at(row, col) == expected);
            }
        }
    }

    // 3. Bounds CHECKing
    {
        Tensor2D tensor(2, 3);

        try {
            (void)tensor.at(2, 0);
            std::cout << "ERROR: invalid row accepted\n";
            return 1;
        }
        catch (const std::out_of_range&) {
            std::cout << "PASS: invalid row rejected\n";
        }

        try {
            (void)tensor.at(0, 3);
            std::cout << "ERROR: invalid column accepted\n";
            return 1;
        }
        catch (const std::out_of_range&) {
            std::cout << "PASS: invalid column rejected\n";
        }
    }

    // 4. Invalid construction
    {
        try {
            Tensor2D tensor(0, 3);
            std::cout << "ERROR: zero rows accepted\n";
            return 1;
        }
        catch (const std::invalid_argument&) {
            std::cout << "PASS: zero rows rejected\n";
        }

        try {
            Tensor2D tensor(3, 0);
            std::cout << "ERROR: zero columns accepted\n";
            return 1;
        }
        catch (const std::invalid_argument&) {
            std::cout << "PASS: zero columns rejected\n";
        }

        try {
            Tensor2D tensor(
                std::numeric_limits<std::size_t>::max(), 2
            );
            std::cout << "ERROR: tensor size overflow accepted\n";
            return 1;
        }
        catch (const std::overflow_error&) {
            std::cout << "PASS: tensor size overflow rejected\n";
        }
    }

    // 5. Const correctness
    {
        Tensor2D tensor(2, 3);
        tensor.at(1, 2) = 42.0f;

        const Tensor2D& view = tensor;

        CHECK(view.at(1, 2) == 42.0f);
        CHECK(view.at(0, 0) == 0.0f);
    }

    // 6. Copy construction
    {
        Tensor2D original(2, 3);
        original.at(0, 0) = 7.0f;

        Tensor2D copied = original;
        copied.at(0, 0) = 99.0f;

        CHECK(original.at(0, 0) == 7.0f);
        CHECK(copied.at(0, 0) == 99.0f);

        CHECK(copied.num_rows() == 2);
        CHECK(copied.num_cols() == 3);
        CHECK(copied.size() == 6);
    }

    // 7. Copy assignment
    {
        Tensor2D a(2, 3);
        a.at(0, 0) = 10.0f;

        Tensor2D b(4, 5);
        b = a;

        CHECK(b.num_rows() == 2);
        CHECK(b.num_cols() == 3);
        CHECK(b.size() == 6);
        CHECK(b.at(0, 0) == 10.0f);

        b.at(0, 0) = 99.0f;
        CHECK(a.at(0, 0) == 10.0f);
    }

    // 8. Move construction
    {
        Tensor2D a(2, 3);
        a.at(0, 0) = 10.0f;

        Tensor2D b = std::move(a);

        CHECK(b.num_rows() == 2);
        CHECK(b.num_cols() == 3);
        CHECK(b.size() == 6);
        CHECK(b.at(0, 0) == 10.0f);

        CHECK(a.num_rows() == 0);
        CHECK(a.num_cols() == 0);
        CHECK(a.size() == 0);
    }

    // 9. Move assignment
    {
        Tensor2D a(2, 3);
        Tensor2D b(4, 5);

        a.at(0, 0) = 17.0f;

        b = std::move(a);

        CHECK(b.num_rows() == 2);
        CHECK(b.num_cols() == 3);
        CHECK(b.size() == 6);
        CHECK(b.at(0, 0) == 17.0f);

        CHECK(a.num_rows() == 0);
        CHECK(a.num_cols() == 0);
        CHECK(a.size() == 0);
    }

    // 10. Copy self-assignment
    {
        Tensor2D tensor(2, 3);
        tensor.at(0, 0) = 42.0f;

        Tensor2D& alias = tensor;
        tensor = alias;

        CHECK(tensor.num_rows() == 2);
        CHECK(tensor.num_cols() == 3);
        CHECK(tensor.size() == 6);
        CHECK(tensor.at(0, 0) == 42.0f);
    }

    // 11. Move self-assignment
    {
        Tensor2D a(2, 3);
        a.at(0, 0) = 42.0f;

        Tensor2D& alias = a;
        a = std::move(alias);

        CHECK(a.num_rows() == 2);
        CHECK(a.num_cols() == 3);
        CHECK(a.size() == 6);
        CHECK(a.at(0, 0) == 42.0f);
    }

    std::cout << "All Tensor2D tests passed.\n";
    return 0;
}
