#include "StevesUI/OptionWidgetBase.h"

#include "StevesUI.h"
#include "StevesGameSubsystem.h"
#include "StevesUEHelpers.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UOptionWidgetBase::NativeConstruct()
{
    Super::NativeConstruct();

    auto GS = GetStevesGameSubsystem(GetWorld());
    if (GS)
    {
        GS->OnInputModeChanged.AddDynamic(this, &UOptionWidgetBase::InputModeChanged);
        UpdateFromInputMode(GS->GetLastInputModeUsed());
        ClearOptions();
    }
    else
        UE_LOG(LogStevesUI, Error, TEXT("StevesGameSubsystem is missing!"));

    // Check we can bind everything we need, bind click
    if (!MouseVersion)
        UE_LOG(LogStevesUI, Error, TEXT("%s should have a MouseVersion instance."), *this->GetClass()->GetName());
    if (!GamepadVersion)
        UE_LOG(LogStevesUI, Error, TEXT("%s should have a GamepadVersion instance."), *this->GetClass()->GetName());
    if (MouseUpButton)
        MouseUpButton->OnClicked.AddDynamic(this, &UOptionWidgetBase::MouseUpClicked);
    else        
        UE_LOG(LogStevesUI, Error, TEXT("%s should have a MouseUpButton instance."), *this->GetClass()->GetName());
    if (MouseDownButton)
        MouseDownButton->OnClicked.AddDynamic(this, &UOptionWidgetBase::MouseDownClicked);
    else
        UE_LOG(LogStevesUI, Error, TEXT("%s should have a MouseDownButton instance."), *this->GetClass()->GetName());
    if (!MouseUpImage)
        UE_LOG(LogStevesUI, Error, TEXT("%s should have a MouseUpImage instance."), *this->GetClass()->GetName());
    if (!MouseDownImage)
        UE_LOG(LogStevesUI, Error, TEXT("%s should have a MouseDownImage instance."), *this->GetClass()->GetName());
    if (!GamepadUpImage)
        UE_LOG(LogStevesUI, Error, TEXT("%s should have a GamepadUpImage instance."), *this->GetClass()->GetName());
    if (!GamepadDownImage)
        UE_LOG(LogStevesUI, Error, TEXT("%s should have a GamepadDownImage instance."), *this->GetClass()->GetName());

    
}


void UOptionWidgetBase::NativeDestruct()
{
    auto GS = GetStevesGameSubsystem(GetWorld());
    if (GS)
    {
        GS->OnInputModeChanged.RemoveAll(this);
    }

    if (MouseUpButton)
        MouseUpButton->OnClicked.RemoveAll(this);
    if (MouseDownButton)
        MouseDownButton->OnClicked.RemoveAll(this);
    
}

void UOptionWidgetBase::ChangeOption(int Delta)
{
    SetSelectedIndex(FMath::Clamp(SelectedIndex + Delta, 0, Options.Num() - 1));    
}


void UOptionWidgetBase::MouseUpClicked()
{
    ChangeOption(1);
}

void UOptionWidgetBase::MouseDownClicked()
{
    ChangeOption(-1);
}

void UOptionWidgetBase::ClearOptions()
{
    Options.Empty();
    SetSelectedIndex(-1);
}

void UOptionWidgetBase::UpdateFromInputMode(EInputMode Mode)
{
    switch (Mode)
    {
    case EInputMode::Mouse:
        SetMouseMode();
        break;
    case EInputMode::Keyboard:
    case EInputMode::Gamepad:
        SetButtonMode();
        break;
    case EInputMode::Unknown:
    default:
        break;
    }
}

void UOptionWidgetBase::InputModeChanged(int PlayerIndex, EInputMode NewMode)
{
    UpdateFromInputMode(NewMode);
}

void UOptionWidgetBase::SynchronizeProperties()
{
    Super::SynchronizeProperties();

    SyncButtonProperties(MouseUpButton);
    SyncButtonProperties(MouseDownButton);
    SyncButtonProperties(GamepadVersion);
    
    SyncTextProperties(MouseText);
    SyncTextProperties(GamepadText);

    SyncButtonImageProperties(MouseUpImage);
    SyncButtonImageProperties(MouseDownImage);
    SyncButtonImageProperties(GamepadUpImage);
    SyncButtonImageProperties(GamepadDownImage);
}

void UOptionWidgetBase::SyncButtonProperties(UButton* Button) const
{
    if (Button)
    {
        Button->SetStyle(ButtonStyle);
    }
}

void UOptionWidgetBase::SyncButtonImageProperties(UImage* Img) const
{
    if (Img)
    {
        Img->SetBrush(ButtonImage);
        Img->SetColorAndOpacity(ButtonImageColour);
    }
}

void UOptionWidgetBase::SyncTextProperties(UTextBlock* Txt) const
{
    if (Txt)
    {
        Txt->SetFont(Font);
        Txt->SetColorAndOpacity(TextColour);
        Txt->SetStrikeBrush(TextStrikeBrush);
        Txt->SetShadowOffset(TextShadowOffset);
        Txt->SetShadowColorAndOpacity(TextShadowColor);
    }
}


void UOptionWidgetBase::SetMouseMode()
{
    if (!MouseVersion)
        return;
    
    bool bHadFocus = false;
    if (GamepadVersion)
    {
        bHadFocus = GamepadVersion->HasKeyboardFocus();
        GamepadVersion->SetVisibility(ESlateVisibility::Hidden);
    }

    MouseVersion->SetVisibility(ESlateVisibility::Visible);
    if (bHadFocus)
        SetFocusProperly();
    
}

void UOptionWidgetBase::SetButtonMode()
{
    if (!GamepadVersion)
        return;
    const bool bHadFocus = (MouseUpButton && MouseUpButton->HasKeyboardFocus()) || (MouseDownButton && MouseDownButton->HasKeyboardFocus());

    if (MouseVersion)
        MouseVersion->SetVisibility(ESlateVisibility::Hidden);

    GamepadVersion->SetVisibility(ESlateVisibility::Visible);
    if (bHadFocus)
        SetFocusProperly();
    
}

void UOptionWidgetBase::SetFocusProperly_Implementation()
{
    if (GamepadVersion && GamepadVersion->IsVisible())
        GamepadVersion->SetFocus();
    else if (MouseUpButton && MouseDownButton)
        MouseUpButton->GetIsEnabled() ? MouseUpButton->SetFocus() : MouseDownButton->SetFocus();
}

void UOptionWidgetBase::SetSelectedIndex(int NewIndex)
{
    const bool bRaiseEvent = SelectedIndex != NewIndex;
    
    SelectedIndex = NewIndex;

    const FText NewText = GetSelectedOption();
    if (MouseText)
        MouseText->SetText(NewText);
    if (GamepadText)
        GamepadText->SetText(NewText);

    const bool CanDecrease = SelectedIndex > 0;
    const bool CanIncrease = SelectedIndex < Options.Num() - 1;
    if (MouseDownButton)
        MouseDownButton->SetIsEnabled(CanDecrease);
    if (MouseUpButton)
        MouseUpButton->SetIsEnabled(CanIncrease);
    if (GamepadDownImage)
        GamepadDownImage->SetVisibility(CanDecrease ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    if (GamepadUpImage)
        GamepadUpImage->SetVisibility(CanIncrease ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

    if (bRaiseEvent)
        OnSelectedOptionChanged.Broadcast(this, SelectedIndex);
    
}

int UOptionWidgetBase::AddOption(FText Option)
{
    return Options.Add(Option);
}

void UOptionWidgetBase::SetOptions(const TArray<FText>& InOptions, int NewSelectedIndex)
{
    Options = InOptions;
    SetSelectedIndex(NewSelectedIndex);
}

FText UOptionWidgetBase::GetSelectedOption() const
{
    if (Options.IsValidIndex(SelectedIndex))
        return Options[SelectedIndex];

    return FText();
}

EInputMode UOptionWidgetBase::GetCurrentInputMode() const
{
    auto GS = GetStevesGameSubsystem(GetWorld());
    if (GS)
        return GS->GetLastInputModeUsed();

    return EInputMode::Unknown;
}

