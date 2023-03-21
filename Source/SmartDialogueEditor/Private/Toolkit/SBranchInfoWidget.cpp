// SBranchInfoWidget.cpp

#include "SBranchInfoWidget.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "EditorStyle/Public/EditorStyleSet.h"

void SBranchInfoWidget::Construct(const FArguments& InArgs)
{
	Branch = InArgs._Branch;
	Editor = InArgs._Editor;

	ChildSlot
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SBox)
			.WidthOverride(150)
			[
				SNew(SEditableTextBox)
				.Text(FText::FromString(Branch.Name))
				// ...
			]
		]
		+ SHorizontalBox::Slot()
		.FillWidth(1.0f)
		[
			SNew(SEditableTextBox)
			.Text(FText::FromString(Branch.Text))
			// ...
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SButton)
			.ButtonStyle(FEditorStyle::Get(), "FlatButton")
			.ContentPadding(FMargin(1.0f, 1.0f, 1.0f, 1.0f))
			[
				SNew(SImage)
				.Image(FEditorStyle::GetBrush("Cross"))
			]
			// ...
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SButton)
			.ButtonStyle(FEditorStyle::Get(), "FlatButton")
			.ContentPadding(FMargin(1.0f, 1.0f, 1.0f, 1.0f))
			[
				SNew(SImage)
				.Image(FEditorStyle::GetBrush("Cross"))
			]
			// ...
		]
	];
}