param (
    [Alias("r")]
    [Switch]$Release,

    [Alias("d")]
    [Switch]$Debug,

    [Switch]$Local
)

$ErrorActionPreference = "Stop"

$BuildType = "Debug"
if ($Release) { $BuildType = "Release" }

Write-Host "--- Building terme_examples ($BuildType) ---" -ForegroundColor Cyan

$ConanArgs = @(
    "install", ".",
    "--output-folder=build",
    "--build=missing",
    "-s", "build_type=$BuildType",y
    "-s", "compiler.cppstd=20"
)
if ($Local) {
    $ConanArgs += "-c", "user.terme_examples:local=True"
}
conan @ConanArgs

cmake -S . -B build `
    -G "Visual Studio 17 2022" `
    -DCMAKE_TOOLCHAIN_FILE="build/generators/conan_toolchain.cmake" `
    -DCMAKE_BUILD_TYPE=$BuildType

cmake --build build --config $BuildType

Write-Host "[SUCCESS] Build complete: build/$BuildType/terme_examples.exe" -ForegroundColor Green
