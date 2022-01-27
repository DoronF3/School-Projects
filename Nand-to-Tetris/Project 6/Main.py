import sys
from Parser import Parser
from Parser import Commands
from Code import Code
from SymbolTable import SymbolTable
import os

EXCEPTION_MESSAGE = "File not found , try another address "


class Main:
    """
    A class that runs the program and acts as the assembler.
    """

    curr_rom_add = 0
    # Initialize the rom to 0.

    curr_ram_add = 16

    # Initialize the ram to 16.

    def main(self):

        """
        A method that runs the entire program.
        """
        code = Code()   # Create a new code object.
        path = sys.argv[1]
        try:
            if os.path.isdir(path):
                for file in os.listdir(path):
                    if file.endswith(".asm"):
                        self.file_translating_routine(code, path+"\\"+file)
            elif os.path.isfile(path) and path.endswith(".asm"):
                file = path
                self.file_translating_routine(code, file)
        except FileNotFoundError:
            print(EXCEPTION_MESSAGE)


    """
    Method that translate the file from assembly to machine language 
    """
    def file_translating_routine(self, code, file):
        symbol_table = SymbolTable()
        # Create the default symbol table.
        self.first_pass(symbol_table, file)
        # Pass through the commands for the first time.
        self.second_pass(code, symbol_table, file)
        # Pass through the commands for the second time.

    def first_pass(self, symbol_table, file_path):

        """
        A method that goes through the commands and adds all the symbols to the
        symbol table.
        :param symbol_table: The default symbol table.
        """

        with open(file_path) as f:
            # Open the commands file.
            parser = Parser(f)
            # Parse the commands file.
            while parser.has_more_commands():
                parser.advance()
                parser.command_type()
                if (parser.curr_command is Commands.A) or (parser.curr_command
                                                           is Commands.C):
                    self.curr_rom_add += 1
                elif parser.curr_command is Commands.L:
                    symbol_table.add_entry(parser.L_symbol(),
                                           self.curr_rom_add)
            f.close()
            # Close the commands file.

    def second_pass(self, code, symbol_table, file_path):

        """
        A method that goes over the code in the second time,
        in this time we will do the translation of commands to machine language.
        :param code: A code type object.
        :param symbol_table: he current symbol table.
        """

        with open(file_path.split(".")[0] + ".hack", "w+") as w:
            # Open the destination file.
            with open(file_path) as f:
                # Open the commands file.
                parser = Parser(f)
                # Parse the commands file.
                while parser.has_more_commands():
                    parser.advance()
                    # Get the next command.
                    parser.command_type()
                    # Get the current command type.
                    final = self.code_translation(code, parser, symbol_table)
                    # Translate to machine language.
                    if final:
                        w.write(final)
                        # Write the command to destination file.
                        w.write('\n')
            f.close()
        w.close()

    def code_translation(self, code, parser, symbol_table):

        """
        A method that translates assembly code to machine language.
        :param code: A code type object.
        :param parser: A parser type object.
        :param symbol_table: The current symbol table.
        :return: The machine language translation.
        """

        final = ""
        number = 0
        if parser.curr_command is Commands.A:
            # A instruction.
            string = parser.symbol()
            if string.isdigit():
                number = int(parser.symbol())
            else:
                if symbol_table.contains(string):
                    # Symbol is in the table.
                    number = symbol_table.get_address(string)
                    # Get the given symbol address.
                else:
                    # The symbol is not on the table,
                    symbol_table.add_entry(string, self.curr_ram_add)
                    # Add the symbol to the table.
                    number = self.curr_ram_add
                    # Assign the symbol an address.
                    self.curr_ram_add += 1
            final = bin(number)[2:].zfill(16)
        elif  parser.curr_line.strip().startswith('/'):
            final = ''
        elif parser.curr_command is Commands.C:

            # C instruction.
            dest = code.dest(parser.dest())
            # Get dest.
            comp = code.comp(parser.comp().strip())
            # Get comp.
            jump = code.jump(parser.jump().strip())
            # Get jump.
            final = comp + dest + jump
            # Translate to machine language.
        return final


if __name__ == '__main__':
    main = Main()
    # Create a new main object.
    main.main()
    # Run the program.
