# Fractal Generation and Visualization

This directory contains C++ files for generating and visualizing various fractal patterns.

## Files

- `fractal.cpp`: Source code containing implementations for generating different types of fractals.
- `fractal.h`: Header file with class and method declarations for fractal generation.
- `fractaldrawer.cpp`: Source code for reading input from a CSV file and printing generated fractals.

## Usage

1. Compile the fractal generation and visualization program using a C++ compiler:
   ```sh
   g++ fractal.cpp fractaldrawer.cpp -o FractalDrawer -lboost_system -lboost_filesystem
   ```
   Note: Make sure you have the Boost C++ library installed to compile successfully.
2. Create a CSV file with fractal type and dimensions data, where each line follows the format: fractal_type,dimensions.
3. Run the compiled executable along with the CSV file path to generate and visualize fractal patterns:
   ```sh
   ./FractalDrawer path/to/your/fractal_data.csv
   ```
4. The program will parse the CSV file, generate the specified fractals, and display the patterns in reverse order.

## Notes

- Ensure you have the Boost C++ library installed, as it's used for filesystem operations.
- The provided compilation and execution instructions are for illustration purposes and may need adjustments based on your development environment.
- The fractal patterns generated and visualized in this program include Sierpinski Carpet, Sierpinski Triangle, and Vicsek Fractal. You can extend the program to include more fractal types if desired.
