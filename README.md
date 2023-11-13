# Image Organizer

## Overview
Image Organizer is a C++ application designed to organize image files in a directory by date. It categorizes images based on the year and month extracted from their filenames.

## Features
- **Directory Sorting:** Organizes images into folders based on year or month.
- **Backup Creation:** Option to create a backup of the original directory before organizing.
- **Date Extraction:** Extracts dates from filenames using predefined patterns.
- **Silent Mode:** Suppresses output for automated workflows.

## Requirements
- C++17 or higher.
- The `std::filesystem` library.

## Usage
1. Compile the program with C++17 support.
2. Run the program and follow the prompts to:
   - Select a directory.
   - Choose sorting by year or month.
   - Enable backup or silent mode with `-b` and `-s` flags respectively.

## Testing
Unit tests are provided using Google Test framework. Run `make test` to execute tests. Make sure you have the gtest library installed.
