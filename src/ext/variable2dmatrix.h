#ifndef VARIABLEMATRIX
#define VARIABLEMATRIX

#include <QHash>
#include <cassert>


template<typename T>
/**
 * @brief A 2D matrix whose dimensions can be variable (defined at execution time)
 */
class Variable2DMatrix
{
private:
    int cols;
    int rows;
    QHash<int, QHash<int, T>> matrix;

public:
    Variable2DMatrix() : cols(0), rows(0) {}

    Variable2DMatrix(const int _cols, const int _rows) :
        cols(_cols), rows(_rows)
    {
        assert(_cols > 0 && _rows > 0);

        for (int i=0; i<cols; i++)
        {
            matrix[i] = QHash<int, T>();
            matrix[i].reserve(_rows);
        }
    }

    void fill(const T value)
    {
        for (int i=0; i<cols; i++)
            for (int j=0; j<rows; j++)
                matrix[i][j] = value;
    }

    T& operator()(const int col, const int row)
    {
        assert(col >= 0 && col < cols);
        assert(row >= 0 && row < rows);

        return matrix[col][row];
    }
};

#endif // VARIABLEMATRIX
