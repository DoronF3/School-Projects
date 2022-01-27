import re
from enum import Enum

KEYWORD = {'class', 'constructor', 'function', 'method', 'field', 'static',
           'var',
           'int', 'char', 'boolean', 'void', 'true', 'false', 'null', 'this',
           'let',
           'do', 'if', 'else', 'while', 'return'}

SYMBOLS = {'{', '}', '(', ')', '[', ']', '.', ',', ';', '+', '-', '*', '/',
           '&',
           '|', '<', '>', '=', '~', '&gt;', '&lt;', '&quot;', '&amp;'}

STATEMENTS = ["let", "while", "if", "do", "return"]
# A statements array.

OP = ['+', '-', '*', '/', '&', '|', '<', '>', '=', '&gt;', '&lt;', '&quot;',
      '&amp;']
# An operator array.

SIGNS = {'>': '&gt;', '<': '&lt;', '\"': '&quot;', '&': '&amp;'}
# A sign dictionary.

IDENTIFIERS = {'static', 'field', 'local', 'arg'}

OP_DICT = {'+': "add", '-': "sub", '&amp;': "and", '|': "or",
           '&lt;': "lt", '&gt;': "gt", '=': "eq"}


class ID(Enum):
    """
    A class of enums.
    """

    STATIC, FIELD, ARG, VAR = range(4)


def is_keyword(s):
    """
    A method that checks if the given symbol is a keyword.
    :param s: The given symbol.
    :return: True if the symbol is a keyword otherwise, false.
    """

    return s in KEYWORD


def is_symbol(s):
    """
    A method that checks if the given symbol is a symbol.
    :param s: The given symbol.
    :return: True if the symbol is a symbol otherwise, false.
    """

    return s in SYMBOLS


def is_integer_const(s):
    """
    A method that checks if the given symbol is an integer const.
    :param s: The given symbol.
    :return: True if the symbol is an integer const, otherwise false.
    """

    return (re.match(r"[-+]?\d+$", s) is not None) and (0 <= int(s) <= 32767)


def is_string_const(s):
    """
    A method that checks if the given symbol is a string const.
    :param s: The given symbol.
    :return: True if the symbol is a string const, otherwise false.
    """

    return s.find("\"") == -1 and s.find("\n") == -1


def is_identifier(s):
    """
    A method that checks if the given symbol is an identifier.
    :param s: The given symbol.
    :return: True if the symbol is an identifier, otherwise false.
    """

    return re.match(r"^[A-Za-z_][A-Za-z_\d]*$", s) is not None


def tags(keyword, token):
    """
    A method that creates the tags for a given symbol.
    :param keyword: The type of the current token.
    :param token: The given token.
    :return: The following format <token_type> token </token_type>.
    """

    return "<{}> {} </{}>".format(keyword, token, keyword)


def is_function(s):
    """
    A method that
    :param s:
    :return:
    """

    return is_identifier(s.split('(')[0]) and s.strip()[-1] == ')'
