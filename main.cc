#include <iostream>
#include "parser.h"

void menu() {
  std::string directory;
  std::string filename;
  int sortingType;

  bool exit = false;
  bool validSortingInput = false;

  while (!exit) {
    std::cout << "Choose an option:" << std::endl;
    std::cout << "1. Organize a directory" << std::endl;
    std::cout << "2. Test file matching" << std::endl;
    std::cout << "3. Exit" << std::endl;

    int choice;
    std::cin >> choice;
    // If the user enters a non-integer, std::cin will be in a failed state
    if (!std::cin) {
      std::cout << "Invalid choice" << std::endl;
      std::cin.clear();
      std::cin.ignore(10000, '\n');
      continue;
    }

    switch(choice) {
      case 1:
        std::cout << std::endl << "Which directory would you like to organize?" << std::endl;
        std::cin >> directory;
        // Check if dir exists
        if (!std::filesystem::exists(directory)) {
          std::cerr << "Directory does not exist" << std::endl;
          return;
        }

        std::cout << std::endl << "How would you like to organize it?\n1.Year\n2.Month" << std::endl;
        std::cin >> sortingType;

        SortLevel sortLevel;

        while (!validSortingInput) {
          if (sortingType == 1) {
            sortLevel = SortLevel::Year;
            validSortingInput = true;
            break;
          }
          else if (sortingType == 2) {
            sortLevel = SortLevel::Month;
            validSortingInput = true;
            break;
          }
          else {
            std::cerr << "Invalid input. Please enter '1' or '2'." << std::endl;
            break;
          }
        }


        std::cout << "Organizing by " << ((sortingType == 1) ? "year" : "month") << std::endl;
        parser::sortPictures(directory, sortLevel);
        parser::getDirectoryStructure(directory);
        break;
      case 2:
        std::cout << std::endl << "Enter a filename to test:" << std::endl;
        std::cin >> filename;
        std::cout << "Extracted date: " << parser::extractDate(filename, SortLevel::Month) << std::endl;
        break;
      case 3:
        std::cout << "Exiting..." << std::endl;
        exit = true;
        break;
      default: std::cout << "Invalid choice" << std::endl;
    }
  }
}

int main(int argc, char*argv[]) {
  // Silent flag (-s) means no output
  (argc > 1 && std::string(argv[1]) == "-s") ? parser::silent = true : parser::silent = false;
  menu();
  return 0;
}
