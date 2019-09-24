
$ModName = "RenewablePower"
$ModId = "DGiLzB3ZErWu2V"
$GameDir = "E:\Program Files\Epic Games\SatisfactoryEarlyAccess"


$CurrentDir = $PSScriptRoot
$ProjectDir = "$($CurrentDir)"
$CookedDir = "$($ProjectDir)\Saved\Cooked\WindowsNoEditor"
$PakDir = "$($ProjectDir)\Saved\Pak"

$CPPFolder = "$($ProjectDir)\Modloader\"

$PreZipDir = "$($ProjectDir)\PreZipDir"
$ZipDir = "$($ProjectDir)\ZipDir"

$RunGame = $false;

$Version="1.1.2"


if((Test-Path "$($PakDir)\FactoryGame\*") -eq $true){
    write-host "Cleaning Pak Dir"
    Remove-Item -Path "$($PakDir)\FactoryGame\*" -Recurse | out-null
}

New-Item -ItemType Directory -Path "$($PakDir)\FactoryGame\Content\FactoryGame" -Force | out-null
write-host "Copying Mod Pak Folder"
Copy-Item -Path "$($CookedDir)\FactoryGame\Content\FactoryGame\$($ModName)" -Recurse -Destination "$($PakDir)\FactoryGame\Content\FactoryGame\" | out-null

write-host "Packing Mod $($ModName).pak"
Set-Location -Path "$($PakDir)" | out-null
python C:\\Windows\\System32\\u4pak.py pack "$($ModName).pak" "FactoryGame"


if((Test-Path "$($GameDir)\FactoryGame\Content\Paks\$($ModName)_p.sig") -eq $false){
    write-host "Copying Sig File!"
    Copy-Item -Path "$($GameDir)\FactoryGame\Content\Paks\FactoryGame-WindowsNoEditor.sig" -Destination "$($GameDir)\FactoryGame\Content\Paks\$($ModName)_p.sig" | out-null
}

write-host "Copying Mod Pak to Content Folder"
Copy-Item -Path "$($PakDir)\$($ModName).pak" -Destination "$($GameDir)\FactoryGame\Content\Paks\$($ModName)_p.pak" | out-null
write-host "Copying Mod DLL to mods folder"
Copy-Item -Path "$($CPPFolder)\x64\Release\$($ModName).dll" -Destination "$($GameDir)\FactoryGame\Binaries\Win64\mods\$($ModName).dll" | out-null


if((Test-Path "$($PreZipDir)") -eq $false){
    write-host "Copying Pre Zip Folder!"
    New-Item -ItemType Directory -Path "$($PreZipDir)" -Force | out-null
}

Copy-Item -Path "$($CPPFolder)\x64\Release\$($ModName).dll" -Destination "$($PreZipDir)\$($ModName).dll"
Copy-Item -Path "$($GameDir)\FactoryGame\Content\Paks\$($ModName)_p.pak" -Destination "$($PreZipDir)\$($ModName)_p.pak"
Copy-Item -Path "$($GameDir)\FactoryGame\Content\Paks\$($ModName)_p.sig" -Destination "$($PreZipDir)\$($ModName)_p.sig"

echo @"
{
  "mod_id": "$($ModId)",
  "name": "$($ModName)",
  "version": "$($Version)",
  "sml_version": "v1.0.0-pr7",
  "description": "Renewable Power for Satisfactory",
  "authors": ["Mrhid6"]
}
"@ | set-content -Path "$($PreZipDir)\data.json"


if((Test-Path "$($ZipDir)") -eq $false){
    write-host "Copying Pre Zip Folder!"
    New-Item -ItemType Directory -Path "$($ZipDir)" -Force | out-null
}

$CompressPaths = @("$($PreZipDir)\$($ModName).dll", "$($PreZipDir)\$($ModName)_p.pak", "$($PreZipDir)\$($ModName)_p.sig","$($PreZipDir)\data.json")

Compress-Archive -Path $CompressPaths -DestinationPath "$($ZipDir)\$($ModName)_$($Version).zip" -force

[System.Media.SystemSounds]::Beep.Play()


if($RunGame -eq $true){
    & "$($GameDir)\FactoryGame\Binaries\Win64\FactoryGame-Win64-Shipping.exe"
}