#include "catch.hpp"
#include "Activation.h"
#include "util.h"

TEST_CASE("Activations work", "[activation]")
{
    SECTION("Relu activation works")
    {
        Matrix vec = mkMatrix({{1, 3, -5, 0, -4, 9}}).vectorize();
        Activation relu = Activation(ActivationType::Relu);
        CHECK(relu.getActivationType() == ActivationType::Relu);

        Matrix expected = mkMatrix({{1, 3, 0, 0, 0, 9}}).vectorize();

        CHECK_THAT(expected, MatrixEquals(relu(vec)));
    }

    SECTION("Softmax activation works")
    {
        Matrix vec = mkMatrix({{1,2,3}}).vectorize();
        Activation softmax = Activation(ActivationType::Softmax);
        CHECK(softmax.getActivationType() == ActivationType::Softmax);

        Matrix expected = mkMatrix({{0.0900306, 0.244728, 0.665241}}).vectorize();

        CHECK_THAT(expected, MatrixEquals(softmax(vec)));
    }
}
