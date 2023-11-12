#!/bin/bash

# Number of files to generate
num_files=200

# Base directory to create files in
base_dir="./generated_files"

# Create base directory if it doesn't exist
mkdir -p "$base_dir"

# Function to generate a random number in a range
rand() {
    echo $(($RANDOM % $1 + $2))
}

for ((i=1; i<=num_files/2; i++))
do
    # Generate random date
    year=$(rand 21 2000) # Random year between 2000 and 2020
    month=$(printf "%02d" $(rand 12 1)) # Random month
    day=$(printf "%02d" $(rand 31 1)) # Random day

    # Generate random suffix for filename
    suffix=$(rand 10000 1)

    # Create file with pattern IMG-YYYYMMDD-XXXX.png
    touch "$base_dir/IMG-$year$month$day-$suffix.png"

    # Create file with pattern YYYYMMDD_XXXX.png
    touch "$base_dir/${year}${month}${day}_$suffix.png"
done

echo "Generated $num_files files for each pattern in $base_dir."
