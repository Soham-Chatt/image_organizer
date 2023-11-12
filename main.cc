#include <iostream>
#include "parser.h"

void menu() {
  std::string directory;
  std::string filename;
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
        std::cout << "Organizing " << directory << "..." << std::endl;

        parser::sortPictures(directory);
        break;
      case 2:
        std::cout << "Enter a filename to test:" << std::endl;
        std::cin >> filename;
        std::cout << "Extracted year: " << parser::extractYear(filename) << std::endl;
        break;
      case 3:
        std::cout << "Exiting..." << std::endl;
        exit = true;
        break;
      default: std::cout << "Invalid choice" << std::endl;
    }
  }
}

int main() {
  menu();
  return 0;
}
