//unused
template <typename T>
class Matrix{
public:
    Matrix() = default;
    Matrix(int rows, int cols, T init_value)
        : rows(rows), cols(cols){
        allocArray();
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                matrix[i][j] = init_value;
            }
        }
    }

    ~Matrix(){
        for(int i = 0; i < rows; i++){
            delete [] matrix[i];
        }
        delete [] matrix;
    }

    void setValue(int x, int y, T val){
        matrix[x][y] = val;
    }
    T getValue(int x, int y) const{
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
    T** matrix;

    void allocArray(){
		matrix = new T*[rows];
		for(int i = 0; i < rows; i++)
		{
			matrix[i] = new T[cols];
		}
	}

};