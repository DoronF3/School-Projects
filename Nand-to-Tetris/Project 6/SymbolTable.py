class SymbolTable:

    """
    A class that represents the symbols table of the program.
    """

    symbol_dict = dict()
    # Create a new dictionary.

    def __init__(self):

        """
        Initializes the dictionary.
        """

        self.initialize_dict()

    def add_entry(self, symbol, address):

        """
        A method that adds the pair (symbol,address) to the table.
        :param symbol: The given string to add.
        :param address: The number of the memory register to save it in.
        """

        self.symbol_dict[symbol] = address

    def contains(self, symbol):

        """
        A method that checks if a given symbol is in the table.
        :param symbol: The string to check.
        :return: True if the symbol exists, False otherwise.
        """

        x = self.symbol_dict.get(symbol)
        return x is not None

    def get_address(self, symbol):

        """
        A method that given a symbol returns its relevant address.
        :param symbol: The string to check.
        :return: The address of the given symbol.
        """

        return self.symbol_dict.get(symbol)

    def initialize_dict(self):

        """
        A method that initializes the dictionary with the hack default symbols.
        """

        self.symbol_dict['SP'] = 0
        self.symbol_dict['LCL'] = 1
        self.symbol_dict['ARG'] = 2
        self.symbol_dict['THIS'] = 3
        self.symbol_dict['THAT'] = 4
        self.symbol_dict['SCREEN'] = 16384
        self.symbol_dict['KBD'] = 24576
        for i in range(16):
            self.symbol_dict['R' + str(i)] = i
