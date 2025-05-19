# Create build directory if it doesn't exist
if (!(Test-Path -Path "build")) {
    New-Item -ItemType Directory -Path "build"
}

# Navigate to build directory
Set-Location -Path "build"

# Configure CMake
cmake -G "Visual Studio 17 2022" -A x64 ..

# Build the project
cmake --build . --config Release

# Return to original directory
Set-Location -Path ".."

Write-Host "Build complete! The executable can be found in build/bin/Release/"
