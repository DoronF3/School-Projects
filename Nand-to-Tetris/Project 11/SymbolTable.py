from Helper import *


class SymbolTable:
    """
    A class representing the abstraction of Symbol table
    """
    __class = None
    __sub_routine = None
    __indexes = None

    def __init__(self):
        """
        A c'tor that creates a new empty symbol table.
        :return:
        """

        self.__class = dict()
        self.__sub_routine = dict()
        self.__indexes = {identifier: 0 for identifier in IDENTIFIERS}

    def start_subroutine(self):
        """
        A method that starts a new subroutine scope.
        :return:
        """

        self.__sub_routine = dict()
        for name, val in self.__sub_routine.items():
            if val[1] in {'arg', 'local'}:
                del self.__sub_routine[name]
        self.__indexes['local']=0
        self.__indexes['arg'] = 0
    def define(self, name, type_str, kind):
        """
        A method that defines a new identifier of a given name, type, and kind and assigns it a
        running index. STATIC and FIELD identifiers have a class scope, while ARG and VAR
        identifiers have a subroutine scope.
        :param name: The name of the identifier.
        :param type_str: The type of the given identifier given in string format.
        :param kind: The kind of the identifier.
        :return:
        """

        assert kind in IDENTIFIERS  # todo decide if string representation or enum
        if kind in {'static', 'field'}:
            self.__class[name] = (type_str, kind, self.__indexes[kind])
        else:
            if kind == 'var':
                self.__sub_routine[name] = (
                type_str, 'local', self.__indexes[kind])
            else:
                self.__sub_routine[name] = (
                type_str, kind, self.__indexes[kind])
        self.__indexes[kind] += 1

    def var_count(self, kind):
        """
        A method that returns the number of variables of the given kind already defined in the
        current scope.
        :param kind: The given kind we wish to check.
        :return: The number of the given variables defined in the scope.
        """
        assert kind in IDENTIFIERS
        return self.__indexes[kind]

    def kind_of(self, name,to_write=False):
        """
        A method that returns the kind of the named identifier in the current scope.
        :param name: The name of the current identifier.
        :return: The kind of the given named identifier, if doesn't exist in current scope retruns NONE.
        """

        if name in self.__sub_routine:
            if self.__sub_routine[name][1]=='arg' and to_write:
                return 'argument'
            return self.__sub_routine[name][1]
        elif name in self.__class:
            if self.__class[name][1]=='field' and to_write:
                return 'this'
            return self.__class[name][1]
        return None

    def type_of(self, name):
        """
        A method that returns the type of the named identifier in the current scope.
        :param name: The name of the current type.
        :return: The type of the identifier in the current scope.if doesn't exist in current scope retruns NONE
        """

        if name in self.__sub_routine:
            return self.__sub_routine[name][0]
        elif name in self.__class:
            return self.__class[name][0]
        return None

    def index_of(self, name):
        """
        A method that returns the index assigned to named identifier.
        :param name: The name of the given identifier.
        :return: The index assigned to the named identifier.if doesn't exist in current scope retruns NONE
        """

        if name in self.__sub_routine:
            return self.__sub_routine[name][2]
        elif name in self.__class:
            return self.__class[name][2]
        return None
