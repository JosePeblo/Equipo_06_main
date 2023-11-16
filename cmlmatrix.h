#pragma once
#include <stdexcept>

namespace cml { // Cool math library

// Simple wrapper to create and access heap allocated multidimensional arrays.
template<typename T>
class matrix {
public:
    matrix()
    {
        this->cols = 0;
        this->rows = 0;
        this->mat = nullptr;
    }

    matrix(int _rows, int _cols): rows(_rows), cols(_cols)
    {
        mat = new T[_rows * _cols];
        for(int i = 0; i < rows * cols; i++)
        {
            mat[i] = 0;
        }
    }

    matrix(matrix& that)
    {
        this->rows = that.rows;
        this->cols = that.cols;
        this->mat = new T[this->rows * this->cols];
        for(int i = 0; i < this->rows * this->cols; i++)
        {
            this->mat[i] = that.mat[i];
        }
    }

    ~matrix()
    {
        delete [] mat;
        mat = nullptr;
        cols = 0;
        rows = 0;
    }

    struct row {
        T* mat;
        int cols;
        row(T* _mat, int _cols): mat(_mat), cols(_cols){}
        T& operator[](int _col)
        {
            if(_col >=  cols || _col < 0) 
                throw std::runtime_error("Column index out of range");
            return *(mat + _col);
        }

    };

    row operator[](int _row)
    {
        if(_row >= rows || _row < 0) 
            throw std::runtime_error("Row index out of range");
        return row(mat + cols * _row, cols);
    }

    int Rows() const { return this->rows; }
    int Cols() const { return this->cols; }

    void print()
    {
        for(int i = 0; i < rows; i++)
        {
            for(int j = 0; j < cols; j++)
            {
                std::cout << mat[i * cols + j];
                if(j + 1 < cols)
                    std::cout << ' ';
            }
            std::cout << '\n';
        }
    }

private:
    int rows, cols;
    T* mat;
};

}
