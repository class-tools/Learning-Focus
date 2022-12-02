$itemarray = ".\learning_focus.py", ".\learning_focus.ico", ".\learning_focus.spec"
if (& Test-Path -Path ".\build") {
	& Remove-Item -Path ".\build" -Force -Recurse
}
& New-Item -Name ".\build" -ItemType "directory" > $null
foreach ($item in $itemarray) {
	& Copy-Item -Path $item -Destination ".\build" -Recurse
}
& Set-Location -Path ".\build"
& pyinstaller learning_focus.spec
& Set-Location -Path ".."