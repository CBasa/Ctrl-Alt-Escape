// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Log.h"


CPP_Log::CPP_Log()
{
    return;
}

CPP_Log::~CPP_Log()
{
	return;
}



void CPP_Log::Log(CLog Type, const FString& Message, const float& Duration)
{
    float LogDuration = Duration;
    FColor LogColor;
    FString LogString;

    switch (Type)
    {
    case CLog::INFO:
        LogColor = FColor::Green;
        LogString = FString::Printf(TEXT("CyberByte  INFO: %s"), *Message);
        break;
    case CLog::WARN:
        LogColor = FColor::Yellow;
        LogString = FString::Printf(TEXT("CyberByte  WARN: %s"), *Message);
        break;
    case CLog::ERROR:
        LogColor = FColor::Red;
        LogString = FString::Printf(TEXT("CyberByte ERROR: %s"), *Message);
        break;
    case CLog::OTHER:
    default:
        LogColor = FColor::Blue;
        LogString = FString::Printf(TEXT("CyberByte GENERAL: %s"), *Message);
        break;
        
    }

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, LogDuration, LogColor, LogString);
    }
}

// Log message with appended value
void CPP_Log::Log(CLog Type, const FString& Message, const bool& Value, const float& Duration)
{
        
    float LogDuration = Duration;
    FColor LogColor;
    FString LogString;

    switch (Type)
    {
    case CLog::INFO:
        LogColor = FColor::Green;
        LogString = FString::Printf(TEXT("CyberByte  INFO: %s"), *Message);
        LogString += FString::Printf(TEXT("%d"), Value);
            break;
    case CLog::WARN:
        LogColor = FColor::Yellow;
        LogString = FString::Printf(TEXT("CyberByte  WARN: %s"), *Message);
        LogString += FString::Printf(TEXT("%d"), Value);
        break;
    case CLog::ERROR:
        LogColor = FColor::Red;
        LogString = FString::Printf(TEXT("CyberByte ERROR: %s"), *Message);
        LogString += FString::Printf(TEXT("%d"), Value);
        break;
    case CLog::OTHER:
    default:
        LogColor = FColor::Blue;
        LogString = FString::Printf(TEXT("CyberByte GENERAL: %s"), *Message);
        LogString += FString::Printf(TEXT("%d"), Value);
        break;
    }
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, LogDuration, LogColor, LogString);
    }
}
