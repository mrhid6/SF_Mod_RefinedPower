#include "AlpakitWidget.h"
#include "TextBoxListItem.h"
#include "Developer/DesktopPlatform/Public/IDesktopPlatform.h"
#include "Developer/DesktopPlatform/Public/DesktopPlatformModule.h"
#include "Editor/UATHelper/Public/IUATHelperModule.h"

void SAlpakaWidget::Construct(const FArguments& InArgs)
{
	Settings = GetMutableDefault<UAlpakitSettings>();
	for (FString mod : Settings->Mods)
	{
		TSharedPtr<FEditableItem> item = MakeShareable(new FEditableItem());
		item.Get()->Text = mod;
		Items.Add(item);
	}
	ChildSlot
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
		//~~~~~~~~~~~~~~~~~~~~
		//  Being Slate Construct Block
		[
			SNew(SVerticalBox)
			+	SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+	SHorizontalBox::Slot()
						.Padding(3.0f)
						.AutoWidth()
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Fill)
						[
							SNew(STextBlock)
							.Text(FText::FromString("Satisfactory Game Folder"))
						]
					+	SHorizontalBox::Slot()
						.Padding(3.0f)
						.FillWidth(1.0f)
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Fill)
						[
							SNew(SEditableTextBox)
							.Text_Lambda([this]() { return Settings->SatisfactoryGamePath; })
							.OnTextChanged_Lambda([this](const FText& InText) { Settings->SatisfactoryGamePath = InText; Settings->SaveConfig(); })
						]
					+	SHorizontalBox::Slot()
						.Padding(3.0f)
						.AutoWidth()
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Fill)
						[
							SNew(SButton)
							.VAlign(VAlign_Center)
							.HAlign(HAlign_Center)
							.Text(FText::FromString("Choose folder"))
							.OnClicked_Raw(this, &SAlpakaWidget::ChooseSatisfactoryFolder)
						]
				]	
			+	SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+	SHorizontalBox::Slot()
						.Padding(3.0f)
						.AutoWidth()
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Fill)
						[
							SNew(STextBlock)
							.Text(FText::FromString("Start game after paking"))
						]
					+	SHorizontalBox::Slot()
						.Padding(3.0f)
						.FillWidth(1.0f)
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Fill)
						[
							SNew(SCheckBox)
							.IsChecked_Lambda([this]() {return Settings->StartGame ? ECheckBoxState::Checked : ECheckBoxState::Unchecked; })
							.OnCheckStateChanged_Lambda([this](ECheckBoxState state) { Settings->StartGame = (state == ECheckBoxState::Checked); Settings->SaveConfig(); })
						]
				]	
			+	SVerticalBox::Slot()
				.FillHeight(1.0f)
				[
					SNew(SScrollBox)
					+ SScrollBox::Slot()
						[
							SNew(SButton)
							.Text(FText::FromString("Add mod"))
							.OnClicked_Raw(this, &SAlpakaWidget::AddMod)
						]

					//The actual list view creation
					+ SScrollBox::Slot()
						[
							SAssignNew(ListViewWidget, SListView<TSharedPtr<FEditableItem>>)
							.ItemHeight(24)
							.SelectionMode(ESelectionMode::None)
							.ListItemsSource(&Items) //The Items array is the source of this listview
							.OnGenerateRow(this, &SAlpakaWidget::OnGenerateRowForList)
						]
				]
			+ SVerticalBox::Slot()
				.AutoHeight()
				.HAlign(HAlign_Right)
				[
					SNew(SHorizontalBox)
					+	SHorizontalBox::Slot()
						.Padding(3.0f)
						.FillWidth(1.0f)
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Fill)
						[
							SAssignNew(SaveSettingsButton, SButton)
							.VAlign(VAlign_Center)
							.HAlign(HAlign_Center)
							.Text(FText::FromString("Save Settings"))
							.OnClicked_Raw(this, &SAlpakaWidget::SaveSettings)
						]
					+	SHorizontalBox::Slot()
						.Padding(3.0f)
						.AutoWidth()
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Fill)
						[
							SAssignNew(AlpakitButton, SButton)
							.VAlign(VAlign_Center)
							.HAlign(HAlign_Center)
							.Text(FText::FromString("Alpakit!"))
							.OnClicked_Raw(this, &SAlpakaWidget::Pakit)
						]
				]
		]; //End Slate Construct Block
		//~~~~~~~~~~~~~~~~~~~~~~~~

}

FReply SAlpakaWidget::AddMod()
{
	//Adds a new item to the array (do whatever you want with this)
	TSharedPtr<FEditableItem> item = MakeShareable(new FEditableItem());
	item.Get()->Text = "ModName";
	Items.Add(item);
	//Update the listview
	ListViewWidget->RequestListRefresh();

	return FReply::Handled();
}


TSharedRef<ITableRow> SAlpakaWidget::OnGenerateRowForList(TSharedPtr<FEditableItem> Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	//Create the row
	return
		SNew(STableRow< TSharedPtr<FEditableItem> >, OwnerTable)
		.Padding(2.0f)
		[
			SNew(SHorizontalBox)
			+	SHorizontalBox::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Center)
				.FillWidth(1.0f)
				[
					SNew(STextboxListItem)
					.Item(Item)
				]
			+	SHorizontalBox::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.AutoWidth()
				.Padding(2.0f)
				[
					SNew(SButton)
					.Text(FText::FromString("Delete"))
					.OnClicked_Lambda([this, Item]() { Items.Remove(Item); return FReply::Handled(); })
				]
		];
}

TSharedRef<SWidget> SAlpakaWidget::AsWidget()
{
	return SharedThis(this);
}

FReply SAlpakaWidget::SaveSettings()
{
	Settings->Mods.Empty();
	for (TSharedPtr<FEditableItem> item : Items)
		Settings->Mods.Add(item->Text);
	Settings->SaveConfig();
	return FReply::Handled();
}

FReply SAlpakaWidget::Pakit()
{
	SaveSettings();
	AlpakitButton.Get()->SetEnabled(false);
	CookContent();
	return FReply::Handled();
}

FReply SAlpakaWidget::ChooseSatisfactoryFolder()
{
	FString satisfactoryPath;
	if (OpenDirectoryDialog(FString("Choose Satisfactory install folder"), Settings->SatisfactoryGamePath.ToString(), satisfactoryPath))
	{
		Settings->SatisfactoryGamePath = FText::FromString(satisfactoryPath);
		Settings->SaveConfig();
	}
	return FReply::Handled();
}

bool SAlpakaWidget::OpenFileDialog(const FString& DialogTitle, const FString& DefaultPath, const FString& FileTypes, TArray<FString>& OutFileNames)
{
	TSharedPtr<SWindow> ParentWindow = FSlateApplication::Get().FindWidgetWindow(AsShared());
	const void* ParentWindowHandle = (ParentWindow.IsValid() && ParentWindow->GetNativeWindow().IsValid())
		? ParentWindow->GetNativeWindow()->GetOSWindowHandle()
		: nullptr;
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (DesktopPlatform)
	{
		//Opening the file picker!
		uint32 SelectionFlag = 0; //A value of 0 represents single file selection while a value of 1 represents multiple file selection
		return DesktopPlatform->OpenFileDialog(ParentWindowHandle, DialogTitle, DefaultPath, FString(""), FileTypes, SelectionFlag, OutFileNames);
	}
	return false;
}

bool SAlpakaWidget::OpenDirectoryDialog(const FString& DialogTitle, const FString& DefaultPath, FString& OutFolderName)
{
	TSharedPtr<SWindow> ParentWindow = FSlateApplication::Get().FindWidgetWindow(AsShared());
	const void* ParentWindowHandle = (ParentWindow.IsValid() && ParentWindow->GetNativeWindow().IsValid())
		? ParentWindow->GetNativeWindow()->GetOSWindowHandle()
		: nullptr;
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (DesktopPlatform)
	{
		//Opening the file picker!
		return DesktopPlatform->OpenDirectoryDialog(ParentWindowHandle, DialogTitle, DefaultPath, OutFolderName);
	}
	return false;
}

void SAlpakaWidget::CookDone(FString result, double runtime)
{
	if (result.Equals("completed", ESearchCase::IgnoreCase))
	{
		// Cooking was successful
		TArray<FString> mods;
		for (TSharedPtr<FEditableItem> ModItem : Items)
		{
			mods.Add(ModItem->Text);
		}

		FString CmdExe = TEXT("cmd.exe");
		FString UPakPath = FPaths::ConvertRelativePathToFull(FPaths::EngineDir() / TEXT("Binaries/Win64/UnrealPak.exe"));
		FString PakListPath = FPaths::ConvertRelativePathToFull(FPaths::EngineDir() / TEXT("Programs/AutomationTool/Saved/Logs") / FString::Printf(TEXT("PakList_%s-WindowsNoEditor.txt"), FApp::GetProjectName()));

		// Get list of all cooked assets
		TArray<FString> FilesToPak;
		FFileHelper::LoadFileToStringArray(FilesToPak, *PakListPath);

		for (FString mod : mods)
		{
			// Choose from the cooked list only the current mod assets
			TArray<FString> ModFilesToPak;
			FString modCookFolder = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() / FString::Printf(TEXT("Saved/Cooked/WindowsNoEditor/%s/Content/FactoryGame/%s"), FApp::GetProjectName(), *mod)).Replace(L"/", L"\\");
			UE_LOG(LogTemp, Log, TEXT("%s"), *modCookFolder);
			for (FString file : FilesToPak)
				if (file.TrimQuotes().StartsWith(modCookFolder))
					ModFilesToPak.Add(file);

			// Save it for UnrealPak.exe
			FString ModPakListPath = FPaths::ConvertRelativePathToFull(FPaths::EngineDir() / TEXT("Programs/AutomationTool/Saved/Logs") / FString::Printf(TEXT("PakList_%s-WindowsNoEditor.txt"), *mod));
			FFileHelper::SaveStringArrayToFile(ModFilesToPak, *ModPakListPath);
			
			// Setup the pak file path
			IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
			FString modPakFolder = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() / L"Mods");
			if (!PlatformFile.DirectoryExists(*modPakFolder))
				PlatformFile.CreateDirectory(*modPakFolder);
			FString pakFilePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() / L"Mods" / FString::Printf(L"%s.pak", *mod));
			
			// Run the paker and wait
			FString FullCommandLine = FString::Printf(TEXT("/c \"\"%s\" %s\""), *UPakPath, *FString::Printf(TEXT("%s -create=\"%s\""), *pakFilePath, *ModPakListPath));
			TSharedPtr<FMonitoredProcess> PakingProcess = MakeShareable(new FMonitoredProcess(CmdExe, FullCommandLine, true));
			PakingProcess->OnOutput().BindLambda([this, mod](FString output) { UE_LOG(LogTemp, Log, TEXT("Paking %s: %s"), *mod, *output); });
			PakingProcess->Launch();

			UE_LOG(LogTemp, Log, TEXT("Packing %s"), *mod);
			while (PakingProcess->Update())
				FPlatformProcess::Sleep(0.03);

			// Copy to Satisfactory Content/Paks folder
			PlatformFile.CopyFile(*FPaths::ConvertRelativePathToFull(Settings->SatisfactoryGamePath.ToString() / FString::Printf(L"FactoryGame/Content/Paks/%s.pak", *mod)), *pakFilePath);

			// Copy sig
			PlatformFile.CopyFile(*FPaths::ConvertRelativePathToFull(Settings->SatisfactoryGamePath.ToString() / FString::Printf(L"FactoryGame/Content/Paks/%s.sig", *mod)), *FPaths::ConvertRelativePathToFull(Settings->SatisfactoryGamePath.ToString() / L"FactoryGame/Content/Paks/FactoryGame-WindowsNoEditor.sig"));

			UE_LOG(LogTemp, Log, TEXT("Packed %s"), *mod);
		}
		AlpakitButton.Get()->SetEnabled(true);
		if (Settings->StartGame)
		{
			FString gamePath = FString::Printf(TEXT("\"%s\""), *FPaths::ConvertRelativePathToFull(Settings->SatisfactoryGamePath.ToString() / L"FactoryGame/Binaries/Win64/FactoryGame-Win64-Shipping.exe").Replace(L"/", L"\\"));
			int pathLength = gamePath.Len();
			char SatisfactoryPath[WINDOWS_MAX_PATH + 5];
			for (int i = 0; i < pathLength; i++)
			{
				SatisfactoryPath[i] = gamePath[i];
				SatisfactoryPath[i + 1] = '\000';
			}
			system(SatisfactoryPath);
		}
	}
	else
		UE_LOG(LogTemp, Error, TEXT("Error while running Aplakit. Cooking returned: %s"), *result);
}

// https://answers.unrealengine.com/questions/500324/how-create-folder-when-my-game-is-running.html
//If this function cannot find or create the directory, returns false.
static FORCEINLINE bool VerifyOrCreateDirectory(const FString& TestDir)
{
	// Every function call, unless the function is inline, adds a small
	// overhead which we can avoid by creating a local variable like so.
	// But beware of making every function inline!
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	// Directory Exists?
	if (!PlatformFile.DirectoryExists(*TestDir))
	{
		PlatformFile.CreateDirectory(*TestDir);

		if (!PlatformFile.DirectoryExists(*TestDir))
		{
			return false;
			//~~~~~~~~~~~~~~
		}
	}
	return true;
}

void SAlpakaWidget::CookContent()
{
	// Create WwiseAudio folder if it doesn't exist
	VerifyOrCreateDirectory(FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir()) / TEXT("WwiseAudio"));

	// Run the cook
	FString ProjectPath = FPaths::IsProjectFilePathSet() ? FPaths::ConvertRelativePathToFull(FPaths::GetProjectFilePath()) : FPaths::RootDir() / FApp::GetProjectName() / FApp::GetProjectName() + TEXT(".uproject");
	FString CommandLine = FString::Printf(TEXT("BuildCookRun -nop4 -project=\"%s\" -cook -package -pak -skipstage -iterate"), *ProjectPath);

	IUATHelperModule::Get().CreateUatTask(CommandLine, FText::FromString("Windows"), FText::FromString("Cooking content"), FText::FromString("Cooking"), nullptr, [this](FString result, double runtime) { CookDone(result, runtime); });
}