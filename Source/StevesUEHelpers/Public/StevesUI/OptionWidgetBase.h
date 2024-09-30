#pragma once

#include "CoreMinimal.h"
#include "FocusableUserWidget.h"
#include "Styling/SlateTypes.h"
#include "StevesHelperCommon.h"
#include "OptionWidgetBase.generated.h"

class UTextBlock;
class UImage;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSelectedOptionChanged, class UOptionWidgetBase*, Widget, int, NewIndex);

UCLASS(Abstract, BlueprintType)
class STEVESUEHELPERS_API UOptionWidgetBase : public UFocusableUserWidget
{
    GENERATED_BODY()


public:

    // -- Properties automatically bound to Blueprint widget
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget), Category = UI)
    UWidget* MouseVersion;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget), Category = UI)
    UButton* MouseUpButton;    

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget), Category = UI)
    UButton* MouseDownButton;    

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget), Category = UI)
    UImage* MouseUpImage;    

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget), Category = UI)
    UImage* MouseDownImage;    

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget), Category = UI)
    UTextBlock* MouseText;    

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget), Category = UI)
    UButton* GamepadVersion;    

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget), Category = UI)
    UImage* GamepadUpImage;    

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget), Category = UI)
    UImage* GamepadDownImage;    

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget), Category = UI)
    UTextBlock* GamepadText;

    /// Event raised when the selected option changes
    UPROPERTY(BlueprintAssignable)
    FOnSelectedOptionChanged OnSelectedOptionChanged;

    UFUNCTION(BlueprintCallable, Category = UI)
    /// Remove all options
    virtual void ClearOptions();

    /**
     * @brief Adds a new option
     * @param Option The text for the new option
     * @return The index for the new option
     */
    UFUNCTION(BlueprintCallable, Category = UI)
    virtual int AddOption(FText Option);

    
    /**
     * @brief Sets all of the options available for this control
     * @param Options All options to be available
     * @param NewSelectedIndex Which of the options to select by default
     */
    UFUNCTION(BlueprintCallable, Category = UI)
    virtual void SetOptions(const TArray<FText>& Options, int NewSelectedIndex = 0);
    
    UFUNCTION(BlueprintPure, Category = UI)
    virtual int GetSelectedIndex() const { return SelectedIndex; }

    UFUNCTION(BlueprintPure, Category = UI)
    virtual FText GetSelectedOption() const;
    /**
     * @brief Change the selected index option
     * @param NewIndex The new index to set, can be -1 for no selection
     */
    UFUNCTION(BlueprintCallable, Category = UI)
    virtual void SetSelectedIndex(int NewIndex);

    virtual void SetFocusProperly_Implementation() override;


protected:

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Content)
    TArray<FText> Options;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Content)
    int SelectedIndex;

    UFUNCTION(BlueprintCallable, Category = UI)
    virtual void SetMouseMode();
    UFUNCTION(BlueprintCallable, Category = UI)
    virtual void SetButtonMode();

    UFUNCTION(BlueprintCallable, Category = UI)
    virtual void UpdateFromInputMode(EInputMode Mode);
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;
    UFUNCTION(BlueprintCallable, Category = UI)
    virtual void ChangeOption(int Delta);
    UFUNCTION(BlueprintCallable, Category = UI)
    virtual EInputMode GetCurrentInputMode() const;
	virtual void UpdateUpDownButtons();


protected:
    UFUNCTION()
    void InputModeChanged(int PlayerIndex, EInputMode NewMode);
    UFUNCTION()
    void MouseUpClicked();
    UFUNCTION()
    void MouseDownClicked();
};
