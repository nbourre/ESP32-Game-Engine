# TEST 2120
Write-Host "Copying ESP32 Game Engine to Arduino libraries..."

# Define source and destination paths
$SRC = Get-Location
$DEST = "$env:USERPROFILE\Documents\Arduino\libraries\ESP32 Game Engine"

# Ensure destination exists
If (!(Test-Path $DEST)) {
    New-Item -ItemType Directory -Path $DEST -Force
}

# Copy all files while preserving structure
Copy-Item -Path "$SRC\*" -Destination "$DEST\" -Recurse -Force

Write-Host "Done!"
