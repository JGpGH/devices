# test if venv is installed by checking for Scripts/activate.ps1
if (Test-Path -Path ".\Scripts\Activate.ps1") {
    Write-Host "Virtual environment is installed."
} else {
    python -m venv .
}

if ($?) {
    .\Scripts\Activate.ps1
} else {
    Write-Host "Failed to create virtual environment."
    exit 1
}

if ($?) {
    Write-Host "Virtual environment activated."
} else {
    Write-Host "Failed to activate virtual environment."
    exit 1
}

pip install -r requirements.txt
if ($?) {
    Write-Host "Virtual environment created and requirements installed successfully."
} else {
    Write-Host "Failed to install requirements."
    exit 1
}

python3 ensureInstalled.py
if ($?) {
    Write-Host "All dependencies are installed successfully."
} else {
    Write-Host "Failed to ensure all dependencies are installed."
    exit 1
}