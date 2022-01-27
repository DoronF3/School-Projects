#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>
#include "HashMap.hpp"

#define FILE_ERR "Invalid input\n"
#define USAGE_ERR "Usage: SpamDetector <database path> <message path> <threshold>\n"

typedef boost::tokenizer<boost::char_separator<char>> tokenizer;

/**
 * A method that returns the number of digits of a given number.
 * @param points The given number to check.
 * @return The number of digits of the number if its not 0, otherwise -1.
 */
int numOfDig(int points)
{
    int counter = 0;
    if (points == 0)
    {
        return 1;
    }
    while (points != 0)
    {
        points /= 10;
        counter++;
    }
    return counter;

}

/**
 * A method that checks that the given value is a number.
 * @param t A string value representing a number.
 * @return The number if its valid, -1 otherwise.
 */
int checkNum(const std::string &t)
{
    try
    {
        int points = std::stoi(t);
        if (((int) t.length() != numOfDig(points)) || points < 0)
        {
            return -1;
        }
        return points;
    }
    catch (std::invalid_argument &e)
    {
        return -1;
    }
}

/**
 * A method that receives a vector of fractals parses the file and adds the relevant vectors,
 * or exits the program if the input file is invalid.
 * @param vector The given vector we wish to add fractals too.
 * @param in The given file to parse.
 */
bool parser(HashMap<std::string, int> &map, std::ifstream &in)
{
    std::string s;
    boost::char_separator<char> sep{","};
    while (std::getline(in, s))
    {
        if (s.empty() || s.find_first_of(',') != s.find_last_of(','))
        {
            return false;
        }
        std::string stri{};
        int count = 0, points = -1;
        tokenizer tok{s, sep};
        for (const auto &t: tok)
        {
            if (count == 0) // String.
            {
                stri = t;
            }
            else if (count == 1) // value.
            {
                points = checkNum(t);
            }
            else if (count == 2) // Invalid input.
            {
                return false;
            }
            count++;
        }
        if (points == -1) // A value wasn't initialized.
        {
            return false;
        }
        map.insert(stri, points);
    }
    return true;
}

/**
 * A method that checks if a given message is spam or not and prints a relevant message.
 * @param map The given map to check the values.
 * @param message The given message to check.
 * @param threshold The threshold we need to pass.
 */
void checkSpam(HashMap<std::string, int> &map, std::ifstream &message, int threshold)
{
    std::string s, curLine;
    int sum = 0;
    while (std::getline(message, curLine))
    {
        for (char c : curLine)
        {
            s += tolower(c); // Get the line in lower case.
        }
        s += "\n";
    }
    for (const auto &pair: map)
    {
        std::string key = pair.first;
        int val = pair.second;
        std::string::size_type index = 0;
        for (char &ch : key)
        {
            ch = tolower(ch); // Get the key in lower case.
        }
        while ((index = s.find(key, index)) != std::string::npos)
        {
            sum += val;
            index += key.length();
        }
    }
    if (sum >= threshold) // The message is spam.
    {
        std::cout << "SPAM" << std::endl;
    }
    else
    {
        std::cout << "NOT_SPAM" << std::endl;
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
    if (argc != 4) // Check that the right amount of arguments were given.
    {
        std::cerr << USAGE_ERR;
        return EXIT_FAILURE;
    }
    int threshold = checkNum(argv[3]);
    if (threshold == -1)
    {
        std::cerr << FILE_ERR;
        return EXIT_FAILURE;
    }
    if (!boost::filesystem::exists(argv[1]) || !boost::filesystem::exists(argv[2]) || (threshold <= 0) \
        || (int) std::strlen(argv[3]) != numOfDig(threshold))
        // Checks that the paths of both given files exist and the threshold is positive.
    {
        std::cerr << FILE_ERR;
        return EXIT_FAILURE;
    }
    std::ifstream in(argv[1]); // Create a stream to read the database file.
    HashMap<std::string, int> map;
    if (!parser(map, in)) // Check that the given input in the file was valid.
    {
        in.close();
        std::cerr << FILE_ERR;
        return EXIT_FAILURE;
    }
    std::ifstream message(argv[2]); // Create a stream to read the message file.
    checkSpam(map, message, threshold);
    in.close();
    message.close();
    return 0;
}