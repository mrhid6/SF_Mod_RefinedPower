
$ModName = "RenewablePower"
$ModId = "DGiLzB3ZErWu2V"
$GameDir = "E:\Program Files\Epic Games\SatisfactoryEarlyAccess"
$UnrealEngine = "E:\Program Files\Epic Games\UE_4.21\Engine"

$CookUnreal = $false;
$RunGame = $false;

$PakOnly = $false;


$Version="1.2.0"

$DataJson = @"
{
  "mod_id": "$($ModId)",
  "name": "$($ModName)",
  "version": "$($Version)",
  "sml_version": "v1.0.2",
  "description": "Renewable Power for Satisfactory",
  "authors": ["Mrhid6"],
  "objects": [
     {
        "type": "sml_mod",
        "path": "$($ModName).dll"
     },
     {
        "type": "pak",
        "path": "$($ModName)_p.pak"
     }
  ],
  "dependencies": {
    "CkUs5KM9ShwVfr": "^1.0.0"
  }
}
"@

## Do Not Edit

$CurrentDir = $PSScriptRoot
$ProjectDir = "$($CurrentDir)"
$CookedDir = "$($ProjectDir)\Saved\Cooked\WindowsNoEditor"
$PakDir = "$($ProjectDir)\Saved\Pak"
$CPPFolder = "$($ProjectDir)\Modloader\"
$PreZipDir = "$($ProjectDir)\PreZipDir"
$ZipDir = "$($ProjectDir)\ZipDir"


echo "Packing: $($ModName) - v$($Version)"

if($CookUnreal -eq $true){
    & "$($UnrealEngine)\Binaries\Win64\UE4Editor-Cmd.exe" "$($ProjectDir)\FactoryGame.uproject" -run=Cook  -TargetPlatform=WindowsNoEditor -fileopenlog -unversioned -abslog="$($UnrealEngine)\Programs\AutomationTool\Saved\Cook.txt" -stdout -CrashForUAT -unattended -NoLogTimes  -UTF8Output
}
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

if($PakOnly -eq $false){
    write-host "Copying Mod DLL to mods folder"
    Copy-Item -Path "$($CPPFolder)\x64\Release\$($ModName).dll" -Destination "$($GameDir)\FactoryGame\Binaries\Win64\mods\$($ModName).dll" | out-null
}


if((Test-Path "$($PreZipDir)") -eq $false){
    write-host "Creating Pre Zip Folder!"
    New-Item -ItemType Directory -Path "$($PreZipDir)" -Force | out-null
}

if($PakOnly -eq $false){
    Copy-Item -Path "$($CPPFolder)\x64\Release\$($ModName).dll" -Destination "$($PreZipDir)\$($ModName).dll"
}

Copy-Item -Path "$($GameDir)\FactoryGame\Content\Paks\$($ModName)_p.pak" -Destination "$($PreZipDir)\$($ModName)_p.pak"
Copy-Item -Path "$($GameDir)\FactoryGame\Content\Paks\$($ModName)_p.sig" -Destination "$($PreZipDir)\$($ModName)_p.sig"

echo $DataJson | set-content -Path "$($PreZipDir)\data.json"


if((Test-Path "$($ZipDir)") -eq $false){
    write-host "Creating Zip Folder!"
    New-Item -ItemType Directory -Path "$($ZipDir)" -Force | out-null
}
if($PakOnly -eq $false){
    $CompressPaths = @("$($PreZipDir)\$($ModName).dll", "$($PreZipDir)\$($ModName)_p.pak", "$($PreZipDir)\$($ModName)_p.sig","$($PreZipDir)\data.json")
}else{
    $CompressPaths = @("$($PreZipDir)\$($ModName)_p.pak", "$($PreZipDir)\$($ModName)_p.sig","$($PreZipDir)\data.json")
}

& "$($ProjectDir)\7z.exe" @("a", "-r", "$($ZipDir)\$($ModName)_$($Version).zip", "$($ProjectDir)\PreZipDir\*", "x=9") | out-null

[System.Media.SystemSounds]::Beep.Play()


if($RunGame -eq $true){
    & "$($GameDir)\FactoryGame\Binaries\Win64\FactoryGame-Win64-Shipping.exe"
}