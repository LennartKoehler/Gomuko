#include <array>

template <typename T>
class Matrix{
public:
    Matrix() = default;
    Matrix(int rows, int cols, T init_value)
        : rows(rows), cols(cols){
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                setValue(i, j, init_value);
            }
        }
    }

    ~Matrix(){
    }

    int getPosition(int x, int y){
        return (int)(x + (cols-1) * y);
    }

    void setValue(int x, int y, T val){
        matrix[getPosition(x, y)]= val;
    }
    T getValue(int x, int y){
        return matrix[getPosition(x, y)];
    }

    int get_num_rows() const {
        return rows;
    }
    int get_num_cols() const {
        return cols;
    }


private:
    int rows;
    int cols;
    std::array<T, 400> matrix; // TODO this has to be set at compile time, otherwise it wont be readable

};