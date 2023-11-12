#include <iostream>
#include "parser.h"

void menu() {
  std::string directory;
  std::string filename;
  std::string userInput;

  bool exit = false;

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
        std::cout << "Which directory would you like to organize?" << std::endl;
        std::cin >> directory;
        // Check if dir exists
        if (!std::filesystem::exists(directory)) {
          std::cerr << "Directory does not exist" << std::endl;
          return;
        }

        std::cout << "How would you like to organize it? (Year or Month)" << std::endl;
        std::cin >> userInput;

        SortLevel sortLevel;

        if (userInput == "Year") sortLevel = SortLevel::Year;
        else if (userInput == "Month") sortLevel = SortLevel::Month;
        else {
          std::cerr << "Invalid input. Please enter 'Year' or 'Month'." << std::endl;
          return;
        }

        std::cout << "Organizing by " << userInput << std::endl;
        parser::sortPictures(directory, sortLevel);
        break;
      case 2:
        std::cout << "Enter a filename to test:" << std::endl;
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
