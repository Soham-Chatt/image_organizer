#ifndef IMAGE_ORGANIZER_PARSER_H
#define IMAGE_ORGANIZER_PARSER_H

#include <iostream>
#include <regex>
#include <string>
#include <filesystem>
#include <vector>

class parser {
private:
  static const std::vector<std::regex> regexes;

public:
  static std::string extractYear(const std::string& filename);

  static void sortPictures(const std::string& directory);
};


#endif //IMAGE_ORGANIZER_PARSER_H
