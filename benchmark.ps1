param(
    [int]$TimeoutSec = 20,
    [string]$CsvOut = "benchmark.csv"
)

$algos = @(
    'bubblesort','gnomesort','mergesort','quicksort','introsort',
    'heap_sort','insertion_sort','patience_sorting','radix_sort'
)

$tests = Get-ChildItem tests\*.in | Sort-Object Name

if ($tests.Count -eq 0) {
    Write-Host "Nu exista teste in folderul tests\"
    exit 1
}

Write-Host "Benchmark: $($algos.Count) algoritmi x $($tests.Count) teste = $($algos.Count * $tests.Count) rulari"
Write-Host "Timeout per rulare: ${TimeoutSec}s"
Write-Host ""

$results = New-Object System.Collections.Generic.List[object]

foreach ($algo in $algos) {
    $exe = ".\$algo.exe"
    if (-not (Test-Path $exe)) {
        Write-Host "SKIP: $exe nu exista"
        continue
    }
    $exePath = (Resolve-Path $exe).Path
    Write-Host "=== $algo ==="
    foreach ($t in $tests) {
        $tname = $t.BaseName
        $inPath = $t.FullName

        $psi = New-Object System.Diagnostics.ProcessStartInfo
        $psi.FileName = "cmd.exe"
        $psi.Arguments = "/c `"`"$exePath`" < `"$inPath`" > nul 2> nul`""
        $psi.UseShellExecute = $false
        $psi.CreateNoWindow = $true

        $sw = [System.Diagnostics.Stopwatch]::StartNew()
        $proc = [System.Diagnostics.Process]::Start($psi)
        $finished = $proc.WaitForExit($TimeoutSec * 1000)
        $sw.Stop()

        if (-not $finished) {
            try { $proc.Kill() } catch {}
            $status = "TIMEOUT"
            $timeMs = $TimeoutSec * 1000
        } elseif ($proc.ExitCode -ne 0) {
            $status = "FAIL($($proc.ExitCode))"
            $timeMs = [int]$sw.ElapsedMilliseconds
        } else {
            $status = "OK"
            $timeMs = [int]$sw.ElapsedMilliseconds
        }
        $proc.Dispose()

        Write-Host ("  {0,-12} {1,-9} {2,7} ms" -f $tname, $status, $timeMs)

        $results.Add([PSCustomObject]@{
            Algorithm = $algo
            Test = $tname
            TimeMs = $timeMs
            Status = $status
        })
    }
    Write-Host ""
}

$results | Export-Csv -Path $CsvOut -NoTypeInformation -Encoding UTF8
Write-Host "Rezultate salvate in $CsvOut"
