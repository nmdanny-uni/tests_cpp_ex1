//
// Created by Daniel Kerbel on 12/14/2019.
//

#ifndef UTIL_H
#define UTIL_H

#include "catch.hpp"
#include "Matrix.h"
#include <sstream>

Matrix mkMatrix(const std::initializer_list<std::initializer_list<float>>& rows)
{
    int numRows = rows.size();
    int numCols = rows.begin()->size();
    Matrix matrix(numRows, numCols);
    int inserted = 0;
    for (auto row = rows.begin(); row != rows.end(); ++row)
    {
        auto col = *row;
        assert((int)col.size() == numCols);

        for (float elm: col)
        {
            matrix[inserted++] = elm;
        }
    }
    return matrix;
}



namespace Catch {
    template<>
    struct StringMaker<Matrix> {
        static std::string convert(const Matrix& matrix)
        {

            std::stringstream ss;
            ss << "[" << matrix.getRows() << "x" << matrix.getCols() << "] = {" << std::endl;
            for (int i=0; i < matrix.getRows(); ++i)
            {
                ss << "{";
                for (int j=0; j < matrix.getCols(); ++j)
                {
                    ss << matrix(i,j) << " ";
                }
                ss << "}" << std::endl;
            }
            ss << "}";
            return ss.str();
        }
    };
}


class MatrixMatcher: public Catch::MatcherBase<Matrix>
{
private:
    Matrix _b;
public:
    MatrixMatcher(const Matrix &b)
        : _b(b)
    {}

    bool match(const Matrix &a) const override
    {
        if (a.getRows() != _b.getRows() || a.getCols() != _b.getCols())
        {
            return false;
        }
        for (int ix = 0; ix < a.getRows() * a.getCols(); ++ix)
        {
            if (a[ix] != _b[ix])
            {
                return false;
            }
        }
        return true;
    }

    std::string describe() const override
    {
        std::stringstream ss;
        ss << "equals the matrix ";
        ss << Catch::StringMaker<Matrix>::convert(_b);
        return ss.str();
    };
};

inline MatrixMatcher MatrixEquals(const Matrix& b)
{
    return MatrixMatcher(b);
}

#endif //UTIL_H
