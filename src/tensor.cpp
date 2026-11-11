#include "tensor.hpp"

#include <limits>
#include <stdexcept>
#include <utility>

// Internal helper functions
namespace {

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

} // namespace

Tensor2D::Tensor2D(std::size_t rows, std::size_t cols) 
    : rows(rows), cols(cols), data(validated_element_count(rows, cols)) {}
    
    
Tensor2D::Tensor2D(Tensor2D&& other) noexcept
    : rows(other.rows), cols(other.cols), data(std::move(other.data)) {
        other.rows = 0;
        other.cols = 0;
}
    
Tensor2D& Tensor2D::operator=(Tensor2D&& other) noexcept {
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
    
std::size_t Tensor2D::size() const {
    return data.size();
}

std::size_t Tensor2D::num_rows() const {
    return rows;
}

std::size_t Tensor2D::num_cols() const {
    return cols;
}

float& Tensor2D::at(std::size_t row, std::size_t col) {
    return data[index(row, col, rows, cols)];
}

const float& Tensor2D::at(std::size_t row, std::size_t col) const {
    return data[index(row, col, rows, cols)];
}
