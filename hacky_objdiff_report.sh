#!/bin/bash

# This script was written by DeepSeek and it makes it so
# objdiff-cli can generate a report by creating dummy .o files
# for everything not yet built by make.
# Yes, it takes a while.

# Function to create a minimal valid .o file using mwcceppc.exe via wibo
create_empty_o_file() {
    local file_path=$1
    local dir_path=$(dirname "$file_path")
    local c_file="$dir_path/empty.c"

    # Create the directory if it doesn't exist
    mkdir -p "$dir_path"

    # Create a minimal C file
    echo -e "// empty.c\nvoid dummy(void) {}" > "$c_file"

    # Compile the file into a .o file using mwcceppc.exe via wibo
    wibo ./mwcc_compiler/GC/1.2.5n/mwcceppc.exe -c "$c_file" -o "$file_path"

    # Clean up the C file
    rm "$c_file"
}

# Function to run the command and handle missing files
run_objdiff() {
    while true; do
        # Run the command and capture the output
        output=$(objdiff-cli-linux-x86_64 report generate 2>&1)

        # Check if the command succeeded
        if [ $? -eq 0 ]; then
            echo "Command succeeded!"
            break
        fi

        # Extract the missing file path from the error message
        missing_file=$(echo "$output" | grep -oP 'Failed to open \K[^ ]+')

        # Check if a missing file was found
        if [ -n "$missing_file" ]; then
            # Create a minimal valid .o file using mwcceppc.exe via wibo
            echo "Creating minimal valid .o file: $missing_file"
            create_empty_o_file "$missing_file"
        else
            # If no missing file was found, print the output and continue
            echo "No missing file path found in the output. Continuing..."
            echo "$output"
        fi
    done
}

# Run the function
run_objdiff

objdiff-cli-linux-x86_64 report generate --format json-pretty | less
