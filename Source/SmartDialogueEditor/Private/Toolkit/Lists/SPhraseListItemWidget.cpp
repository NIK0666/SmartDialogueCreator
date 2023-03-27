// SPhraseListItemWidget.cpp


#include "SPhraseListItemWidget.h"

#include "EditorStyleSet.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"

void SPhraseListItemWidget::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SBox)
		.WidthOverride(800.0f)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.FillWidth(1.0f)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.Padding(5.0f)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					[
						SNew(SButton)
						.Text_Lambda([]() { return FText::FromString(TEXT("КнопкаПерсонажа v")); })
					]
					+ SHorizontalBox::Slot()
					[
						SNew(SButton)
						.Text_Lambda([]() { return FText::FromString(TEXT("КнопкаАнимации v")); })
					]
					+ SHorizontalBox::Slot()
					[
						SNew(SButton)
						.Text_Lambda([]() { return FText::FromString(TEXT("КнопкаПеременной")); })
					]
					+ SHorizontalBox::Slot()
					[
						SNew(SCheckBox)
                            .Style(FCoreStyle::Get(), "ToggleButtonCheckbox")
                            .ToolTipText(FText::FromString(TEXT("Или?")))
					]
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Right)
					[
						SNew(SBox)
                            .WidthOverride(90.0f)
                            .HAlign(HAlign_Right)
						[
							SNew(SHorizontalBox)
							+ SHorizontalBox::Slot()
							[
								SNew(SButton)
                                    .ButtonStyle(FEditorStyle::Get(), "FlatButton.Default")
                                    .ToolTipText(FText::FromString(TEXT("Настройки")))
                                    .OnClicked_Lambda([]() { return FReply::Handled(); })
                                    .Content()
								[
									SNew(SImage)
									.Image(FEditorStyle::GetBrush("SettingsEditor.Settings"))
								]
							]
							+ SHorizontalBox::Slot()
							[
								SNew(SButton)
                                    .ButtonStyle(FEditorStyle::Get(), "FlatButton.Default")
                                    .ToolTipText(FText::FromString(TEXT("Перетаскивание")))
                                    .OnClicked_Lambda([]() { return FReply::Handled(); })
                                    .Content()
								[
									SNew(SImage)
									.Image(FEditorStyle::GetBrush("Icons.Grabber"))
								]
							]
							+ SHorizontalBox::Slot()
							[
								SNew(SButton)
                                    .ButtonStyle(FEditorStyle::Get(), "FlatButton.Default")
                                    .ToolTipText(FText::FromString(TEXT("Удаление")))
                                    .OnClicked_Lambda([]() { return FReply::Handled(); })
                                    .Content()
								[
									SNew(SImage)
									.Image(FEditorStyle::GetBrush("Icons.Delete"))
								]
							]
						]
					]
				]
				+ SVerticalBox::Slot()
				.Padding(5.0f)
				[
					SNew(SMultiLineEditableTextBox)
						.AutoWrapText(true)
						// .MinDesiredWidth(500.0f)
						// .MinDesiredHeight(40.0f)
				]
			]
		]
	];
}
