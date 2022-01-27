from Helper import *
from enum import Enum

SPACE_REGEX = r"([\[\])|{/=<;+&>~,\"*}(.\-\n])"

MULTI_LINE_COMMENT = {"/**", '/*'}

INLINE_COMMENT = "//"

REGEX = r"[^\s\w\d_|\^&+\-%*~/=!>.\",;(){}?<\[\]:]"
LINE_BREAK = {'\n', '\r\n'}


class T(Enum):

    """
    A class of enums.
    """

    KEYWORD, SYMBOL, IDENTIFIER, INT_CONST, STRING_CONST = range(5)


class JackTokenizer:

    """
    A class that as the tokenizer strips a given file and adds only the relevant symbols.
    """

    path = ' '
    input_stream = None
    index = 0
    curr_line = []
    curr_token = None
    curr_token_type = None

    def __init__(self, path):

        """
        A method that opens the input file/stream and preprocess it C-Like and opens the
        pre-processed file ready to tokenize it.
        """
        self.path = path

    @staticmethod
    def pre_process(path):

        """
        A method that receives the file path and prepossesses it.
        :param path: The path of the given file.
        """

        line = ''
        with open(path, "r")as r:
            with open(path.replace("jack", "o"), "w+") as w:
                while True:
                    line = JackTokenizer.comments_line(line, r)
                    if line:
                        line = JackTokenizer.change_line(line)
                        w.write(line)
                    line = r.readline()
                    if line:
                        line = line.strip()
                    else:
                        break
        w.close()
        r.close()

    @staticmethod
    def not_in_string(indexes, comment_ind):

        """
        A method that checks if the comment sign is inside a string
        :param indexes:indexes of strings
        :param comment_ind: the comment we want to check
        :return: true if comment is not in string literal
        """

        for element in indexes:
            if element[0] <= comment_ind <= element[1]:
                return False
        return True

    @staticmethod
    def comments_line(line: str, read):

        """
        A method that removes comments from the line
        :param line: line to uncomment 
        :param read: the read file 
        :return: the line uncommented or next line if there is nothing in the current line
        """

        s = ''
        string = set()
        if line.find("\"") > 0:
            i = line.find("\"")
            j = line.find("\"", i + 1)
            string.add(tuple([i, j]))
        for l in LINE_BREAK:
            if line.startswith(l):
                return s
        if line.find(INLINE_COMMENT) >= 0:
            if JackTokenizer.not_in_string(string, line.find(INLINE_COMMENT)):
                s += line[:line.find(INLINE_COMMENT)]
                return JackTokenizer.comments_line(s, read)
            return line
        for l in MULTI_LINE_COMMENT:
            if line.find(l) >= 0:
                if JackTokenizer.not_in_string(string,
                                               line.find(l)):
                    if line.find("*/") < 0:
                        while line:
                            if line.find("*/") >= 0 or line.find(
                                    '*/\n') >= 0:
                                break
                            line = read.readline()
                        break
                    else:
                        return line[
                               :line.find(l)] + " " + JackTokenizer.comments_line(
                            line[line.find("*/") + 2:], read)
                else:
                    return line
        else:
            if line:
                return line
            else:
                return s

    @staticmethod
    def change_line(line):

        """
        A method that adjust the line to her preproccess spec
        :param line: line to change
        :return: spaced line 
        """

        string = ''
        end = ''
        if line.find("\"") > 0:
            i = line.find("\"")
            j = line.find("\"", i + 1)
            string = line[i:j + 1]
            end = line[j + 1:]
            line = line[:i]
        end = re.sub(SPACE_REGEX, " \\1 ", end)
        end = re.sub(REGEX, "", end).strip()
        line = re.sub(SPACE_REGEX, " \\1 ", line)
        line = re.sub(REGEX, "", line).strip().replace("\t", " ")
        if string:
            string = re.sub("(\")", " \\1 ",
                            string)
        if len(line) + len(string) > 0:
            return line + string + end + '\n'
        else:
            return ''

    def has_more_tokens(self):

        """
        A method that checks if we have more tokens in the input.
        :return: True if there are, otherwise false.
        """

        if self.index >= len(self.curr_line):
            next_line = self.input_stream.readline()
            if next_line:
                lst = []
                i = 0
                while next_line.find("\"", i) > 0:
                    i = next_line.find("\"", i)
                    j = next_line.find("\"", i + 1)
                    lst += next_line[:i + 1].split(' ')
                    lst.append(next_line[i:j + 1].replace("\"", ''))
                    lst.append("\"")
                    i = j + 1
                next_line = next_line[i:]
                next_line = lst + next_line.split(" ")
                if next_line[-1].endswith("\n"):
                    next_line[-1] = next_line[-1][:-1]
                for i in range(len(next_line)):
                    if next_line[i] in SIGNS:
                        next_line[i] = SIGNS[next_line[i]]
                self.curr_line = next_line
                self.index = 0
            else:
                return False
        if self.curr_line[self.index] == '':
            self.index += 1
            return self.has_more_tokens()
        return True

    def advance(self):

        """
        A method that gets the next token from the input and makes it the current one.
        This method should only be called if hasMoreTokens() is true.
        Initially there is no current token.
        """

        if len(self.curr_line) > 0:
            s = self.curr_line[self.index].strip()
            self.index += 1
            if is_integer_const(s):
                self.curr_token = s
                self.curr_token_type = T.INT_CONST
            elif s == '&quot;':
                self.curr_token = self.get_string()
                self.curr_token_type = T.STRING_CONST
            elif is_symbol(s):
                self.curr_token = s
                self.curr_token_type = T.SYMBOL
            elif is_keyword(s):
                self.curr_token = s
                self.curr_token_type = T.KEYWORD
            elif is_identifier(s):
                self.curr_token = s
                self.curr_token_type = T.IDENTIFIER
        else:
            if self.has_more_tokens():
                self.advance()
            else:
                print("USER ERROR ")
                exit(-1)
        self.has_more_tokens()

    def get_string(self):

        """
        A method that returns the string literal
        :return:
        """

        s = ''
        while True:
            x = self.curr_line[self.index]
            if x == "&quot;":
                self.index += 1
                break
            if x == '':
                x = ' '
            s += x
            self.index += 1
        return s[1:-1]

    def get_token(self):
        return self.curr_token

    def token_type(self):

        """
        A method that returns the type of the current token.
        :return: One of the following: KEYWORD, SYMBOL, IDENTIFIER, INT_CONST,
        STRING_CONST.
        """

        return self.curr_token_type

    def key_word(self):

        """
        A method that returns the keyword of the current token.
        Should be called only when tokenType() is KEYWORD.
        :return: One of the following: CLASS, METHOD, FUNCTION, CONSTRUCTOR, INT,
        BOOLEAN, CHAR, VOID, VAR, STATIC, FIELD, LET, DO, IF, ELSE, WHILE, RETURN,
        TRUE, FALSE, NULL, THIS.
        """

        assert self.curr_token_type == T.KEYWORD
        return self.get_token()

    def symbol(self):

        """
        A method that returns the character of the current token.
        Should be called only when tokenType() is SYMBOL.
        :return: Char.
        """

        assert self.curr_token_type == T.SYMBOL
        return self.get_token()

    def identifier(self):

        """
        A method that returns the identifier of the current token.
        Should be called only when tokenType() is IDENTIFIER.
        :return: String.
        """

        assert self.curr_token_type == T.IDENTIFIER
        return self.get_token()

    def int_val(self):

        """
        A method that returns the integer value of the current token.
        Should be called only when tokenType() is INT_CONST.
        :return: Int.
        """

        assert self.curr_token_type == T.INT_CONST
        return int(self.get_token())

    def string_val(self):

        """
        A method that returns the string value of the current token,
        without the double quotes.
        Should be called only when tokenType() is STRING_CONST.
        :return: String.
        """

        assert self.curr_token_type == T.STRING_CONST
        return self.get_token()

    def token_type_string(self):

        """
        A method that returns the appropriate string according to the type of the token.
        """

        if self.curr_token_type == T.IDENTIFIER:
            return "identifier"
        elif self.curr_token_type == T.KEYWORD:
            return "keyword"
        elif self.curr_token_type == T.SYMBOL:
            return "symbol"
        elif self.curr_token_type == T.INT_CONST:
            return "integerConstant"
        elif self.curr_token_type == T.STRING_CONST:
            return "stringConstant"
        else:
            assert False

    def write_xml(self):

        """
        A method that writes xml for testing purpose.
        """

        p = lambda x, y: "<{}> {} </{}>\n".format(x, y, x)
        with open(self.path.replace('jack', "txml"), "w+") as w:
            w.write("<tokens>\n")
            while self.has_more_tokens():
                self.advance()
                w.write(p(self.token_type_string(), self.get_token()))
            w.write("</tokens>")
            w.close()
            self.input_stream.close()
            self.input_stream = None

    def close(self):

        """
        A method that closes the file.
        """

        if self.input_stream:
            self.input_stream.close()

    def run_tokenizer(self, path):

        """
        A method for running the tokenizer on a file.
        """

        self.path = path
        self.pre_process(path)
        self.input_stream = open(path.replace("jack", "o"), "r")
