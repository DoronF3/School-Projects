import os
import sys

from CodeWriter import CodeWriter
from Parser import *

EXCEPTION_MESSAGE = "File not found , try another address "


class Main:
    """
    A class that runs the program and acts as the assembler.
    """

    cd = None

    def __init__(self):
        self.cd = CodeWriter()  # Create a new CodeWriter object.

    def main(self):
        """
        A method that runs the entire program.
        """
        path = sys.argv[1]
        try:
            if os.path.isdir(path):
                for file in os.listdir(path):
                    if file.endswith(".vm"):
                        self.file_translating_routine(path + "/" + file)
            elif os.path.isfile(path) and path.endswith(".vm"):
                self.cd.file_name_setter(os.path.split(path)[1])
                self.file_translating_routine(os.path.join(path))
        except FileNotFoundError:
            print(EXCEPTION_MESSAGE)

    def file_translating_routine(self, file_path):
        tail, head = os.path.split(file_path)
        self.cd.file_name_setter(head)
        # Create the default symbol table.
        self.first_pass(file_path)
        # Pass through the commands for the first time.

    def first_pass(self, file_path):

        """
        A method that goes through the commands and adds all the symbols to the
        symbol table.
        :param file_path:
        :param parser:
        """

        with open(file_path.split(".")[0] + ".asm", "w+") as w:
            with open(file_path) as f:
                parser = Parser(f)
                # Open the commands file.
                while parser.has_more_commands():
                    parser.advance()
                    parser.command_type()
                    if isinstance(parser.curr_command, Commands):
                        w.write("\n// {}\n".format(parser.curr_line))
                        if parser.curr_command == Commands.ARITHMETIC:
                            w.write(self.cd.write_arithmetic(parser.arg1()))
                        elif parser.curr_command == Commands.PUSH:
                            w.write(self.cd.write_push_pop(Commands.PUSH,
                                                           parser.arg1(),
                                                           parser.arg2()))
                        elif parser.curr_command == Commands.POP:
                            w.write(self.cd.write_push_pop(Commands.POP,
                                                           parser.arg1(),
                                                           parser.arg2()))
                f.close()
                # Close the vm file.
            w.close()


if __name__ == '__main__':
    main = Main()
    # Create a new main object.
    main.main()
    # Run the program.
