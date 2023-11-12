#include <iostream>
#include "parser.h"

namespace fs = std::filesystem;

void organizeDirectory() {
  std::string directory;
  int sortingType;
  bool validSortingInput = false;

  std::cout << std::endl << "Which directory would you like to organize?" << std::endl;
  std::cin >> directory;
  // Check if directory exists
  if (!std::filesystem::exists(directory)) {
    std::cerr << "Directory does not exist" << std::endl;
    return;
  } else if (parser::backup) {
    std::string backupDirectory = directory + "_backup";
    fs::create_directory(backupDirectory);
    // Copy files from the original directory to the backup directory
    for (const auto& entry : fs::directory_iterator(directory)) {
      try {
        fs::copy(entry.path(), backupDirectory + "/" + entry.path().filename().string());
      } catch (const fs::filesystem_error& e) {
        std::cerr << "Error copying file: " << e.what() << std::endl;
        return;
      }
    }
    std::cout << "Backup created at " << backupDirectory << std::endl;
  }

  std::cout << std::endl << "How would you like to organize it?\n1. Year\n2. Month" << std::endl;
  std::cin >> sortingType;

  SortLevel sortLevel = SortLevel::Year; // Default to year
  while (!validSortingInput) {
    if (sortingType == 1) {
      sortLevel = SortLevel::Year;
      validSortingInput = true;
    } else if (sortingType == 2) {
      sortLevel = SortLevel::Month;
      validSortingInput = true;
    } else {
      std::cerr << "Invalid input. Please enter '1' or '2'." << std::endl;
      return;
    }
  }

  std::cout << "Organizing by " << ((sortingType == 1) ? "year" : "month") << std::endl;
  parser::sortPictures(directory, sortLevel);
  parser::getDirectoryStructure(directory);
}

void testFileMatching() {
  std::string filename;
  std::cout << std::endl << "Enter a filename to test:" << std::endl;
  std::cin >> filename;
  std::cout << "Extracted date: " << parser::extractDate(filename, SortLevel::Month) << std::endl;
}

void menu() {
  bool exit = false;
  while (!exit) {
    std::cout << "Choose an option:" << std::endl;
    std::cout << "1. Organize a directory" << std::endl;
    std::cout << "2. Test file matching" << std::endl;
    std::cout << "3. Exit" << std::endl;

    int choice;
    std::cin >> choice;

    switch(choice) {
      case 1:
        organizeDirectory();
        break;
      case 2:
        testFileMatching();
        break;
      case 3:
        std::cout << "Exiting..." << std::endl;
        exit = true;
        break;
      default:
        std::cout << "Invalid choice" << std::endl;
    }
  }
}

void handleFlags(int argc, char*argv[]) {
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    // Check if the argument starts with a dash
    if (arg[0] == '-') {
      // Iterate over each character in the argument string
      for (char flag : arg.substr(1)) {
        switch (flag) {
          case 's':
            parser::silent = true;
            std::cout << "Silent mode enabled" << std::endl;
            break;
          case 'b':
            parser::backup = true;
            std::cout << "Backup enabled" << std::endl;
            break;
          default:
            throw std::invalid_argument("Invalid flag: " + std::string(1, flag));
        }
      }
    }
  }
}

int main(int argc, char*argv[]) {
  handleFlags(argc, argv);
  menu();
  return 0;
}
