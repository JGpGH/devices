import os
import sys

def buildAndRun(program, fbqn, port):
    os.system(f"arduino-cli compile --fqbn {fbqn} {program}")
    os.system(f"arduino-cli upload -p {port} --fqbn {fbqn} {program}")

def main():
    program = sys.argv[1]
    fbqn = sys.argv[2]
    port = sys.argv[3]
    buildAndRun(program, fbqn, port)

if __name__ == "__main__":
    main()