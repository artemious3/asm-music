def shift_left_by_4(input_file, output_file):
    with open(input_file, 'rb') as infile, open(output_file, 'wb') as outfile:
        while byte := infile.read(1):  # Read one byte at a time
            # Convert byte to an integer, shift left by 4, and mask to get the lower 8 bits
            shifted_byte = (int.from_bytes(byte, 'big') << 2) & 0xFF
            # Write the new byte to the output file
            outfile.write(shifted_byte.to_bytes(1, 'big'))

if __name__ == "__main__":
    input_filename = 'asm/samples/kick.dat'  # Replace with your input file name
    output_filename = 'asm/samples/kick2.dat'  # Replace with your desired output file name
    shift_left_by_4(input_filename, output_filename)
