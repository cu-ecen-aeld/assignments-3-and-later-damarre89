#!/bin/sh

# Check two arguments provided
if [ $# -ne 2 ]; then
    echo "Error: Two arguments required. First: path to directory, Second: search string."
    exit 1
fi

filesdir=$1
searchstr=$2

# Check first argument is a directory
if [ ! -d "$filesdir" ]; then
    echo "Error: $filesdir is not a directory."
    exit 1
fi

# Count number of files in the directory and subdirectories
num_files=$(find "$filesdir" -type f | wc -l)

# Count the number of lines matching the search string
num_matching_lines=$(grep -r "$searchstr" "$filesdir" 2>/dev/null | wc -l)

# Output the results
echo "The number of files are $num_files and the number of matching lines are $num_matching_lines"
