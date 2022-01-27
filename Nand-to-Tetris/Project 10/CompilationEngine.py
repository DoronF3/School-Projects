import os
import sys
from JackTokenizer import *
import Helper

EXCEPTION_MESSAGE = "File not found , try another address "


class CompilationEngine:
    """
    A class that acts as the compilation engine and adds all the needed tags to the
    given file.
    """

    def __init__(self, in_file_name):

        """
        The c'tor for the compilation engine object.
        :param in_file_name: The given file path to translate.
        """

        self.check_path(in_file_name)
        self.parser = None
        self.out_file = None

    def main(self):

        """
        A method that runs the program.
        """

        path = sys.argv[1]
        try:
            if os.path.isdir(path):
                for file in os.listdir(path):
                    if file.endswith(".jack"):
                        self.file_translating_routine(path + "/" + file)
            elif os.path.isfile(path) and path.endswith(".jack"):
                self.file_translating_routine(path)
        except FileNotFoundError:
            print(EXCEPTION_MESSAGE)

    def file_translating_routine(self, path):

        """
        A method for translating the files.
        :param path: The given file path to translate.
        """

        self.parser = JackTokenizer(path)
        self.parser.run_tokenizer(path)
        self.out_file = open(path.replace(".jack", ".xml"), "w+")
        self.compile_class()
        self.out_file.close()
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

    def compile_class(self):

        """
        A method that translates a single class.
        """

        self.parser.advance()
        cur_line = ""

        cur_line += "<class>\n"
        for i in range(3):
            cur_line += " ".join([Helper.tags(self.parser.token_type_string(),
                                              self.parser.get_token())])
            cur_line += "\n"
            self.parser.advance()
        i = 0
        while self.parser.get_token() != '}':
            cur_line += self.compile_class_var_dec()
            line = self.compile_subroutine()
            if line != "":
                cur_line += line
                self.parser.advance()
            i += 1
        cur_line += "\n".join([Helper.tags(self.parser.token_type_string(),
                                           self.parser.get_token()) + "\n"])
        cur_line += "</class>"
        self.out_file.write(cur_line)
        self.out_file.close()

    def compile_class_var_dec(self):

        """
        A method that translates a single class var dec.
        :return: The translated text.
        """

        line = ""
        arr = ["static", "field"]
        if self.parser.get_token() in arr:
            line += "<classVarDec>" + "\n"
            while self.parser.get_token() is not ";":
                line += Helper.tags(self.parser.token_type_string(),
                                    self.parser.get_token()) + "\n"
                self.parser.advance()
            line += Helper.tags(self.parser.token_type_string(),
                                self.parser.get_token()) + "\n"
            self.parser.advance()
            line += "</classVarDec>\n"
        return line

    def compile_subroutine(self):

        """
        A method that translates a single subroutine.
        :return: The translated text.
        """

        line = ""
        arr = ['constructor', 'function', 'method']
        if self.parser.get_token() in arr:
            line += "<subroutineDec>" + "\n"
            for i in range(4):
                line += Helper.tags(self.parser.token_type_string(),
                                    self.parser.get_token()) + "\n"
                self.parser.advance()
            line += self.compile_parameter_list()
            line += Helper.tags(self.parser.token_type_string(),
                                self.parser.get_token()) + "\n"
            self.parser.advance()  # {
            line += "<subroutineBody>\n"
            line += Helper.tags(self.parser.token_type_string(),
                                self.parser.get_token()) + "\n"
            self.parser.advance()
            arr = ["let", "if", "while", "do", "return"]
            while self.parser.get_token() not in arr:
                line += self.compile_var_dec()
            while self.parser.get_token() != "}":
                line += self.compile_statements()
                line += Helper.tags(self.parser.token_type_string(),
                                    self.parser.get_token()) + "\n"
                # self.parser.advance()
            line += "</subroutineBody>\n"
            line += "</subroutineDec>\n"
        return line

    def compile_parameter_list(self):

        """
        A method that translates a single parameter list.
        :return: The translated text.
        """

        line = ""
        line += "<parameterList>" + "\n"
        if self.parser.get_token() != ")":
            while self.parser.get_token() is not ")":
                line += Helper.tags(self.parser.token_type_string(),
                                    self.parser.get_token()) + "\n"
                self.parser.advance()
        line += "</parameterList>" + "\n"
        return line

    def compile_var_dec(self):

        """
        A method that translates a single var dec.
        :return: The translated text.
        """

        line = ""
        line += "<varDec>" + "\n"
        while self.parser.get_token() != ';':
            line += Helper.tags(self.parser.token_type_string(),
                                self.parser.get_token()) + "\n"
            self.parser.advance()
        line += Helper.tags(self.parser.token_type_string(),
                            self.parser.get_token()) + "\n"
        self.parser.advance()
        line += "</varDec>" + "\n"
        return line

    def compile_statements(self):

        """
        A method that translates a single statements.
        :return: The translated text.
        """
        line = ''
        line += '<statements>\n'
        if self.parser.get_token() != '}':
            while self.parser.get_token() in STATEMENTS:
                token = self.parser.get_token()
                if token == 'let':
                    line += self.compile_let()
                elif token == 'if':
                    line += self.compile_if()
                elif token == 'while':
                    line += self.compile_while()
                elif token == 'do':
                    line += self.compile_do()
                elif token == 'return':
                    line += self.compile_return()
                else:
                    self.parser.advance()
        line += '</statements>\n'
        return line

    def compile_do(self):

        """
        A method that translates a do statement.
        :return: The translated text.
        """

        line = "<doStatement>\n"
        for i in range(2):
            line += Helper.tags(self.parser.token_type_string(),
                                self.parser.get_token()) + "\n"
            self.parser.advance()
        if self.parser.get_token() == '(':
            line += Helper.tags(self.parser.token_type_string(),
                                self.parser.get_token()) + "\n"
            self.parser.advance()
            line += self.compile_expression_list()
            line += Helper.tags(self.parser.token_type_string(),
                                self.parser.get_token()) + "\n"
            self.parser.advance()
        elif self.parser.get_token() == '.':
            for i in range(3):
                line += Helper.tags(self.parser.token_type_string(),
                                    self.parser.get_token()) + "\n"
                self.parser.advance()
            line += self.compile_expression_list()
            line += Helper.tags(self.parser.token_type_string(),
                                self.parser.get_token()) + "\n"
            self.parser.advance()
        line += Helper.tags(self.parser.token_type_string(),
                            self.parser.get_token()) + "\n"
        self.parser.advance()
        line += "</doStatement>\n"
        return line

    def compile_let(self):

        """
        A method that translates a let statement.
        :return: The translated text.
        """

        line = "<letStatement>\n"
        for i in range(2):
            line += Helper.tags(self.parser.token_type_string(),
                                self.parser.get_token()) + "\n"
            self.parser.advance()
        if self.parser.get_token() == '[':
            line += Helper.tags(self.parser.token_type_string(),
                                self.parser.get_token()) + "\n"
            self.parser.advance()
            line += self.compile_expression()
            line += Helper.tags(self.parser.token_type_string(),
                                self.parser.get_token()) + "\n"
            self.parser.advance()
        line += Helper.tags(self.parser.token_type_string(),
                            self.parser.get_token()) + "\n"
        self.parser.advance()
        line += self.compile_expression()
        line += Helper.tags(self.parser.token_type_string(),
                            self.parser.get_token()) + "\n"
        self.parser.advance()
        line += "</letStatement>\n"
        return line

    def compile_while(self):

        """
        A method that translates a while statement.
        :return: The translated text.
        """

        line = "<whileStatement>\n"
        for i in range(2):
            line += Helper.tags(self.parser.token_type_string(),
                                self.parser.get_token()) + "\n"
            self.parser.advance()
        line += self.compile_expression()
        line += Helper.tags(self.parser.token_type_string(),
                            self.parser.get_token()) + "\n"
        self.parser.advance()
        line += Helper.tags(self.parser.token_type_string(),
                            self.parser.get_token()) + "\n"
        self.parser.advance()
        line += self.compile_statements()
        line += Helper.tags(self.parser.token_type_string(),
                            self.parser.get_token()) + "\n"
        self.parser.advance()
        line += "</whileStatement>\n"
        return line

    def compile_return(self):

        """
        A method that translates a return statement.
        :return: The translated text.
        """

        line = "<returnStatement>\n"
        line += Helper.tags(self.parser.token_type_string(),
                            self.parser.get_token()) + "\n"
        self.parser.advance()
        if self.parser.get_token() != ';':
            line += self.compile_expression()
        line += Helper.tags(self.parser.token_type_string(),
                            self.parser.get_token()) + "\n"
        self.parser.advance()
        line += "</returnStatement>\n"
        return line

    def compile_if(self):

        """
        A method that translates an if statement.
        :return: The translated text.
        """

        line = "<ifStatement>\n"
        for i in range(2):
            line += Helper.tags(self.parser.token_type_string(),
                                self.parser.get_token()) + "\n"
            self.parser.advance()
        line += self.compile_expression()
        for i in range(2):
            line += Helper.tags(self.parser.token_type_string(),
                                self.parser.get_token()) + "\n"
            self.parser.advance()
        line += self.compile_statements()
        line += Helper.tags(self.parser.token_type_string(),
                            self.parser.get_token()) + "\n"
        self.parser.advance()
        if self.parser.get_token() == 'else':
            line += Helper.tags(self.parser.token_type_string(),
                                self.parser.get_token()) + "\n"
            self.parser.advance()
            line += Helper.tags(self.parser.token_type_string(),
                                self.parser.get_token()) + "\n"
            self.parser.advance()
            line += self.compile_statements()
            line += Helper.tags(self.parser.token_type_string(),
                                self.parser.get_token()) + "\n"
            self.parser.advance()
        line += "</ifStatement>\n"
        return line

    def compile_expression(self):

        """
        A method that translates a single expression.
        :return: The translated text.
        """
        line = ''
        line += "<expression>\n"
        line += self.compile_term()
        while self.parser.get_token() in OP:
            line += Helper.tags(self.parser.token_type_string(),
                                self.parser.get_token()) + "\n"
            self.parser.advance()
            line += self.compile_term()
        line += "</expression>\n"
        return line

    def compile_term(self):

        """
        A method that translates a single term.
        :return: The translated text.
        """

        line = '<term>\n'
        if self.parser.get_token() == '(':
            line += Helper.tags(self.parser.token_type_string(),
                                self.parser.get_token()) + "\n"
            self.parser.advance()
            line += self.compile_expression()
            line += Helper.tags(self.parser.token_type_string(),
                                self.parser.get_token()) + "\n"
            self.parser.advance()
        elif self.parser.get_token() in {'-', '~'}:
            line += Helper.tags(self.parser.token_type_string(),
                                self.parser.get_token()) + "\n"
            self.parser.advance()
            line += self.compile_term()
        else:
            line += Helper.tags(self.parser.token_type_string(),
                                self.parser.get_token()) + "\n"
            self.parser.advance()
            if self.parser.get_token() == '[':
                line += Helper.tags(self.parser.token_type_string(),
                                    self.parser.get_token()) + "\n"
                self.parser.advance()
                line += self.compile_expression()
                line += Helper.tags(self.parser.token_type_string(),
                                    self.parser.get_token()) + "\n"
                self.parser.advance()
            elif self.parser.get_token() == '(':
                line += Helper.tags(self.parser.token_type_string(),
                                    self.parser.get_token()) + "\n"
                self.parser.advance()
                line += self.compile_expression_list()
                line += Helper.tags(self.parser.token_type_string(),
                                    self.parser.get_token()) + "\n"
                self.parser.advance()
            elif self.parser.get_token() == '.':
                for i in range(3):
                    line += Helper.tags(self.parser.token_type_string(),
                                        self.parser.get_token()) + "\n"
                    self.parser.advance()
                line += self.compile_expression_list()
                line += Helper.tags(self.parser.token_type_string(),
                                    self.parser.get_token()) + "\n"
                self.parser.advance()
        line += '</term>\n'
        return line

    def compile_expression_list(self):

        """
        A method that translates a single expression list.
        :return: The translated text.
        """
        line = ''
        line += '<expressionList>\n'
        while self.parser.get_token() != ")":
            line += self.compile_expression()
            if self.parser.get_token() != ',':
                break
            line += Helper.tags(self.parser.token_type_string(),
                                self.parser.get_token()) + "\n"
            self.parser.advance()
        line += '</expressionList>\n'
        return line


if __name__ == '__main__':
    main = CompilationEngine(sys.argv[1])
    main.main()
