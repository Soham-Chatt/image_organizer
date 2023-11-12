#include "parser.h"

namespace fs = std::filesystem;

bool parser::silent = false;

const std::vector<std::regex> parser::regexes = {
    std::regex(R"(IMG-(\d{4})(\d{2})(\d{2})-)"), // IMG-YYYYMMDD-*
    std::regex(R"((\d{4})(\d{2})(\d{2}).*)")     // YYYYMMDD*
};


std::string parser::extractDate(const std::string& filename, SortLevel level) {
  std::smatch matches;
  for (const auto& dateRegex : regexes) {
    if (std::regex_search(filename, matches, dateRegex)) {
      switch (level) {
        case SortLevel::Year:
          return matches[1]; // Year
        case SortLevel::Month:
          return matches[1].str() + "-" + matches[2].str(); // Year-Month
      }
    }
  }
  throw std::invalid_argument("No date found in filename");
}


void parser::sortPictures(const std::string& directory, SortLevel level) {
  for (const auto& entry : fs::directory_iterator(directory)) {
    std::string filename = entry.path().filename().string();
    std::string datePart = extractDate(filename, level);

    if (!datePart.empty()) {
      std::string yearDir = directory + "/" + datePart.substr(0, 4); // Year directory
      std::string targetDir = yearDir;

      // Create the year directory if it does not exist
      if (!fs::exists(yearDir)) {
        fs::create_directory(yearDir);
      }

      // For month level, create an additional month subdirectory
      if (level == SortLevel::Month && datePart.length() > 4) {
        std::string month = datePart.substr(5, 2);
        std::string monthDir = yearDir; monthDir += "/" + month;
        targetDir = monthDir;

        // Create the month directory if it does not exist
        if (!fs::exists(monthDir)) {
          fs::create_directory(monthDir);
        }
      }

      std::string newPath = targetDir; newPath += "/" + filename;

      // Rename/move the file
      fs::rename(entry.path(), newPath);
      if (!silent) std::cout << "Moved " << filename << " to " << newPath << std::endl;
    }
  }
  std::cout << std::endl;
}
