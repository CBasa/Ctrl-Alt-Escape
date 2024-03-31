// Fill out your copyright notice in the Description page of Project Settings.
/* Not required as Stealth Blueprint function is on widget, and calls GameInstance for values! */
/*

#include "CPP_StealthBarWidget_KR.h"

void UCPP_StealthBarWidget_KR::NativeConstruct()
{
	Super::NativeConstruct();

	Player = Cast<ACtrlAltEscapeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));


}

float UCPP_StealthBarWidget_KR::CalculateStealthPercentage()
{
	if (Player)
	{
		return Player->GetCurrentHealth() / Player->GetMaxHealth();
	}
	return 0.0f;
}


*/