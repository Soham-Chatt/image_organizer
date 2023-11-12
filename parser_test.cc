#include "parser.h"
#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include <vector>

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
    try {
      auto extractedYear = parser::extractYear(testFilenames[i]);
      EXPECT_EQ(extractedYear, expectedYears[i]) << "Mismatch at line " << i + 1;
    } catch (const std::exception& e) {
      std::cerr << "Exception at line " << i + 1 << ": " << e.what() << std::endl;
      throw; // Rethrow the exception to let the test fail
    }
  }
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
