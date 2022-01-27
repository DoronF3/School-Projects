class Code:

    """
    A class that initializes all the code dictionaries.
    """

    comp_dict = dict()
    # Create a comp dictionary.

    jump_dict = dict()
    # Create a jump dictionary.

    dest_dict = dict()
    # Create a dest dictionary.

    def __init__(self):

        """
        Initializes the dictionaries.
        """

        self.init_dicts()

    def dest(self, mnemonic):

        """
        A method that returns the binary code of the dest mnemonic,
        if not exist returns none.
        :param mnemonic: A given mnemonic.
        :return: The binary code of the dest mnemonic, else none.
        """

        return self.dest_dict.get(mnemonic)

    def comp(self, mnemonic):

        """
        A method that returns the binary code of the comp mnemonic,
        if not exist returns none.
        :param mnemonic: A given mnemonic.
        :return: The binary code of the comp mnemonic, else none.
        """

        return self.comp_dict.get(mnemonic)

    def jump(self, mnemonic):

        """
        A method that returns the binary code of the jump mnemonic,
        if not exist returns none.
        :param mnemonic: A given mnemonic.
        :return: The binary code of the jump mnemonic.
        """

        return self.jump_dict.get(mnemonic)

    def init_dicts(self):

        """
        A method that adds all the default pairs in the Hack language.
        """

        # dest dict
        self.dest_dict["null"] = "000"
        self.dest_dict["M"] = "001"
        self.dest_dict["D"] = "010"
        self.dest_dict["MD"] = "011"
        self.dest_dict["A"] = "100"
        self.dest_dict["AM"] = "101"
        self.dest_dict["AD"] = "110"
        self.dest_dict["AMD"] = "111"

        # jump dict
        self.jump_dict["null"] = "000"
        self.jump_dict["JGT"] = "001"
        self.jump_dict["JEQ"] = "010"
        self.jump_dict["JGE"] = "011"
        self.jump_dict["JLT"] = "100"
        self.jump_dict["JNE"] = "101"
        self.jump_dict["JLE"] = "110"
        self.jump_dict["JMP"] = "111"

        # comp dict
        # a=0
        self.comp_dict["0"] = "1110101010"
        self.comp_dict["1"] = "1110111111"
        self.comp_dict["-1"] = "1110111010"
        self.comp_dict["D"] = "1110001100"
        self.comp_dict["A"] = "1110110000"
        self.comp_dict["!D"] = "1110001101"
        self.comp_dict["!A"] = "1110110001"
        self.comp_dict["-D"] = "1110001111"
        self.comp_dict["-A"] = "1110110011"
        self.comp_dict["D+1"] = "1110011111"
        self.comp_dict["A+1"] = "1110110111"
        self.comp_dict["D-1"] = "1110001110"
        self.comp_dict["A-1"] = "1110110010"
        self.comp_dict["D+A"] = "1110000010"
        self.comp_dict["D-A"] = "1110010011"
        self.comp_dict["A-D"] = "1110000111"
        self.comp_dict["D&A"] = "1110000000"
        self.comp_dict["D|A"] = "1110010101"

        # a=1
        self.comp_dict["M"] = "1111110000"
        self.comp_dict["!M"] = "1111110001"
        self.comp_dict["-M"] = "1111110011"
        self.comp_dict["M+1"] = "1111110111"
        self.comp_dict["M-1"] = "1111110010"
        self.comp_dict["D+M"] = "1111000010"
        self.comp_dict["D-M"] = "1111010011"
        self.comp_dict["M-D"] = "1111000111"
        self.comp_dict["D&M"] = "1111000000"
        self.comp_dict["D|M"] = "1111010101"

        # a=1
        # Shift instructions.
        self.comp_dict["D<<"] = "1010110000"
        self.comp_dict["A<<"] = "1010100000"
        self.comp_dict["M<<"] = "1011100000"
        self.comp_dict["D>>"] = "1010010000"
        self.comp_dict["A>>"] = "1010000000"
        self.comp_dict["M>>"] = "1011000000"
