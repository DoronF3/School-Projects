
class VMWriter:

    """
    A class that writes the VM commands onto the given file.
    """

    def __init__(self, path):

        """
        A c'tor that creates a new file and prepares it for writing VM commands.
        """
        self._file = open(path, "w+")

    def write_push(self, segment, index):

        """
        A method that writes a VM push command.
        :param segment: The segment we wish to push too.
        :param index: The index we wish to push too.
        """
        if type(index) is int:
            index = str(index)
        self._file.write(" ".join(["push", segment, index])+"\n")

    def write_pop(self, segment, index):

        """
        A method that writes a VM pop command.
        :param segment: The segment we wish to pop.
        :param index: The index we wish to pop too.
        """

        if type(index) is int:
            index = str(index)
        self._file.write(" ".join(["pop", segment, index])+"\n")

    def write_arithmetic(self, command):

        """
        A method that writes a VM arithmetic command.
        :param command: The command we wish to write.
        """

        self._file.write(command+"\n")

    def write_label(self, label):

        """
        A method that writes a VM label command.
        :param label: The label we wish to write.
        """

        self._file.write("label "+label+"\n")

    def write_goto(self, label):

        """
        A method that writes a VM goto command.
        :param label: The label we wish to write.
        """

        self._file.write("goto "+label+"\n")

    def write_if(self, label):

        """
        A method that writes a VM If-goto command.
        :param label: The label we wish to write.
        """

        self._file.write("if-goto "+label+"\n")

    def write_call(self, name, n_args):

        """
        A method that writes a VM call command.
        :param name: The name of the call we wish to write.
        :param n_args: The amount of arguments to write to the call.
        """

        if type(n_args) is int:
            n_args = str(n_args)
        self._file.write(" ".join(["call", name, n_args])+"\n")

    def write_function(self, name, n_locals):

        """
        A method that writes a VM function command.
        :param name: The name of the function we wish to write.
        :param n_locals: The amount of arguments to write to the function.
        """

        if type(n_locals) is int:
            n_locals = str(n_locals)
        self._file.write(" ".join(["function", name, n_locals])+"\n")

    def write_return(self):

        """
        A method that writes a VM return command.
        """

        self._file.write("return\n")

    def close(self):

        """
        A method that closes the output file.
        """

        self._file.close()
