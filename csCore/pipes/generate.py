#!/usr/bin/env python3

import os

# Define the source directories
source_dir = "../../../pipes/include/pipes"

# Output base directory (current working directory)
output_base_dir = "."

# Check the source directory exists
if not os.path.isdir(source_dir):
    raise FileNotFoundError(f"Directory not found: {source_dir}")

# List to store all generated file paths
generated_files = []

# Walk recursively through the source directory
for root, _, files in os.walk(source_dir):
    for file in files:
        if file.endswith(".hpp"):
            # Relative path from source_dir
            rel_path = os.path.relpath(os.path.join(root, file), source_dir)

            # Determine output path
            output_path = os.path.join(output_base_dir, rel_path)

            # Create necessary directories
            os.makedirs(os.path.dirname(output_path), exist_ok=True)

            # Write the new header file
            with open(output_path, "w") as f:
                f.write("#pragma once\n\n")
                f.write(
                    f"#include <pipes/{rel_path.replace(os.sep, '/')}>\n"
                )  # Use forward slashes
                f.write("#include <csCore/pipes/PipesCore.hpp>\n")

            # Track generated file
            generated_files.append(rel_path.replace(os.sep, "/"))

            print(f"Generated: {output_path}")

# Write the list.txt file
with open("list.txt", "w") as list_file:
    for rel_file in generated_files:
        list_file.write(f"{rel_file}\n")

print("All files generated and listed in list.txt.")
