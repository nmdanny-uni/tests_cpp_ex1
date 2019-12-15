//
// Created by Daniel Kerbel on 12/14/2019.
//

#include "catch.hpp"
#include "util.h"

TEST_CASE("Basic matrix operations", "[matrix]")
{
    SECTION("Can create a 1x1 matrix")
    {
        Matrix m;
        CHECK(m.getRows() == 1);
        CHECK(m.getCols() == 1);
        CHECK(m[0] == 0);
        CHECK(m(0, 0) == 0);
        Matrix mVec = m.vectorize();
        CHECK_THAT(m, MatrixEquals(mVec));
    }

    SECTION("Can create, copy and manipulate a 2x3 matrix")
    {
        Matrix m = Matrix(2, 3);
        m[0] = 1;
        m[1] = 2;
        m[2] = 3;
        m[3] = 10;
        m[4] = 20;
        m[5] = 30;


        // testing copying
        Matrix copy_1(m);
        CHECK_THAT(copy_1, MatrixEquals(m));
        Matrix copy_2 = m;
        CHECK_THAT(copy_2, MatrixEquals(m));

        // ensures the matrix we've built looks like the expected matrix
        Matrix copy3 = mkMatrix({
                                    {1,2,3},
                                    {10, 20, 30}
                                });
        CHECK_THAT(copy3, MatrixEquals(m));

        // ensure modifying copies 1,2 doesn't modify the original matrix
        copy_1[0] = 1337;
        copy_2[0] = 1338;
        CHECK_THAT(copy3, MatrixEquals(m));

        // self-assigning the matrix works and doesn't modify it
        copy3 = copy3;
        CHECK_THAT(copy3, MatrixEquals(m));

        // ensuring that modifying the copies didn't affect each other
        CHECK(1337 == copy_1(0,0));
        CHECK(1338 == copy_2(0,0));

    }

    SECTION("Newly initialized 2x2 matrix is zeroed")
    {
        Matrix m(2, 2);
        CHECK_THAT(m, MatrixEquals(mkMatrix({{0, 0}, {0, 0}})));
    }

    SECTION("Can add 2 matrices")
    {
        const Matrix a = mkMatrix({
                                {1, 2},
                                {3, 4}
        });
        const Matrix b = mkMatrix({
                                {2, 4},
                                {5, -2}
        });

        const Matrix expected = mkMatrix({
                                       {3, 6},
                                       {8, 2}
        });

        const Matrix sum = a + b;
        REQUIRE_THAT(expected, MatrixEquals(sum));
    }

    SECTION("Can add a matrix to another by accumulating")
    {
        // original matrices, without applying any operators
        const Matrix origA = mkMatrix({{1,2}, {3,4}});
        const Matrix origB = mkMatrix({{9, 8}, {7, 6}});

        Matrix a(origA);
        const Matrix b(origB);
        a += b;
        const Matrix expected = mkMatrix({{10, 10}, {10, 10}});

        CHECK_THAT(expected, MatrixEquals(a));

        // ensure that 'b' wasn't modified
        CHECK_THAT(origB, MatrixEquals(b));
    }

    SECTION("Can multiply a matrix by a scalar")
    {
        const Matrix a = mkMatrix({{1,2}, {3,4}});
        const Matrix copy(a);
        float scalar = -2;
        const Matrix expected = mkMatrix({
                                       {-2, -4},
                                       {-6, -8}
        });

        // doesn't matter which side we're multiplying
        CHECK_THAT(expected, MatrixEquals(a * scalar));
        CHECK_THAT(expected, MatrixEquals(scalar * a));

        // ensure that the original matrix wasn't modified
        CHECK_THAT(copy, MatrixEquals(a));
    }

    SECTION("Can multiply square matrix by square matrix")
    {
        const Matrix a = mkMatrix({{1,2}, {3,4}});
        const Matrix b = mkMatrix({{5,6}, {7,8}});
        const Matrix expectedAb = mkMatrix({
                                             {19,22}, {43,50}
        });

        CHECK_THAT(expectedAb, MatrixEquals(a * b));

        // ensure original matrices weren't modified
        CHECK_THAT(a, MatrixEquals(mkMatrix({{1,2}, {3,4}})));
        CHECK_THAT(b, MatrixEquals(mkMatrix({{5,6}, {7,8}})));
    }

    SECTION("Can multiply rectangle matrix by rectangle matrix")
    {
        const Matrix a = mkMatrix({{1,2}, {3,4}, {5,6}});
        const Matrix b = mkMatrix({{1,2,3}, {4,5,6}});
        const Matrix expectedAb = mkMatrix({
                                               {9, 12, 15},
                                               {19, 26, 33},
                                               {29, 40, 51}
        });

        CHECK_THAT(expectedAb, MatrixEquals(a * b));

        // ensure original matrices weren't modified
        CHECK_THAT(a, MatrixEquals(mkMatrix({{1,2}, {3,4}, {5,6}})));
        CHECK_THAT(b, MatrixEquals(mkMatrix({{1,2,3}, {4,5,6}})));
    }

    SECTION("Can vectorize a matrix")
    {
        const Matrix a = mkMatrix({{1,2}, {3,4}, {5,6}});
        const Matrix expectedVec = mkMatrix({{1}, {2}, {3}, {4}, {5}, {6}});

        CHECK_THAT(expectedVec, MatrixEquals(a.vectorize()));

        // ensure original matrix wasn't modified
        CHECK_THAT(a, MatrixEquals(mkMatrix({{1,2}, {3,4}, {5,6}})));
    }
}

