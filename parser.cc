#include "parser.h"

namespace fs = std::filesystem;

const std::vector<std::regex> parser::regexes = {
    std::regex(R"(IMG-(\d{4})(\d{2})(\d{2})-)"), // IMG-YYYYMMDD-*
    std::regex(R"((\d{4})(\d{2})(\d{2}).*)")     // YYYYMMDD*
};


std::string parser::extractYear(const std::string& filename) {
  std::smatch matches;
  for (const auto& dateRegex : regexes) {
    if (std::regex_search(filename, matches, dateRegex)) {
      return matches[1];
    }
  }
  throw std::invalid_argument("No date found in filename");
}

void parser::sortPictures(const std::string& directory) {
  for (const auto& entry : fs::directory_iterator(directory)) {
    std::string filename = entry.path().filename().string();
    std::string year = extractYear(filename);

    if (!year.empty()) {
      std::string newDir = directory; newDir += "/" + year;
      if (!fs::exists(newDir)) {
        fs::create_directory(newDir);
      }
      newDir += "/" + filename;
      fs::rename(entry.path(), newDir);
      std::cout << "Moved " << filename << " to " << newDir << std::endl;
    }
  }
}
