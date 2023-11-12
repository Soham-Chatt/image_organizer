#!/bin/bash

# Directory to store the test/reference files
test_directory="tests"

# Create the directory if it does not exist
mkdir -p $test_directory

# Function to generate test/reference files for each pattern
generate_files() {
    # Test and reference files for each pattern
    test_file1="$test_directory/test1.txt"
    reference_file1="$test_directory/reference1.txt"
    test_file2="$test_directory/test2.txt"
    reference_file2="$test_directory/reference2.txt"

    : > $test_file1
    : > $reference_file1
    : > $test_file2
    : > $reference_file2

    # Generate files for the first pattern (IMG-YYYYMMDD-WAXXXX.jpg)
    for year in {2020..2023}; do
        for month in {01..12}; do
            for day in {01..28}; do
                img_name="IMG-$year$month$day-WA$(printf '%04d' $RANDOM).jpg"
                echo "$img_name" >> $test_file1
                echo "$year" >> $reference_file1
            done
        done
    done

    # Generate files for the second pattern (YYYYMMDD_XXXXXX.jpg)
    for year in {2020..2023}; do
        for month in {01..12}; do
            for day in {01..28}; do
                img_name="$year$month$day_$(printf '%06d' $RANDOM).jpg"
                echo "$img_name" >> $test_file2
                echo "$year" >> $reference_file2
            done
        done
    done
}

# Call the function
generate_files

echo "Test and reference files generated in $test_directory."
