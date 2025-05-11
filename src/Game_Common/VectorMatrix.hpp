#pragma once
#include <array>
#include <vector>
#include <tuple> // Required for std::tuple
#include <memory>
#include <cassert>
#include <iostream>
#include <sstream> // Required for std::ostringstream



// Custom class to represent a matrix element
template <typename T>
class MatrixElement {
public:
    MatrixElement(T value, int row, int col) 
        : value(value), row(row), col(col) {}

    T getValue() const { return value; }
    int getRow() const { return row; }
    int getCol() const { return col; }

private:
    T value;
    int row;
    int col;
};

template <typename T>
class Matrix{
public:
    Matrix() = default;
    Matrix(int rows, int cols, T init_value)
        : rows(rows), cols(cols), values(rows * cols, init_value){
        // for(int i = 0; i < rows; i++){
        //     for(int j = 0; j < cols; j++){
        //         setValue(i, j, init_value);
        //     }
        // }
    }

    ~Matrix(){
    }

    Matrix(int rows, int cols, std::vector<T>& values) : rows(rows), cols(cols), values(values){
    }

    Matrix(const Matrix& other)
        : rows(other.get_num_rows()), cols(other.get_num_cols()), values(other.values) {
        // std::cout << "Copy constructor called\n";
    }
    template <typename U>
    Matrix(const Matrix<U>& other, T init_value)
        : rows(other.get_num_rows()), cols(other.get_num_cols()), values(other.get_num_rows() * other.get_num_cols()) {
        std::fill(values.begin(), values.end(), init_value); // Fill the vector with init_value
    }
    Matrix(const Matrix& other, T init_value)
        : rows(other.get_num_rows()), cols(other.get_num_cols()), values(other.get_num_rows() * other.get_num_cols()) {
        std::fill(values.begin(), values.end(), init_value); // Fill the vector with init_value
    }

    


    template <typename U>
    Matrix<decltype(T{} * U{})> operator*(const Matrix<U>& other) const {
        assert(rows == other.get_num_rows() && cols == other.get_num_cols() && "Matrix dimensions must match for multiplication");
    
        // Determine the resulting type from the multiplication (decltype)
        using ResultType = decltype(T{} * U{});
    
        // Create a new matrix to store the result
        Matrix<ResultType> new_matrix(rows, cols, ResultType{});
    
        // Perform element-wise multiplication
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                ResultType result = static_cast<ResultType>(this->getValue(i, j)) * static_cast<ResultType>(other.getValue(i, j));
                new_matrix.setValue(i, j, result);
            }
        }
    
        return new_matrix;
    }

    Matrix<T> operator*(float other) const {
    
        Matrix<T> new_matrix = Matrix(*this);
    
        // Perform element-wise multiplication
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                new_matrix.setValue(i, j, this->getValue(i, j) * other);
            }
        }
    
        return new_matrix;
    }

    Matrix<T> operator/(float other) const {
    
        Matrix<T> new_matrix = Matrix(*this);
    
        // Perform element-wise multiplication
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                new_matrix.setValue(i, j, this->getValue(i, j) / other);
            }
        }
    
        return new_matrix;
    }

    Matrix<T> replace(T x, T replacement) const {
        Matrix<T> replMatrix = Matrix<T>(*this);
        for (int i = 0; i < values.size(); i++){
            if (values[i] == x){
                replMatrix.values[i] = replacement;
            }
        }
        return replMatrix;
    }

    T sum() const {
        T total = T{}; // Initialize the sum with the default value of T (e.g., 0 for int, 0.0 for float)
        for (const T& value : values) {
            total += value;
        }
        return total;
    }

    int getPosition(int x, int y) const {
        return (int)(y + (cols) * x);
    }

    void setValue(int x, int y, T val){
        assert(x >= 0 && x < rows && y >= 0 && y < cols && "Index out of bounds");
        values[getPosition(x, y)]= val;
    }
    T getValue(int x, int y) const {
        assert(x >= 0 && x < rows && y >= 0 && y < cols && "Index out of bounds");
        return values[getPosition(x, y)];
    }

    std::vector<T>& getVector(){
        return values;
    }

    int get_num_rows() const {
        return rows;
    }
    int get_num_cols() const {
        return cols;
    }



    // Const iterator class
    class Iterator {
        public:
            Iterator(const Matrix& mat, int pos) : matrix(mat), pos(pos) {}
    
            // Dereference operator - returns tuple (value, row, col)
            MatrixElement<T> operator*() const {
                int row = pos / matrix.cols;
                int col = pos % matrix.cols;
                return MatrixElement<T>(matrix.values[pos], row, col);
            }
    
            // Prefix increment (++it)
            Iterator& operator++() {
                ++pos;
                return *this;
            }
    
            // Comparison operator
            bool operator!=(const Iterator& other) const {
                return pos != other.pos;
            }
            private:
            const Matrix& matrix;
            int pos;
        };

    // Begin and End functions for iterators
    Iterator begin() const { return Iterator(*this, 0); }
    Iterator end() const { return Iterator(*this, rows * cols); }

    std::ostream& print(std::ostream& os) const {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                os << getValue(i, j) << "\t";
            }
            os << "\n"; // Newline after each row
        }
        os << "\n"; // Newline after each row

        return os;
    }
    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
        return matrix.print(os);
    }

private:
    int rows;
    int cols;
    std::vector<T> values;

};
