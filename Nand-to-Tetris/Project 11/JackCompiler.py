import os
import sys

from CompilationEngine import *
from VMWriter import *

EXCEPTION_MESSAGE = "File not found , try another address "


class JackCompiler:
    """
    A class that acts as main and translates the given jack files to VM files.
    """

    def __init__(self, path):

        """
        The c'tor for the compilation engine object.
        :param in_file_name: The given file path to translate.
        """

        self.check_path(path)
        self.parser = None
        self.compiler = None
        self.writer = None

    def main(self):

        """
        A method that runs the program.
        """

        path = sys.argv[1]
        try:
            if os.path.isdir(path):
                for file in os.listdir(path):
                    if file.endswith(".jack"):
                        self.file_translating_routine(path + "/" + file,
                                                      path + "/" + file.replace('.jack','.vm'))
            elif os.path.isfile(path) and path.endswith(".jack"):
                self.file_translating_routine(path,
                                              path.replace(".jack", ".vm"))
        except FileNotFoundError:
            print(EXCEPTION_MESSAGE)

    def file_translating_routine(self, path, to_write):

        """
        A method for translating the files.
        :param path: The given file path to translate.
        """

        self.parser = JackTokenizer(path)
        self.writer = VMWriter(to_write)
        self.compiler = CompilationEngine(self.parser, self.writer)
        self.parser.run_tokenizer(path)
        self.compiler.compile_class()
        self.parser.close()

    @staticmethod
    def check_path(file1):

        """
        A method that checks that the path is valid else throws exception.
        :param file1: The given file path to translate.
        """

        try:
            os.path.exists(file1)
        except FileNotFoundError:
            print(EXCEPTION_MESSAGE)


if __name__ == '__main__':
    main = JackCompiler(sys.argv[1])
    main.main()
