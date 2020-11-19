#pragma once

#include "CoreMinimal.h"


#include "FocusableInputInterceptorUserWidget.h"
#include "Framework/Application/IInputProcessor.h"
#include "StevesHelperCommon.h"


#include "MenuStack.generated.h"

class UContentWidget;
class UMenuBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMenuStackClosed, class UMenuStack*, Stack, bool, bWasCancel);

/// Represents a modal stack of menus which take focus and have a concept of "Back"
/// Each level within is a MenuBase, which must be "pushed" on to the stack.
/// Contained within MenuSystem (multiple menu stacks supported)
/// Has a Priority so that when multiple menu stacks are open, higher priority gets focus,
/// and when closed, next highest priority gets focus back. Focus is given when the first MenuBase is pushed onto the stack,
/// and given up when the last one is popped.
/// You can style this widget to be the general surrounds in which all MenuBase levels live inside
/// Create a Blueprint subclass of this and make sure you include a UContentWidget with the name
/// "MenuContainer" somewhere in the tree, which is where the menu contents will be placed. 
UCLASS(Abstract, BlueprintType)
class STEVESUEHELPERS_API UMenuStack : public UFocusableInputInterceptorUserWidget
{
    GENERATED_BODY()

protected:
    EInputMode LastInputMode;
    EInputModeChange PreviousInputMode;
    EMousePointerVisibilityChange PreviousMouseVisibility;
    EGamePauseChange PreviousPauseState;
    
    TArray<UMenuBase*> Menus;

    virtual void FirstMenuOpened();
    virtual void LastMenuClosed(bool bWasCancel);

    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    void SavePreviousInputMousePauseState();
    virtual void ApplyInputModeChange(EInputModeChange Change) const;
    virtual void ApplyMousePointerVisibility(EMousePointerVisibilityChange Change) const;
    virtual void ApplyGamePauseChange(EGamePauseChange Change) const;

    virtual bool HandleKeyDownEvent(const FKeyEvent& InKeyEvent) override;
    UFUNCTION()
    void InputModeChanged(int PlayerIndex, EInputMode NewMode);

public:
    /// The focus priority of this stack compared to others. When a MenuStack is opened, if it has higher priority than
    /// any existing MenuStack open, it will be given focus. When a MenuStack with focus is closed, the next highest
    /// priority one will be given focus. 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Behaviour")
    int FocusPriority = 0;

    /// This property will bind to a blueprint variable of the same name to contain the actual menu content
    /// If not set, or the UiMenuBase is set to not use this container, levels are added independently to viewport
    /// Use a NamedSlot for this most of the time, it gives you the most layout flexibility.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
    UContentWidget* MenuContainer;

    /// Event raised when the stack is closed for any reason. If bWasCancel, the menu stack was closed because the
    /// last item was cancelled.
    UPROPERTY(BlueprintAssignable)
    FOnMenuStackClosed OnClosed;

    /// How this stack should change the input mode when it opens (default no change)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Behavior")
    EInputModeChange InputModeSettingOnOpen = EInputModeChange::DoNotChange;

    /// How this stack should set the mouse pointer visibility when it opens (default no change)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Behavior")
    EMousePointerVisibilityChange MousePointerVisibilityOnOpen = EMousePointerVisibilityChange::DoNotChange;

    /// How this stack should set the game pause state when it opens (default no change)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Behavior")
    EGamePauseChange GamePauseSettingOnOpen = EGamePauseChange::DoNotChange;

    /// How this stack should change the input mode when it closes (default no change)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Behavior")
    EInputModeChange InputModeSettingOnClose = EInputModeChange::DoNotChange;

    /// How this stack should set the mouse pointer visibility when it closes (default no change)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Behavior")
    EMousePointerVisibilityChange MousePointerVisibilityOnClose = EMousePointerVisibilityChange::DoNotChange;

    /// How this stack should set the game pause state when it closes (default no change)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Behavior")
    EGamePauseChange GamePauseSettingOnClose = EGamePauseChange::DoNotChange;

    /// Push a new menu level by class. This will instantiate the new menu, display it, and inform the previous menu that it's
    /// been superceded. Use the returned instance if you want to cache it
    UFUNCTION(BlueprintCallable)
    UMenuBase* PushMenuByClass(TSubclassOf<UMenuBase> MenuClass);

    /// Push a new menu level by instance on to the stack. This will display the new menu and inform the previous menu that it's
    /// been superceded, which will most likely mean it will be hidden (but will retain its state)
    UFUNCTION(BlueprintCallable)
    void PushMenuByObject(UMenuBase* NewMenu);

    /// Pop the top level of the menu stack. This *destroys* the top level menu, meaning it will lose all of its state.
    /// You won't need to call this manually most of the time, because calling Close() on the MenuBase will do it.
    UFUNCTION(BlueprintCallable)
    void PopMenu(bool bWasCancel);

    /// Get the number of active levels in the menu
    UFUNCTION(BlueprintCallable)
    int Count() const { return Menus.Num(); }

    /// Close the entire stack at once. This does not give any of the menus chance to do anything before close, so if you
    /// want them to do that, use PopMenu() until Count() == 0 instead
    UFUNCTION(BlueprintCallable)
    void CloseAll(bool bWasCancel);

    /// Whether the top MenuBase on this stack is requesting focus
    UFUNCTION(BlueprintCallable)
    bool IsRequestingFocus() const;
    
    /// Triggers this stack to take focus (specifically its topmost MenuBase) if appropriate
    /// This means if both top menu on the stack requests focus, and gamepad or keyboard is in use
    UFUNCTION(BlueprintCallable)
    void TakeFocusIfDesired();
    
    virtual void SetFocusProperly_Implementation() override;
    virtual void PopMenuIfTop(UMenuBase* UiMenuBase, bool bWasCancel);
    virtual void RemoveFromParent() override;
};
