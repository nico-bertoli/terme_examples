# ------------------------------ read params
param (
    [Alias("r")]
    [Switch]$Release,
    [Alias("d")]
    [Switch]$BuildDebug
)

$ErrorActionPreference = "Stop"

$BuildType = "Debug"
if ($Release) { $BuildType = "Release" }

Write-Host "`n--- Starting Project Build Process ($BuildType) ---" -ForegroundColor Cyan

# ------------------------------ helper function
function Install-ConanDep {
    param ([string]$PackageName, [string]$Version, [string]$RepoUrl, [string]$BuildType, [string]$SubDir = "")

    $FullRef = "$PackageName/$Version"
    Write-Host "[INFO] Checking for $FullRef..." -ForegroundColor Gray
    
    $Search = conan list "$FullRef*" --format json | ConvertFrom-Json
    $hasPkg = $false
    if ($null -ne $Search.LocalCache."$FullRef") { $hasPkg = $true }

    if (-not $hasPkg) {
        Write-Host "[INFO] $FullRef not found. Cloning and creating..." -ForegroundColor Yellow
        $TmpDir = Join-Path ([System.IO.Path]::GetTempPath()) ([guid]::NewGuid().ToString())
        git clone --depth 1 $RepoUrl $TmpDir
        $ConanPath = $TmpDir
        if ($SubDir -ne "") { $ConanPath = Join-Path $TmpDir $SubDir }
        
        conan create $ConanPath --version $Version --build=missing -s build_type=$BuildType
        Remove-Item -Recurse -Force $TmpDir
    } else {
        Write-Host "[INFO] $FullRef found in cache." -ForegroundColor Green
    }
}

# ------------------------------dependencies
Install-ConanDep -PackageName "nbkit" -Version "1.0.0" -RepoUrl "https://github.com/nico-bertoli/nbkit.git" -BuildType $BuildType
Install-ConanDep -PackageName "terme" -Version "1.0.0" -RepoUrl "https://github.com/nico-bertoli/terme.git" -BuildType $BuildType -SubDir "terme"

# ------------------------------ build
Write-Host "`n--- Generating Local Build Files ---" -ForegroundColor Cyan

if (Test-Path "build") { Remove-Item -Recurse -Force "build" }
New-Item -Path "build" -ItemType Directory | Out-Null

Write-Host "[INFO] Running Conan Install..." -ForegroundColor Yellow
conan install . --output-folder=build --build=missing -s build_type=$BuildType
if ($LASTEXITCODE -ne 0) { exit 1 }

Write-Host "[INFO] Running CMake Configure..." -ForegroundColor Yellow

$ToolchainFile = Resolve-Path "build/build/generators/conan_toolchain.cmake"

cmake -S . -B build `
    -G "Visual Studio 17 2022" `
    -DCMAKE_TOOLCHAIN_FILE="$ToolchainFile" `
    -DCMAKE_BUILD_TYPE=$BuildType

Write-Host "[INFO] Running CMake Build..." -ForegroundColor Yellow
cmake --build build --config $BuildType

if ($LASTEXITCODE -eq 0) {
    Write-Host "`n[SUCCESS] Build finished! Executable is in build/build/$BuildType/" -ForegroundColor Green
} else {
    Write-Host "`n[ERROR] Build failed." -ForegroundColor Red
    exit 1
}