# ------------------------------ read params
param (
    [Alias("r")]
    [Switch]$Release,

    [Alias("d")]
    [Switch]$BuildDebug,

    [Parameter(ValueFromRemainingArguments = $true)]
    $RemainingArgs
)

# ------------------------------ error handling
$ErrorActionPreference = "Stop"

# ------------------------------ determine build type
$BuildType = "Debug"
if ($Release) { $BuildType = "Release" }

Write-Host "--- Preparing to Run Project ($BuildType) ---" -ForegroundColor Cyan

# ------------------------------ find and run executable
$ExeName = "terme_examples.exe"

# Search common paths for MSVC (config folders) and Single-Config generators
$PathsToTry = @(
    (Join-Path "build" (Join-Path $BuildType $ExeName)), 
    (Join-Path "build" $ExeName)
)

$FoundPath = $null
foreach ($Path in $PathsToTry) {
    if (Test-Path $Path) {
        $FoundPath = $Path
        break
    }
}

if ($null -ne $FoundPath) {
    Write-Host "[INFO] Running: $FoundPath" -ForegroundColor Green
    Write-Host "----------------------------------------------" -ForegroundColor Gray
    
    & $FoundPath $RemainingArgs
    
    $ExitCode = $LASTEXITCODE
}
else {
    Write-Host "[ERROR] $ExeName not found for $BuildType configuration." -ForegroundColor Red
    Write-Host "[TIP] Run .\xbuild.ps1 first." -ForegroundColor Yellow
    
    Write-Host "`n[DEBUG] Files found in build directory:" -ForegroundColor Gray
    if (Test-Path "build") {
        Get-ChildItem "build" -Recurse -Filter "*.exe" | Select-Object FullName
    } else {
        Write-Host "Build folder does not exist." -ForegroundColor Red
    }
    exit 1
}

Write-Host "----------------------------------------------" -ForegroundColor Gray
if ($ExitCode -eq 0) {
    Write-Host "[SUCCESS] Execution finished." -ForegroundColor Green
} else {
    Write-Host "[WARNING] Executable exited with code: $ExitCode" -ForegroundColor Yellow
}

exit $ExitCode