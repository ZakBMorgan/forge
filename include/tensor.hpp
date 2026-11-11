#pragma once

#include <cstddef>
#include <vector>

class Tensor2D {
private:
    std::size_t rows = 0;
    std::size_t cols = 0;
    std::vector<float> data;

public:
    // Construction
    Tensor2D(std::size_t rows, std::size_t cols);
    Tensor2D(const Tensor2D&) = default;
    Tensor2D(Tensor2D&& other) noexcept;

    // Assignment
    Tensor2D& operator=(const Tensor2D&) = default;
    Tensor2D& operator=(Tensor2D&& other) noexcept;

    // Inspection
    std::size_t size() const;
    std::size_t num_rows() const;
    std::size_t num_cols() const;

    // Element access
    float& at(std::size_t row, std::size_t col);
    const float& at(std::size_t row, std::size_t col) const;

    
};