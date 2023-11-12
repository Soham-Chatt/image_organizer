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
  return ""; // Return empty string for non-matching files
}


void parser::sortPictures(const std::string& directory, SortLevel level) {
  int ignoredFiles = 0;

  // Vector to hold filenames
  std::vector<std::string> filenames;

  // Collect all filenames
  for (const auto& entry : fs::directory_iterator(directory)) {
    filenames.push_back(entry.path().filename().string());
  }

  // Sort filenames
  std::sort(filenames.begin(), filenames.end());

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
      if (!silent) std::cerr << "Moved " << filename << " to " << newPath << std::endl;
    } else {
      ignoredFiles++;
    }
  }

  if (ignoredFiles > 0) {
    std::cout << std::endl;
    std::cerr << "WARNING: Could not parse the date for all files. Number of ignored files: " << ignoredFiles << std::endl;
  }

  std::cout << std::endl;
}
