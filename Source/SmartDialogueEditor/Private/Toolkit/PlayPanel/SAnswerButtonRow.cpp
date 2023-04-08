// 

#include "SAnswerButtonRow.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"

void SAnswerButtonRow::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SButton)
		.ButtonStyle(FAppStyle::Get(), "FlatButton")
		.OnClicked_Lambda([InArgs]()
		{
			if (InArgs._OnSelectedAnswer.IsBound())
			{
				InArgs._OnSelectedAnswer.Execute(InArgs._AnswerIndex);
			}
			return FReply::Handled();
		})
		.ContentPadding(FMargin(8, 2))
		.HAlign(HAlign_Left)
		[
			SNew(STextBlock)
			.Text(InArgs._AnswerText)
			.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 18))
			.TextStyle(FAppStyle::Get(), "NormalText.Important")
		]
	];
}


