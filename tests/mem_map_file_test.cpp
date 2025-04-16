#include "core/mem_map_file.h"

#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include <string_view>
#include <span>

namespace fs = std::filesystem;

using core::MemMapFile;

constexpr std::string_view kTestDataString{"This will be in a tmp file!\n"};
constexpr std::span<const char> kTestData(kTestDataString.data(), kTestDataString.size());

template <typename T>
static constexpr bool SpanEqual(std::span<T> a, std::span<T> b) {
    if (a.size() != b.size()) return false;
    for (int i = 0; i < a.size(); i++)
        if (a[i] != b[i]) return false;
    return true;
}

class MemoryMappedFileTest : public ::testing::Test {
protected:
    fs::path temp_file_path_;

    void SetUp() override {
        temp_file_path_ = fs::temp_directory_path() / "mmap_test_file";
        std::ofstream ofs(temp_file_path_);
        ofs.write(kTestData.data(), kTestData.size());
        ofs.close();
    }

    void TearDown() override {
        if (fs::exists(temp_file_path_))
            fs::remove(temp_file_path_);
    }
};

TEST_F(MemoryMappedFileTest, BasicRead) {
    MemMapFile file(temp_file_path_.string());

    EXPECT_EQ(file.size(), kTestData.size());
    std::span<const char> file_data(file.data(), file.size());
    EXPECT_TRUE(SpanEqual(file_data, kTestData));
}

TEST_F(MemoryMappedFileTest, MissingFile) {
    fs::remove(temp_file_path_);
    EXPECT_THROW({
        MemMapFile file(temp_file_path_.string());;
    }, core::FileOpenFailure);
}
