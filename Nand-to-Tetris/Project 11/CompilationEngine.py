from JackTokenizer import *
from SymbolTable import *

EXCEPTION_MESSAGE = "File not found , try another address "


class CompilationEngine:
    """
    A class that acts as the compilation engine and adds all the needed tags to the
    given file.
    """

    def __init__(self, parser, writer):

        """
        The c'tor for the compilation engine object.
        :param in_file_name: The given file path to translate.
        """
        self.if_counter = 0
        self.while_counter = 0
        self.parser = parser
        self.writer = writer
        self.class_name = None
        self.symbol = SymbolTable()

    def compile_class(self):

        """
        A method that translates a single class.
        """

        self.parser.advance(2)
        self.class_name = self.parser.get_token()
        self.parser.advance(2)
        while self.parser.get_token() != '}':
            self.compile_class_var_dec()
            self.compile_subroutine()

        self.writer.close()

    def compile_class_var_dec(self):

        """
        A method that translates a single class var dec.
        :return: The translated text.
        """

        arr = ["static", "field"]
        if self.parser.get_token() in arr:
            kind = self.parser.get_token()
            self.parser.advance()
            type_str = self.parser.get_token()
            self.parser.advance()
            while self.parser.get_token() is not ";":
                self.symbol.define(self.parser.get_token(), type_str, kind)
                self.parser.advance()
                if self.parser.get_token() == ',':
                    self.parser.advance()
            self.parser.advance()

    def compile_subroutine(self):

        """
        A method that translates a single subroutine.
        :return: The translated text.
        """

        arr = ['constructor', 'function', 'method']
        self.symbol.start_subroutine()
        self.if_counter = 0
        self.while_counter = 0
        num_of_local_vars = 0
        subroutine_type = self.parser.get_token()
        num_of_args = 1 if subroutine_type == 'method' else 0
        if self.parser.get_token() not in arr:
            return
        if self.parser.get_token() == arr[0]:
            self.parser.advance(2)
            name = self.parser.get_token()
            self.parser.advance(2)
            num_of_args += self.compile_parameter_list()
        else:
            self.parser.advance(2)
            name = self.parser.get_token()
            self.parser.advance(2)
            if subroutine_type == arr[2]:
                self.symbol.define('this', self.class_name, 'arg')
            num_of_args += self.compile_parameter_list()
        self.parser.advance(2)
        arr = ["let", "if", "while", "do", "return"]
        while self.parser.get_token() not in arr:
            assert self.parser.get_token() == 'var'
            num_of_local_vars += self.compile_var_dec(0)
        self.writer.write_function(self.name_t(name), num_of_local_vars)
        if subroutine_type == 'constructor':
            self.writer.write_push('constant',
                                   self.symbol.var_count('field'))
            self.writer.write_call('Memory.alloc',
                                   1)
            self.writer.write_pop('pointer', 0)
        if subroutine_type == 'method':
            self.writer.write_push('argument', 0)
            self.writer.write_pop('pointer', 0)
        while self.parser.get_token() != "}":
            self.compile_statements()
        self.parser.advance()

    def compile_parameter_list(self):

        """
        A method that translates a single parameter list.
        :return: The translated text.
        """
        i = 0
        if self.parser.get_token() != ")":
            while self.parser.get_token() is not ")":
                type_str = self.parser.get_token()
                self.parser.advance()
                self.symbol.define(self.parser.get_token(), type_str, 'arg')
                self.parser.advance()
                if self.parser.get_token() == ',':
                    self.parser.advance()
                i += 1
        return i

    def compile_var_dec(self, num_of_local_vars):

        """
        A method that translates a single var dec.
        :param num_of_local_vars:
        :return: The translated text.
        """

        self.parser.advance()
        type_str = self.parser.get_token()
        self.parser.advance()
        while self.parser.get_token() != ';':
            num_of_local_vars += 1
            self.symbol.define(self.parser.get_token(), type_str, 'local')
            self.parser.advance()
            if self.parser.get_token() == ',':
                self.parser.advance()
        self.parser.advance()
        return num_of_local_vars

    def compile_statements(self):

        """
        A method that translates a single statements.
        :return: The translated text.
        """
        if self.parser.get_token() != '}':
            while self.parser.get_token() in STATEMENTS:
                token = self.parser.get_token()
                if token == 'let':
                    self.compile_let()
                elif token == 'if':
                    self.compile_if()
                elif token == 'while':
                    self.compile_while()
                elif token == 'do':
                    self.compile_do()
                elif token == 'return':
                    self.compile_return()
                else:
                    self.parser.advance()

    def compile_do(self):

        """
        A method that translates a do statement.
        :return: The translated text.
        """

        self.parser.advance()
        self.compile_term()
        self.writer.write_pop('temp', 0)
        self.parser.advance()

    def compile_let(self):

        """
        A method that translates a let statement.
        :return: The translated text.
        """

        self.parser.advance()
        var_name = self.parser.get_token()
        self.parser.advance()
        if self.parser.get_token() == '[':
            self.parser.advance()
            self.compile_expression()
            self.writer.write_push(self.symbol.kind_of(var_name, True),
                                   self.symbol.index_of(var_name))

            self.writer.write_arithmetic('add')

            self.parser.advance(2)
            self.compile_expression()
            self.writer.write_pop('temp',0)
            self.writer.write_pop('pointer', 1)
            self.writer.write_push('temp',0)
            self.parser.advance()
            self.writer.write_pop('that', 0)
        else:
            self.parser.advance()
            self.compile_expression()
            self.writer.write_pop(self.symbol.kind_of(var_name, True),
                                  self.symbol.index_of(var_name))
            self.parser.advance()

    def compile_while(self):

        """
        A method that translates a while statement.
        :return: The translated text.
        """
        num = self.while_counter
        self.parser.advance(2)
        self.writer.write_label("WHILE_EXP" + str(num))
        self.while_counter += 1
        self.compile_expression()
        self.writer.write_arithmetic('not')
        self.writer.write_if("WHILE_END" + str(num))
        self.parser.advance(2)
        self.compile_statements()
        self.parser.advance()
        self.writer.write_goto("WHILE_EXP" + str(num))
        self.writer.write_label("WHILE_END" + str(num))

    def compile_return(self):

        """
        A method that translates a return statement.
        :return: The translated text.
        """

        self.parser.advance()
        if self.parser.get_token() != ';':
            self.compile_expression()
        else:
            self.writer.write_push('constant', 0)
        self.writer.write_return()
        self.parser.advance()

    def compile_if(self):

        """
        A method that translates an if statement.
        :return: The translated text.
        """
        num = self.if_counter
        self.parser.advance(2)
        self.if_counter += 1
        self.compile_expression()
        self.writer.write_if("IF_TRUE" + str(num))
        self.writer.write_goto("IF_FALSE" + str(num))
        self.writer.write_label("IF_TRUE" + str(num))
        self.parser.advance(2)
        self.compile_statements()
        self.parser.advance()
        cond = self.parser.get_token() == 'else'
        if cond:
            self.parser.advance(2)
            self.writer.write_goto("IF_END" + str(num))
        self.writer.write_label("IF_FALSE" + str(num))
        if cond:
            self.compile_statements()
            self.parser.advance()
            self.writer.write_label("IF_END" + str(num))

    def compile_expression(self):

        """
        A method that translates a single expression.
        :return: The translated text.
        """
        self.compile_term()
        while self.parser.get_token() in OP:
            cur_op = self.parser.get_token()
            self.parser.advance()
            self.compile_term()
            if cur_op.strip() in OP_DICT:
                self.writer.write_arithmetic(OP_DICT[cur_op])
            elif cur_op == "*":
                self.writer.write_call("Math.multiply", 2)
            elif cur_op == '/':
                self.writer.write_call("Math.divide", 2)

    def compile_term(self):

        """
        A method that translates a single term.
        :return: The translated text.
        """
        var_name = self.parser.get_token()
        if self.parser.get_token() == '(':
            self.parser.advance()
            self.compile_expression()
            self.parser.advance()
            return
        elif var_name in {'true', 'false', 'null'}:
            self.writer.write_push('constant', 0)
            if var_name == 'true':
                self.writer.write_arithmetic('not')
            self.parser.advance()
            return
        elif self.parser.token_type() == T.INT_CONST:
            self.writer.write_push('constant', var_name)
            self.parser.advance()
            return
        elif self.parser.get_token() in {'-', '~'}:
            op = 'neg' if self.parser.get_token() == '-' else 'not'
            self.parser.advance()
            self.compile_term()
            self.writer.write_arithmetic(
                op)  # Assuming compile_term/expression pushing the argument  to the stack
            return
        elif self.parser.token_type() == T.STRING_CONST:
            self.writer.write_push("constant", len(var_name))
            self.writer.write_call("String.new", 1)
            for i in range(len(var_name)):
                self.writer.write_push("constant", ord(var_name[i]))
                self.writer.write_call("String.appendChar", 2)
            self.parser.advance()
            return
        else:
            self.parser.advance()
            if self.parser.get_token() == '[':
                self.parser.advance()
                self.compile_expression()
                self.writer.write_push(self.symbol.kind_of(var_name, True),
                                       self.symbol.index_of(var_name))
                self.writer.write_arithmetic('add')
                self.writer.write_pop('pointer', 1)
                self.writer.write_push('that', 0)
                self.parser.advance()
                return
            elif self.parser.get_token() == '(':
                n_args = 1
                self.parser.advance()
                self.writer.write_push('pointer', 0)
                n_args += self.compile_expression_list()
                self.writer.write_call(self.name_t(var_name),
                                       n_args)
                self.parser.advance()

                return
            elif self.parser.get_token() == '.':
                n_args = 0
                static = True
                if self.symbol.kind_of(var_name):
                    static = False
                    obj_name = var_name
                    var_name = self.symbol.type_of(var_name)
                for i in range(2):
                    var_name += self.parser.get_token()
                    self.parser.advance()
                self.parser.advance()
                if not static:
                    n_args += 1
                    self.writer.write_push(self.symbol.kind_of(obj_name, True),
                                           (self.symbol.index_of(obj_name)))
                n_args += self.compile_expression_list()
                self.writer.write_call(var_name, n_args)

                self.parser.advance()
                return
            elif var_name == 'this':
                self.writer.write_push('pointer', 0)
            elif self.symbol.kind_of(var_name):
                self.writer.write_push(self.symbol.kind_of(var_name, True),
                                       self.symbol.index_of(var_name))
                return

    def compile_expression_list(self):

        """
        A method that translates a single expression list.
        :return: The translated text.
        """
        sum = 0
        while self.parser.get_token() != ")":
            self.compile_expression()
            sum += 1
            if self.parser.get_token() != ',':
                break
            self.parser.advance()
        return sum

    def name_t(self, x):
        """

        :param x:
        :return:
        """
        return '{}.{}'.format(self.class_name, x)
