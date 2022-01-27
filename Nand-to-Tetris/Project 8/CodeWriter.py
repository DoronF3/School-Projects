from Parser import Commands

DEFAULT_TMP_ADD = '5'

DEFAULT_POINTER_ADD = '3'


def SP_inc():
    """
    A method that returns an assembly code to increment SP by 1.
    :return: SP++ in assembly.
    """

    return "@SP\n" + "M=M+1"


def SP_dec():
    """
    A method that returns an assembly code to decrement SP by 1.
    :return: SP-- in assembly.
    """

    return "@SP\n" + "M=M-1"


def pop_D():
    """
    A method that pops the value on top of the stack to D-register
    :return: The assembly code to represent the given action.
    """

    return SP_dec() + "\nA=M\n" + "D=M"


def push_D():
    """
    A method that pushes the value in D-register to the top of the stack
    :return: The assembly code to represent the given action.
    """

    return "@SP\n" + "A=M\n" + "M=D"


def index_D(index):
    """
    A method that sets the index in to D-register
    :return: The assembly code to represent the given action.
    """

    return "@" + str(index) + "\nD=A"


def M_TO_D():
    """
    Set the M- value to D-register
    :return:
    """
    return "M=D"


def M_to_M_PLUS_D():
    """
    set M to be M+D
    :return:
    """
    return "M=M+D"


def D_to_M():
    """
    set the value of D- register to M
    :return:
    """
    return "D=M"


def A_TO_M():
    """
    set the A-register value to M
    :return:
    """
    return "A=M"


def put_tmp(n):
    """
    A method that puts value in @R n memory address
    :param n: int could be 13, 14 ,15
    :return: The assembly code to represent the given action.
    """

    return "@R" + str(n) + "\n" + "M=D"


def get_tmp(n):
    """
    A method that gets value in @R n memory address to D-register
    :param n: int could be 13, 14 ,15
    :return: The assembly code to represent the given action.
    """

    return "@R" + str(n)


def get_ram_add(segment):
    """
    A method that gets to the ram address after having index in D-register
    :param segment: The given segment to work with.
    :return: The assembly code to represent the given action.
    """
    if segment == "3" or segment == "5":
        return "@" + segment + "\n" + "D=A"
    return "@" + segment + "\n" + "D=M"


def get_binary(operation):
    """
    Return the string for in assembly language for the binary operations :
    add, sub, and , or
    *Expects that operation in [+,-,&,|]
    :param operation: string represent the operation
    :return: string of the assembly command
    """

    return "\n".join(
        [get_numbers(), get_tmp(13), "D=D" + operation + "M", push_D(),
         SP_inc()])


def get_unary(operation):
    """
    Method for translating unary arithmetic VM command to HACK assembly
    language
    * Expects that operation in [-,!]
    :param operation: string represent the operation
    :return: string of the assembly command
    """

    return "\n".join([SP_dec(), "A=M", "M=" + operation + "M", SP_inc()])


def get_numbers():
    """
    A method that pops the first value in the stack stores it in RAM[13],
    and then pops another value out of the stack and stores it on the D-register.
    :return: The assembly syntax for the operation.
    """

    final = "\n".join([pop_D(), put_tmp(13), pop_D()])
    return final


def get_jump(comp):
    """
    Method for returning jump command in assembly hack lang
    :param comp: one of 'lt','eq','gt','jmp'
    :return: the assembly command
    """
    assert comp in ['lt', 'eq', 'gt', 'jmp', 'jne'], "Bad comparision"
    if comp == 'lt':
        return "D;JLT"
    elif comp == 'eq':
        return "D;JEQ"
    elif comp == 'gt':
        return "D;JGT"
    elif comp == 'jne':
        return "D;JNE"
    else:
        return "0;JMP"


def get_comparator(command, index):
    """
    A method that given a comparison command pushes to the stack true or false.
    :param command: The type of comparison we wish to evaluate.
    :param index: The index of the label we wish to enter.
    :return: The assembly syntax for the operation.
    """

    assert command in ['lt', 'eq', 'gt'], "Bad comparision"
    same_sgn = ["@TRUE{}".format(index),
                get_jump(command), "@SP", A_TO_M(), "M=0",
                "@FALSE{}".format(index),
                get_jump('jmp'),
                "(TRUE{})".format(index),
                "@SP", A_TO_M(), "M=-1", "(FALSE{})".format(index), SP_inc()]
    if command == "eq":
        return "\n".join([get_numbers(), get_tmp(13), "D=D-M"] + same_sgn)
    flag = [-1, 0]  # default for lt
    if command == "gt":
        flag = flag[::-1]
    return '\n'.join(
        [pop_D(), put_tmp(13), get_tmp(13), D_to_M(), "@YNEG{}".format(index),
         "D;JLT", pop_D(), put_tmp(14), get_tmp(14), D_to_M(),
         "@XNEGYPOS{}".format(index), get_jump('lt'),
         "@SAMESGN{}".format(index),
         "0;JMP", "(YNEG{})".format(index), pop_D(), put_tmp(14), get_tmp(14),
         D_to_M(), "@XPOSYNEG{}".format(index), get_jump('gt'),
         "(SAMESGN{})".format(index), get_tmp(14), D_to_M(), get_tmp(13),
         "D=D-M", ] + same_sgn + ["@END{}".format(index), get_jump('jmp'),
                                  "(XNEGYPOS{})".format(index),
                                  "D={}".format(flag[0]), push_D(), SP_inc(),
                                  "@END{}".format(index),
                                  get_jump('jmp'),
                                  "(XPOSYNEG{})".format(index),
                                  "D={}".format(flag[1]),
                                  push_D(), SP_inc(), "(END{})".format(index)])


class CodeWriter:
    """
    A class that initializes all the code dictionaries.
    """

    curr_func = ''
    func_index = 0
    label_index = 0
    segments1_dict = {'local': 'LCL', 'argument': 'ARG', 'this': 'THIS',
                      'that': 'THAT', 'pointer': DEFAULT_POINTER_ADD,
                      'temp': DEFAULT_TMP_ADD}

    # Create a segment dictionary for segments: local, this, that,pointer
    # and args.
    # The temp and pointer are set to their base addresses : 5,3

    def __init__(self, file_name=''):

        """
        Initializes the dictionaries.
        """
        self.label_index = 0
        self.file_name = file_name

    def file_name_setter(self, file_name):
        self.file_name = file_name

    def write_arithmetic(self, command):

        """
        A method that writes to the output file the assembly code that implements
        the given command.
        :param command: The current command we need to translate.
        :return: The assembly code for the current command.
        or false if the command is not legal
        """

        if command == "add":
            return get_binary('+')
        elif command == "sub":
            return get_binary('-')
        elif command == "and":
            return get_binary('&')
        elif command == "or":
            return get_binary('|')
        elif command == "neg":
            return get_unary('-')
        elif command == "not":
            return get_unary('!')
        elif command == "eq" or command == "gt" or command == "lt":
            self.label_index += 1
            # Check equality commands (equal, greater than, lower than).
            return get_comparator(command, self.label_index)
        return False

    def write_push_pop(self, command, segment, index):

        """
        A method that writes to the output file the assembly code that implements
        the given command.
        :param command: The current command we need to translate.
        :param segment: The segment we need to work with.
        :param index: The index of the segment.
        :return: The assembly code for the current command, segment and index.
                or False if illegal command or segment or index.
        """

        if command == Commands.PUSH:
            # Push command.
            if segment in self.segments1_dict.keys():
                # local, arg, this ,that segment
                return "\n".join(
                    [index_D(index), put_tmp(15),
                     get_ram_add(self.segments1_dict[segment]),
                     put_tmp(14), get_tmp(15), D_to_M(), get_tmp(14),
                     M_to_M_PLUS_D(),
                     D_to_M(), "A=D", D_to_M(),
                     push_D(), SP_inc()])
            elif segment == "constant":
                return "\n".join([index_D(index), push_D(), SP_inc()])
            elif segment == "static":
                return "\n".join(
                    ["@{}.{}".format(self.file_name, str(index)),
                     D_to_M(),
                     push_D(), SP_inc()])
        elif command == Commands.POP:
            # Pop command.
            if segment in self.segments1_dict.keys():
                return "\n".join(
                    [pop_D(), put_tmp(13), index_D(index), put_tmp(15),
                     get_ram_add(self.segments1_dict[segment]),
                     put_tmp(14),
                     get_tmp(15), D_to_M(), get_tmp(14),
                     M_to_M_PLUS_D(),
                     get_tmp(13), D_to_M(), get_tmp(14), A_TO_M(),
                     M_TO_D()])
            elif segment == "static":
                return "\n".join(
                    [pop_D(), "@{}.{}".format(self.file_name, str(index)),
                     M_TO_D()])
        return False

    # PROJECT 8 starts from here ///////////////////////////////////////////

    def write_init(self):
        """
        Writes the assembly code that effects the
        VM initialization (also called bootstrap
        code). This code should be placed in the
        ROM beginning in address 0x0000.
        :return: The assembly code for the current command.
        """
        return '\n'.join(["@256", "D=A", "@SP", M_TO_D()]) + \
               '\n' + self.write_call(
            "Sys.init", 0)

    def write_label(self, label):
        """
        Writes the assembly code that is the
        translation of the given label command.
        :return: The assembly code for the current command.
        """
        return "({}:{})\n".format(self.curr_func, label)

    def write_goto(self, label):
        """
        Writes the assembly code that is the
        translation of the given goto command
        :return: The assembly code for the current command.
        """
        return '\n'.join(
            ["@{}:{}".format(self.curr_func, label), get_jump('jmp')])

    def write_if(self, label):
        """
        Writes the assembly code that is the
        translation of the given if-goto command.

        """
        return '\n'.join(
            [pop_D(), "@{}:{}".format(self.curr_func, label), get_jump('jne')])

    def write_call(self, function_name, num_args):
        """
        Writes the assembly code that is the
        translation of the given Call command.
        :return: The assembly code for the current command.
        """
        # push ret add
        final = "\n".join(
            ["@RET_{}{}".format(function_name, self.func_index), "D=A",
             push_D(), SP_inc()])
        # push LCL , ARG ,THIS, THAT
        for segment in ["LCL", "ARG", "THIS", "THAT"]:
            final = '\n' + "\n".join(
                [final, "@{}".format(segment), D_to_M(), push_D(), SP_inc()])
        # ARG=SP-n-5 == ARG=SP-(n+5)
        final = '\n' + '\n'.join(
            [final, "@SP", D_to_M(), "@{}".format(num_args + 5), "D=D-A",
             "@ARG",M_TO_D()])
        # LCL = SP
        final += '\n' + "\n".join(["@SP", D_to_M(), "@LCL", M_TO_D()])
        # goto f
        final += '\n' + "\n".join(
            ["@{}".format(function_name), get_jump('jmp')])
        # write return label
        final += '\n' + "\n".join(
            ["(RET_{}{})".format(function_name, self.func_index)])
        self.func_index += 1
        return final

    def write_return(self):
        """
        Writes the assembly code that is the
        translation of the given Return command.
        """
        # Frame=LCL frame is local var
        final = "\n".join(["@LCL", D_to_M(), put_tmp(15)])
        # RET =* (Frame-5)
        final = '\n'.join(
            [final, "@5", "D=A", get_tmp(15), "A=M-D", D_to_M(), put_tmp(13)])
        # *ARG=pop()
        final = '\n'.join([final, pop_D(), "@ARG", A_TO_M(), M_TO_D()])
        # SP = ARG +1
        final = '\n'.join([final, "@ARG", D_to_M(), "@SP", "M=D+1"])
        # THAT,THIS,ARG,LCL = *(Frame-1) ,*(Frame-2),*(Frame-3),*(Frame-4)
        for segment in ["THAT", "THIS", "ARG", "LCL"]:
            final = '\n'.join([final, get_tmp(15), "M=M-1", A_TO_M(), D_to_M(),
                               "@{}".format(segment), M_TO_D()])
        # goto RET
        final = '\n'.join([final, get_tmp(13), A_TO_M(),get_jump('jmp')])
        return final

    def write_function(self, function_name, num_locals):
        """
        Writes the assembly code that is the trans. of
        the given Function command.
        :return: The assembly code for the current command.
        """
        self.curr_func = function_name
        return '\n'.join(['({})'.format(function_name)] +
                         ['\n'.join(
                             ['\n'.join(["D=0", push_D(), SP_inc()]) for i in
                              range(num_locals)])])
