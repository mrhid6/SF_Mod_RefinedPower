## Introduction:
Ficsit pioneers can now have refined power production that can provide a variety of power generators either to power your outposts or power your main factories.

## Installation:
**Version 2.0.14 (Refined Power)**
1. Download and install **SMM** (**Satisfactory Mod Manager/The Launcher**)
2. Download the latest version of **Refined Power**
3. Unlock the Schematics in the HUB and enjoy.

## Development
If you want to help develop this mod you will need to do the following steps:

### Setup:
* Download the SML Repository (SML Project)
* Download the CSS Unreal Engine: [HERE](https://github.com/SatisfactoryModdingUE/UnrealEngine/releases)
* Visual Studio 2017 needs to be installed with the packages `Game Development for c++` and `Desktop Development for c++`. This installs the buildtools which are essential in not crashing the game.
* Intergrate Wwise into the SML project
* Download Refined Power Repository
* Copy the Refined Power Repository into the SML Project
* Edit FactoryGame.uproject and replace `ExampleMod` with `RefinedPower`
* Right click on FactoryGame.uproject and click `Generate Visual Studio Project Files`
* Open FactoryGame.sln in Visual Studio
* Compile as Development Editor Win64
* Open FactoryGame.uproject.
