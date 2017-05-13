#include "Commons.h"
#include "TCF2GameInstance.h"

void UTCF2GameInstance::SetHolderInstance(UObject* holder)
{
	BlockHolder = holder;
	BlockHolder->AddToRoot();
}

void UTCF2GameInstance::DaytimeChanged(bool isNight, bool calledByLevelLoad)
{
	IsNightInGame = isNight;
	OnDaytimeChangedEvent.Broadcast(isNight, calledByLevelLoad);
}


void UTCF2GameInstance::AddErrorMessages(TArray<FText> messages) {
	LoadingErrorMessages.Append(messages);
}

bool UTCF2GameInstance::TakeErrorMessages(TArray<FText>& messages) {
	messages.Append(LoadingErrorMessages);
	auto res = LoadingErrorMessages.Num() > 0;
	ClearErrorMessages();
	return res;
}

void UTCF2GameInstance::ClearErrorMessages() {
	LoadingErrorMessages.Empty();
}


void UTCF2GameInstance::AddWarningMessages(TArray<FText> messages) {
	LoadingWarningMessages.Append(messages);
}

bool UTCF2GameInstance::TakeWarningMessages(TArray<FText>& messages) {
	messages.Append(LoadingWarningMessages);
	auto res = LoadingWarningMessages.Num() > 0;
	ClearWarningMessages();
	return res;
}

void UTCF2GameInstance::ClearWarningMessages() {
	LoadingWarningMessages.Empty();
}