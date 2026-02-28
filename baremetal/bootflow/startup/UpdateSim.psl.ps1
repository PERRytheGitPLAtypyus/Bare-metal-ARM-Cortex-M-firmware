# Clear the console for a clean start
Clear-Host
Write-Host "Connecting to remote repository: https://cdn.update-server.internal..." -ForegroundColor Cyan
Start-Sleep -Seconds 2

$progress = 0
while($progress -le 100) {
    # This cmdlet creates the native Windows progress overlay
    Write-Progress -Activity "Downloading Critical System Patch KB50314" `
                   -Status "$progress% Complete" `
                   -PercentComplete $progress `
                   -CurrentOperation "Receiving data packet..."

    # Randomize timing to simulate real network jitter
    Start-Sleep -Milliseconds (Get-Random -Minimum 150 -Maximum 600)

    # Increment progress by a variable amount
    $progress += (Get-Random -Minimum 1 -Maximum 4)
}

Write-Host "`n[COMPLETED] All packages verified and staged." -ForegroundColor Green
Write-Host "Press any key to exit..."
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")