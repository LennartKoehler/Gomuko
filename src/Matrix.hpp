#include <iostream>


class Matrix{
public:
    Matrix() = default;
    Matrix(int rows, int cols)
        : rows(rows), cols(cols){
        allocArray();
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                matrix[i][j] = 0;
            }
        }
    }

    ~Matrix(){
        std::cerr << "deleting";
        for(int i = 0; i < rows; i++){
            delete [] matrix[i];
        }
        delete [] matrix;
    }

    void setValue(int x, int y, int val){
        matrix[x][y] = val;
    }
    int getValue(int x, int y) const{
        return matrix[x][y];
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
    int** matrix;

    void allocArray(){
		matrix = new int*[rows];
		for(int i = 0; i < rows; i++)
		{
			matrix[i] = new int[cols];
		}
	}

};