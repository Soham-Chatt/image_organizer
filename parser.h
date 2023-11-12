#ifndef IMAGE_ORGANIZER_PARSER_H
#define IMAGE_ORGANIZER_PARSER_H

#include <iostream>
#include <regex>
#include <string>
#include <filesystem>
#include <vector>

enum class SortLevel {
  Year ,
  Month,
};

class parser {
private:
  static const std::vector<std::regex> regexes;

public:
  static bool silent;

  static std::string extractDate(const std::string &filename, SortLevel level);

  static void sortPictures(const std::string &directory, SortLevel level);
};


#endif //IMAGE_ORGANIZER_PARSER_H
