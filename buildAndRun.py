import os
import sys

def buildAndRun(device, fbqn, port):
    os.system(f"arduino-cli compile --fqbn {fbqn} {device}")
    os.system(f"arduino-cli upload -p {port} --fqbn {fbqn} {device}")

def main():
    device = sys.argv[1]
    fbqn = sys.argv[2]
    port = sys.argv[3]
    buildAndRun(device, fbqn, port)

if __name__ == "__main__":
    main()