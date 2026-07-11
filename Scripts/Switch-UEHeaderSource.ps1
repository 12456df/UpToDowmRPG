param(
    [Parameter(Mandatory = $true)]
    [string]$File
)

if (-not (Test-Path -LiteralPath $File)) {
    Write-Error "File not found: $File"
    exit 1
}

$full = (Resolve-Path -LiteralPath $File).Path
$dir = Split-Path $full -Parent
$name = Split-Path $full -Leaf
$candidates = New-Object System.Collections.Generic.List[string]

function Add-Candidate([string]$path) {
    if (-not [string]::IsNullOrWhiteSpace($path)) {
        [void]$candidates.Add($path)
    }
}

if ($full -match '(?i)[\\/]Public[\\/]') {
    $asPrivate = [regex]::Replace($full, '(?i)[\\/]Public[\\/]', '\Private\')
    if ($name -match '\.h$') {
        Add-Candidate ($asPrivate -replace '\.h$', '.cpp')
        Add-Candidate ($asPrivate -replace '\.h$', '.c')
    } elseif ($name -match '\.hpp$') {
        Add-Candidate ($asPrivate -replace '\.hpp$', '.cpp')
    }
    Add-Candidate $asPrivate
}
elseif ($full -match '(?i)[\\/]Private[\\/]') {
    $asPublic = [regex]::Replace($full, '(?i)[\\/]Private[\\/]', '\Public\')
    if ($name -match '\.cpp$') {
        Add-Candidate ($asPublic -replace '\.cpp$', '.h')
        Add-Candidate ($asPublic -replace '\.cpp$', '.hpp')
    } elseif ($name -match '\.c$') {
        Add-Candidate ($asPublic -replace '\.c$', '.h')
    }
    Add-Candidate $asPublic
}

$stem = [IO.Path]::GetFileNameWithoutExtension($name)
$ext = [IO.Path]::GetExtension($name).ToLowerInvariant()
$altExts = switch ($ext) {
    '.h'   { @('.cpp', '.c', '.cc', '.cxx') }
    '.hpp' { @('.cpp', '.cc', '.cxx') }
    '.cpp' { @('.h', '.hpp') }
    '.c'   { @('.h') }
    '.cc'  { @('.h', '.hpp') }
    '.cxx' { @('.h', '.hpp') }
    default { @() }
}
foreach ($e in $altExts) {
    Add-Candidate (Join-Path $dir ($stem + $e))
}

$target = $null
foreach ($c in $candidates) {
    if (Test-Path -LiteralPath $c) {
        $target = (Resolve-Path -LiteralPath $c).Path
        break
    }
}

if (-not $target) {
    Write-Host "Didn't find a corresponding file for: $full"
    Write-Host "Tried:"
    foreach ($c in $candidates) { Write-Host "  $c" }
    exit 2
}

$cursorCmd = Get-Command cursor -ErrorAction SilentlyContinue
if ($cursorCmd) {
    & cursor -g $target
    exit 0
}

$cursorExe = Join-Path $env:LOCALAPPDATA 'Programs\cursor\Cursor.exe'
if (Test-Path $cursorExe) {
    Start-Process -FilePath $cursorExe -ArgumentList @('-g', $target)
    exit 0
}

Invoke-Item -LiteralPath $target
