import re
from enum import Enum


class Commands(Enum):

    """
    A Enum class for the representation of the commands available in the
    hack assembly.
    """

    A, C, L = range(3)


L_COMMAND_SIGN = "("
# A magic number.

A_COMMAND_SIGN = "@"
# A magic number.


class Parser:

    """
    A class that parse a assembly file one line at a time.
    """

    curr_line = ""
    curr_command = None
    next_line = ""
    file = None

    def __init__(self, file):

        """
        A constructor that receives a file.
        :param file: The file from which we need to read from.
        """

        self.file = file

    def has_more_commands(self):

        """
        A method that checks if there are more commands to run, if there are returns True,
        otherwise the method returns False.
        :return: True if there are more commands to run, False otherwise.
        """

        self.next_line = self.file.readline()
        if self.next_line in ['\n', '\r\n']:
            self.advance()
            self.has_more_commands()
        return self.next_line

    def advance(self):

        """
        A method that moves the parser to the next command if available.
        """

        self.curr_line = self.next_line.strip()
        self.curr_line = self.curr_line.replace(" ","")
    def command_type(self):

        """
        A method that determines which type of command we want to parse.
        """

        if self.curr_line.startswith(A_COMMAND_SIGN):
            self.curr_command = Commands.A
        elif self.curr_line.startswith(L_COMMAND_SIGN):
            self.curr_command = Commands.L
        elif re.match("\\w", self.curr_line):
            self.curr_command = Commands.C

    def find(self, x):

        """
        A method that receives a line and strips it.
        :param x: the index of the first character after the operator =.
        :return: The given command line without white spaces.
        """

        note = self.curr_line.find("/")
        if note >= 0:
            return self.curr_line[x.start() + 1:self.curr_line.find("/")].replace(
                " ", "")
        else:
            return self.curr_line[x.start() + 1:len(self.curr_line)].replace(" ", "")

    def comp(self):

        """
        A method that returns the comp part of C-instruction.
        :return: The comp part of C-instruction.
        """
        self.curr_line=self.curr_line.replace(" ","")
        x = re.search("=", self.curr_line)
        y = re.search(";", self.curr_line)
        if not y:
            return self.find(x)
        if not x:
            return self.curr_line[:y.start()].replace(" ", "")
        return self.curr_line[x.start() + 1:y.start()].replace(" ", "")

    def dest(self):

        """
        A method that returns the dest part of C-instruction,
        if there is no dest section returns null.
        :return: The dest part of C-instruction, otherwise null.
        """

        x = re.search("=", self.curr_line)
        if x:
            return self.curr_line[:x.start()].replace(" ", "")
        return "null"

    def jump(self):

        """
        A method that returns the jump part of C-instruction,
        if there is no jump section returns null.
        :return: The jump part of C-instruction, otherwise null.
        """

        x = re.search(";", self.curr_line)
        if x:
            # There is a jump section.
            return self.find(x)
        return "null"

    def symbol(self):

        """
        A method that returns the symbol or decimal Xxx of the current command @Xxx.
        :return: The symbol or decimal Xxx of the current command @Xxx.
        """

        x = self.curr_line.replace(" ", "")
        return x[1:]

    def L_symbol(self):

        """
        A method that returns the L-symbol of the current command (Xxx).
        :return: The L-symbol of the current command (Xxx).
        """

        return self.curr_line[self.curr_line.find("(") + 1:self.curr_line.find(")")]
