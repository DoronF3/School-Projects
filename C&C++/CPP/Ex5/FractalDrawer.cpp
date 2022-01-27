#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>
#include "Fractal.h"

#define NUM_ARGS_MSG "Usage: FractalDrawer <file path>\n"
#define FILE_ERR "Invalid input\n"
#define EXTENSION ".csv"

typedef boost::tokenizer<boost::char_separator<char>> tokenizer;

/**
 * A method that checks that the given line is valid.
 * @param t A string value.
 * @param count The index of the given argument.
 * @return The number if its valid, 0 otherwise.
 */
int checkNum(const std::string &t, int count)
{
    if (std::isdigit(t[0]) && t.length() == 1)
    {
        int num = std::atoi(&(t[0]));
        if ((count == 0 && num >= 1 && num <= 3) || (count == 1 && num >= 1 && num <= 6))
        {
            return num;
        }
        return 0;
    }
    return 0;
}

/**
 * A method that creates a new fractal with the given dimensions.
 * @param num The type of the fractal we want to create.
 * @param dim The dimension of the fractal we wish to create.
 * @return A pointer to the new fractal.
 */
static Fractal *factory(int num, int dim)
{
    switch (num)
    {
        case 1:
            return new SierpinskiCarpet(dim);
        case 2:
            return new SierpinskiTriangle(dim);
        case 3:
            return new VicsekFractal(dim);
        default:
            std::cerr << FILE_ERR;
            exit(EXIT_FAILURE);
    }
}

/**
 * A method that receives a vector of fractals parses the file and adds the relevant vectors,
 * or exits the program if the input file is invalid.
 * @param vector The given vector we wish to add fractals too.
 * @param in The given file to parse.
 */
bool parser(std::vector<Fractal *> &vector, std::ifstream &in)
{
    std::string s;
    boost::char_separator<char> sep{","};
    while (std::getline(in, s))
    {
        int count = 0, frac = 0, dim = 0;
        tokenizer tok{s, sep};
        for (const auto &t: tok)
        {
            if (count == 0) // Fractal type.
            {
                frac = checkNum(t, count);
            }
            else if (count == 1) // Fractal dim.
            {
                dim = checkNum(t, count);
            }
            else if (count == 2) // Invalid input.
            {
                return false;
            }
            count++;
        }
        if (frac == 0 || dim == 0) // A value wasn't initialized.
        {
            return false;
        }
        vector.insert(vector.begin(), factory(frac, dim));
    }
    return true;
}

/**
 * A method that frees the memory we allocate.
 * @param vec The given vector.
 */
void deleteVec(const std::vector<Fractal *> &vec)
{
    for (auto &fractal: vec)
    {
        delete (fractal);
    }
}

/**
 * The main method that runs the program.
 * @param argc The amount of arguments given by the user.
 * @param argv The arguments given.
 * @return 0 if the program run well, exit failure otherwise.
 */
int main(int argc, char *argv[])
{
    if (argc != 2) // Check that the right amount of arguments were given.
    {
        std::cerr << NUM_ARGS_MSG;
        exit(EXIT_FAILURE);
    }
    if (!boost::filesystem::exists(argv[1]) || boost::filesystem::extension(argv[1]) != EXTENSION)
        // Check that the path exists and the file is of extension .csv.
    {
        std::cerr << FILE_ERR;
        exit(EXIT_FAILURE);
    }
    std::ifstream in(argv[1]); // Create a stream to read th file.
    std::vector<Fractal *> vec; // Create a vector of fractals.
    if (!parser(vec, in)) // Check that the given input in the file was valid.
    {
        in.close();
        deleteVec(vec);
        std::cerr << FILE_ERR;
        exit(EXIT_FAILURE);
    }
    for (auto &i: vec)
    {
        i->printFractal(); // Print fractals in reverse order.
    }
    in.close();
    deleteVec(vec);
    return 0;
}