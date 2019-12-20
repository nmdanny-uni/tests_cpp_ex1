//
// Created by Daniel Kerbel on 12/14/2019.
//

#ifndef UTIL_H
#define UTIL_H

#include "catch.hpp"
#include "Matrix.h"
#include <sstream>

/** Helper function for creating a matrix using brace initialization syntax, 'rows' is basically a list of lists
 */
static Matrix mkMatrix(const std::initializer_list<std::initializer_list<float>>& rows)
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
        /** Used for printing a matrix */
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

/** Used for comparing matrices for equality */
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
            // like checking that a[ix] == _b[ix], but uses approximation since we're dealing with floats
            // see https://github.com/catchorg/Catch2/blob/master/docs/assertions.md for more information
            if (a[ix] != Approx(_b[ix]))
            {
                std::cerr << "While checking 2 matrices for equality, at index [" << ix << "], the values " << a[ix]
                          << " and " << _b[ix] << " aren't equal(approximately)" << std::endl;
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
