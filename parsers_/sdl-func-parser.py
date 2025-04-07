import os
import re

def parse_header_file(file_path):
    """Parse a single header file for extern function declarations."""
    fasm_declarations = []
    # Regex to match the extern function declarations
    pattern = re.compile(r'extern\s+DECLSPEC\s+\w+\s+\*?\s?SDLCALL\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*\(.*')

    with open(file_path, 'r') as file:
        for line in file:
            match = pattern.search(line)
            if match:
                function_name = match.group(1)
                fasm_declarations.append(f'extrn {function_name}')

    return fasm_declarations

def parse_directory(directory_path):
    """Parse all header files in the given directory."""
    all_fasm_declarations = []
    
    for filename in os.listdir(directory_path):
        if filename.endswith('.h'):  # Assuming header files have .h extension
            file_path = os.path.join(directory_path, filename)
            fasm_declarations = parse_header_file(file_path)
            all_fasm_declarations.extend(fasm_declarations)

    return all_fasm_declarations

def main():
    directory_path = input("Enter the directory path containing header files: ")
    fasm_declarations = parse_directory(directory_path)

    # Output the results
    for declaration in fasm_declarations:
        print(declaration)

if __name__ == "__main__":
    main()
