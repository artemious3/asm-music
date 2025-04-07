import os
import re

def parse_header_file(file_path):
    """Parse a single header file for numeric defines."""
    fasm_constants = []
    # Regex to match #define statements for numeric constants
    pattern = re.compile(r'#define\s+([A-Za-z_][A-Za-z0-9_]*)\s+([0-9a-fA-FxX]+)')

    with open(file_path, 'r') as file:
        for line in file:
            match = pattern.search(line)
            if match:
                constant_name = match.group(1)
                constant_value = match.group(2)
                fasm_constants.append(f'{constant_name}= {constant_value}')

    return fasm_constants

def parse_directory(directory_path):
    """Parse all header files in the given directory for numeric defines."""
    all_fasm_constants = []
    
    for filename in os.listdir(directory_path):
        if filename.endswith('.h'):  # Assuming header files have .h extension
            file_path = os.path.join(directory_path, filename)
            fasm_constants = parse_header_file(file_path)
            all_fasm_constants.extend(fasm_constants)

    return all_fasm_constants

def main():
    directory_path = input("Enter the directory path containing header files: ")
    fasm_constants = parse_directory(directory_path)

    # Output the results
    for constant in fasm_constants:
        print(constant)

if __name__ == "__main__":
    main()
