#!/usr/bin/env python3

import os
import shutil
import re

# Define the source directory
source_dir = "../../../../pipes/tests"

# Make sure the source directory exists
if not os.path.isdir(source_dir):
    raise FileNotFoundError(f"Directory not found: {source_dir}")

# List all .cpp files in the source directory
cpp_files = [f for f in os.listdir(source_dir) if f.endswith(".cpp")]

# Open list.txt for writing
with open("list.txt", "w") as list_file:
    for cpp_file in cpp_files:
        # Log the filename
        list_file.write(f"{cpp_file}\n")

for cpp_file in cpp_files:
    src_path = os.path.join(source_dir, cpp_file)
    dst_path = cpp_file  # Copy into current folder with same name

    # Read original content
    with open(src_path, "r") as src:
        content = src.read()

    # Replace pipes:: with csCore::pipes::
    content = content.replace("pipes::", "csCore::pipes::")

    # Replace #include "pipes/..." with #include <csCore/pipes/...>
    content = re.sub(r'#include\s+"pipes/(.*?)"', r'#include <csCore/pipes/\1>', content)

    # Write modified content to new file
    with open(dst_path, "w") as dst:
        dst.write(content)

    print(f"Copied and transformed: {cpp_file}")

print("All files copied, transformed, and listed in list.txt.")