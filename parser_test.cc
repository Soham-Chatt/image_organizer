#include "parser.h"
#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

class ParserTest : public ::testing::TestWithParam<std::pair<std::string, std::string>> {
protected:
  std::vector<std::string> readFileLines(const std::string& filename) {
    std::vector<std::string> lines;
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
      lines.push_back(line);
    }
    return lines;
  }
};

TEST_P(ParserTest, ExtractYearFromFilenames) {
  auto files = GetParam();
  auto testFilenames = readFileLines(files.first);
  auto expectedYears = readFileLines(files.second);

  ASSERT_EQ(testFilenames.size(), expectedYears.size()) << "Test and reference files have different number of lines.";

  for (size_t i = 0; i < testFilenames.size(); ++i) {
    auto extractedYear = parser::extractDate(testFilenames[i], SortLevel::Year);

    // Check if the extracted year matches the expected year
    if (!expectedYears[i].empty()) {
      // If the expected year is not empty, we expect a match
      EXPECT_EQ(extractedYear, expectedYears[i]) << "Mismatch at line " << i + 1;
    } else {
      // If the expected year is empty, we expect the extracted year to be empty
      EXPECT_TRUE(extractedYear.empty()) << "Expected no date at line " << i + 1 << " but got " << extractedYear;
    }
  }
}

class SortPicturesTest : public ::testing::Test {
protected:
  std::string testDirectory = "tests/test3"; // Path to your test directory
  std::string backupDirectory = "tests/test3_backup"; // Path to backup directory

  void SetUp() override {
    // Backup the test directory
    if (fs::exists(backupDirectory)) {
      fs::remove_all(backupDirectory); // Clear existing backup
    }
    fs::copy(testDirectory, backupDirectory, fs::copy_options::recursive);
  }

  void TearDown() override {
    // Restore the test directory from backup
    if (fs::exists(testDirectory)) {
      fs::remove_all(testDirectory); // Clear test directory
    }
    fs::copy(backupDirectory, testDirectory, fs::copy_options::recursive);
    fs::remove_all(backupDirectory); // Delete backup
  }
};

TEST_F(SortPicturesTest, SortByYearWithReferenceOutput) {
  std::stringstream buffer;
  std::streambuf* prevcerrbuf = std::cerr.rdbuf(buffer.rdbuf());

  parser::sortPictures(testDirectory, SortLevel::Year);

  std::cerr.rdbuf(prevcerrbuf);

  std::ifstream refFile("tests/reference3year.txt");
  std::stringstream refContent;
  refContent << refFile.rdbuf();

  EXPECT_EQ(buffer.str(), refContent.str()) << "The output of sortPictures (Year) does not match the reference.";
}

TEST_F(SortPicturesTest, SortByMonthWithReferenceOutput) {
  std::stringstream buffer;
  std::streambuf* prevcerrbuf = std::cerr.rdbuf(buffer.rdbuf());

  parser::sortPictures(testDirectory, SortLevel::Month);

  std::cerr.rdbuf(prevcerrbuf);

  std::ifstream refFile("tests/reference3month.txt");
  std::stringstream refContent;
  refContent << refFile.rdbuf();

  EXPECT_EQ(buffer.str(), refContent.str()) << "The output of sortPictures (Month) does not match the reference.";
}


INSTANTIATE_TEST_SUITE_P(
    MultipleFileTests,
    ParserTest,
    ::testing::Values(
    std::make_pair("tests/test1.txt", "tests/reference1.txt"),
    std::make_pair("tests/test2.txt", "tests/reference2.txt")
),
[](const testing::TestParamInfo<ParserTest::ParamType>& info) {
  // Generate a name based on the index of the test case
  return "TestPair" + std::to_string(info.index);
}
);


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
