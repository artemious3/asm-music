import os
import re

def parse_header_file(file_path):
    """Parse a single header file for extern function declarations."""
    fasm_declarations = []
    # Regex to match the extern function declarations
    pattern = re.compile(r'(?:const)?\s?\w+\*?\s+\*?(Pa_[A-Za-z0-9_]*)\s*\(.*')

    with open(file_path, 'r') as file:
        for line in file:
            match = pattern.search(line)
            if match:
                function_name = match.group(1)
                fasm_declarations.append(f'extrn {function_name}')

    return fasm_declarations

def main():
    header_path = input("Enter the header path: ")
    fasm_constants = parse_header_file(header_path)

    # Output the results
    for constant in fasm_constants:
        print(constant)

if __name__ == "__main__":
    main()
