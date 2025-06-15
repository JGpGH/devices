#!/bin/bash

[ -f ./bin/activate ] || {
    python3 -m venv .
}

source ./bin/activate

pip3 install -r requirements.txt || {
    echo "Failed to install requirements. Please check your Python and pip installation."
    exit 1
}

python3 ensureInstalled.py || {
    echo "Failed to ensure installation. Please check the script for errors."
    exit 1
}