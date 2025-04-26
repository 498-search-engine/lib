// authors: @Anubhav652

#include <gtest/gtest.h>
#include <core/csr.h>

#include <vector>

using namespace core;

TEST(CSRMatrixTest, EmptyMatrixMultiply) {
    CSRMatrix matrix(3);
    matrix.Finalize();
    std::vector<float> vec(3, 1.0);
    std::vector<float> result = matrix.Multiply(vec);
    std::vector<float> expected(3, 0.0);
    EXPECT_EQ(result, expected);
}

TEST(CSRMatrixTest, SingleEdgeMultiply) {
    CSRMatrix matrix(2);
    matrix.AddEdge(0, 1, 1.0);
    matrix.Finalize();
    std::vector<float> vec = {2.0, 3.0};
    std::vector<float> result = matrix.Multiply(vec);
    std::vector<float> expected = {3.0, 0.0};
    EXPECT_EQ(result, expected);
}

TEST(CSRMatrixTest, MultipleEdgesFromOneNode) {
    CSRMatrix matrix(4);
    matrix.AddEdge(0, 1, 0.5);
    matrix.AddEdge(0, 2, 0.5);
    matrix.AddEdge(0, 3, 0.5);
    matrix.Finalize();
    std::vector<float> vec = {1.0, 1.0, 1.0, 1.0};
    std::vector<float> result = matrix.Multiply(vec);
    std::vector<float> expected = {1.5, 0.0, 0.0, 0.0};
    EXPECT_EQ(result, expected);
}

TEST(CSRMatrixTest, NodesWithNoOutgoingEdges) {
    CSRMatrix matrix(3);
    matrix.AddEdge(0, 1, 1.0);
    matrix.Finalize();
    std::vector<float> vec = {5.0, 6.0, 7.0};
    std::vector<float> result = matrix.Multiply(vec);
    std::vector<float> expected = {6.0, 0.0, 0.0};
    EXPECT_EQ(result, expected);
}

TEST(CSRMatrixTest, EdgesAddedOutOfOrder) {
    CSRMatrix matrix(3);
    matrix.AddEdge(2, 0, 1.0);
    matrix.AddEdge(1, 0, 1.0);
    matrix.AddEdge(0, 0, 1.0);
    matrix.Finalize();
    std::vector<float> vec = {1.0, 2.0, 3.0};
    std::vector<float> result = matrix.Multiply(vec);
    std::vector<float> expected = {1.0, 1.0, 1.0};
    EXPECT_EQ(result, expected);
}

TEST(CSRMatrixTest, DifferentWeightsAndColumns) {
    CSRMatrix matrix(3);
    matrix.AddEdge(0, 1, 2.0);
    matrix.AddEdge(0, 2, 3.0);
    matrix.Finalize();
    std::vector<float> vec = {4.0, 5.0, 6.0};
    std::vector<float> result = matrix.Multiply(vec);
    std::vector<float> expected = {(2.0*5.0) + (3.0*6.0), 0.0, 0.0};
    EXPECT_EQ(result, expected);
}

TEST(CSRMatrixTest, MultipleNodesContributing) {
    CSRMatrix matrix(3);
    matrix.AddEdge(0, 1, 2.0);
    matrix.AddEdge(1, 2, 3.0);
    matrix.AddEdge(2, 0, 4.0);
    matrix.Finalize();
    std::vector<float> vec = {1.0, 2.0, 3.0};
    std::vector<float> result = matrix.Multiply(vec);
    std::vector<float> expected = {4.0, 9.0, 4.0};
    EXPECT_EQ(result, expected);
}

TEST(CSRMatrixTest, FinalizePrefixSumCorrectness) {
    CSRMatrix matrix(4);
    matrix.AddEdge(0, 1, 1.0);
    matrix.AddEdge(0, 2, 1.0);
    matrix.AddEdge(1, 3, 1.0);
    matrix.Finalize();
    std::vector<float> vec = {0.0, 1.0, 2.0, 3.0};
    std::vector<float> result = matrix.Multiply(vec);
    std::vector<float> expected = {3.0, 3.0, 0.0, 0.0};
    EXPECT_EQ(result, expected);
}

TEST(CSRMatrixTest, ZeroVectorMultiplication) {
    CSRMatrix matrix(2);
    matrix.AddEdge(0, 1, 1.0);
    matrix.AddEdge(1, 0, 1.0);
    matrix.Finalize();
    std::vector<float> vec = {0.0, 0.0};
    std::vector<float> result = matrix.Multiply(vec);
    std::vector<float> expected = {0.0, 0.0};
    EXPECT_EQ(result, expected);
}