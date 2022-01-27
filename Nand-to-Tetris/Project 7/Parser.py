from enum import Enum


class Commands(Enum):
    """
    An Enum class for the representation of the available commands.
    """

    ARITHMETIC, PUSH, POP, LABEL, GOTO, IF_GOTO, FUNCTION, RETURN, CALL = \
        range(9)


class Parser:
    """
    A class that parse a VM file one line at a time.
    """

    arg1_final = ""
    # Initialize arg1.
    arg2_final = None
    # Initialize arg2.
    curr_line = ""
    # Initialize curr_line.
    curr_command = None
    # Initialize curr_command.
    next_line = ""
    # Initialize next_line.
    file = None

    # Initialize file.

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
        if self.next_line in ['\n',
                              '\r\n'] or self.next_line.strip().startswith(
                '/'):
            self.advance()
            self.has_more_commands()
        elif self.next_line.find("/") != -1:
            self.next_line=self.next_line[:self.next_line.find("/")]
        return self.next_line

    def advance(self):

        """
        A method that moves the parser to the next command if available.
        """

        self.curr_line = self.next_line.strip()

    def command_type(self):

        """
        A method that returns a constant representing the current command type.
        :return: A constant representing the current command type.
        """
        tmp = self.curr_line.split(" ")
        line_arr = []
        for i in range(len(tmp)):
            if tmp[i] != '':
                if tmp[i].startswith('//'):
                    break
                line_arr.append(tmp[i].strip('\t'))
        # Split the line into an array.
        if len(line_arr) == 1:
            if line_arr[0] == 'return':
                self.curr_command = Commands.RETURN
            else:
                # An Arithmetic operation
                self.curr_command = Commands.ARITHMETIC
                self.arg1_final = line_arr[0]
        elif len(line_arr) == 2:
            if line_arr[0] == 'label':
                self.curr_command = Commands.LABEL
            elif line_arr[0] == 'goto':
                self.curr_command = Commands.GOTO
            elif line_arr[0] == 'if-goto':
                self.curr_command = Commands.IF_GOTO
            self.arg1_final = line_arr[1]
        elif len(line_arr) == 3:
            if line_arr[0] == 'push':
                self.curr_command = Commands.PUSH
            elif line_arr[0] == 'pop':
                self.curr_command = Commands.POP
            elif line_arr[0] == 'call':
                self.curr_command = Commands.CALL
            elif line_arr[0] == 'function':
                self.curr_command = Commands.FUNCTION
            self.arg1_final = line_arr[1]
            self.arg2_final = line_arr[2]

    def arg1(self):

        """
        A method that returns the first argument of the current command.
        In case of an arithmetic command return the command itself.
        :return: A string value.
        """

        return self.arg1_final

    def arg2(self):

        """
        A method that returns the second argument of the current command.
        Only in case of push, pop, function or call commands.
        :return: An int value.
        """

        return self.arg2_final
