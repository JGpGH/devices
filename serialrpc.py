from serialbuffer import Type, Buffer

class Variable:
    def __init__(self, name, type: Type, value):
        self.name = name
        self.type = type
        self.value = value

    def encode(self, buffer: Buffer):
        buffer.encode(self.value, self.type)

    def decode(self, buffer: Buffer):
        value, success = buffer.decode(self.type)
        if success:
            self.value = value
        return success


    

