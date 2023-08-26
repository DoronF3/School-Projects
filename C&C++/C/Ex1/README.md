## Student Information System

This program is a command-line tool for managing and sorting student information. It allows users to input student data including ID, name, grade, age, country, and city. The program provides options to display student information based on different sorting criteria.

### Usage

To use the Student Information System, compile and run the provided C code file using a C compiler. The program takes command-line arguments to determine the sorting method for displaying student information.

```bash
$ gcc student_info.c -o student_info
$ ./student_info [sorting_method]
```

Replace `[sorting_method]` with one of the following options:

best: Displays information of the best student based on a calculated value.
merge: Displays student information sorted using the merge sort algorithm.
quick: Displays student information sorted using the quick sort algorithm.

### Features
- Input student information including ID, name, grade, age, country, and city.
- Supports different sorting methods to display student information.
- Validates input data and handles errors for incorrect formats.
- Implements merge sort and quick sort algorithms for sorting students.

### Compilation and Execution
1. Compile the program using the provided C code file and a C compiler.
2. Run the compiled executable with appropriate command-line arguments.

### Sample Input Format
The program expects input data for each student in the following format:

[ID]    [Name]  [Grade] [Age]   [Country]   [City]

- `ID`: Student ID (10 characters, numeric).
- `Name`: Student name (up to 40 characters, alphabetic).
- `Grade`: Student grade (numeric between 0 and 100).
- `Age`: Student age (numeric between 18 and 120).
- `Country`: Student country (alphabetic).
- `City`: Student city (alphabetic).

### Sorting Methods
- `best`: Displays information of the student with the highest calculated value.
- `merge`: Displays student information sorted using merge sort.
- `quick`: Displays student information sorted using quick sort.

### Usage Examples
```bash
$ ./student_info best
$ ./student_info merge
$ ./student_info quick
```
