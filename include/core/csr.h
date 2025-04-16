// authors: @Anubhav652

#include <vector>

#if __has_include(<omp.h>)
#include <omp.h>
#endif

namespace core {

/*
    This is a space-efficient approach to storing a sparse graph.
    Primarily used for page rank.
*/
class CSRMatrix {
public:
    std::vector<int> row_ptr_;
    std::vector<int> col_idx_;
    std::vector<float> values_;
    int N_;

    CSRMatrix(int nodes) : N_(nodes) { row_ptr_.resize(N_ + 1, 0); }

    // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
    void AddEdge(int from, int to, float weight) {
        col_idx_.push_back(to);
        values_.push_back(weight);
        row_ptr_[from + 1]++;
    }

    void Finalize() {
        for (int i = 1; i <= N_; ++i) {
            row_ptr_[i] += row_ptr_[i - 1];
        }
    }

    std::vector<float> Multiply(const std::vector<float>& vec) {
        std::vector<float> result(N_, 0.0);
#pragma omp parallel for
        for (int i = 0; i < N_; ++i) {
            for (int j = row_ptr_[i]; j < row_ptr_[i + 1]; ++j) {
                result[i] += values_[j] * vec[col_idx_[j]];
            }
        }
        return result;
    }
};

}  // namespace core