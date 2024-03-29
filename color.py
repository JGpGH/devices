class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

class SentenceBuilder:
    def __init__(self):
        self.sentence = ""

    def addText(self, text):
        self.sentence += text
        return self

    def applyColor(self, color):
        if hasattr(bcolors, color):
            self.sentence += getattr(bcolors, color)
        return self

    def resetColor(self):
        self.sentence += bcolors.ENDC
        return self

    def bold(self):
        self.sentence += bcolors.BOLD
        return self

    def underline(self):
        self.sentence += bcolors.UNDERLINE
        return self

    def build(self):
        # Ensure the sentence ends with a reset color to avoid spilling styles to following texts.
        return self.sentence + bcolors.ENDC
    
def Error(text):
    return SentenceBuilder().addText(text).applyColor("FAIL").bold().build()

def Warning(text):
    return SentenceBuilder().addText(text).applyColor("WARNING").bold().build()

def Info(text):
    return SentenceBuilder().addText(text).applyColor("OKCYAN").bold().build()

def Success(text):
    return SentenceBuilder().addText(text).applyColor("OKGREEN").bold().build()