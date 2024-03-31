// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"

/**
 * Logging class to log  messages to screen.
 * USAGE:
 * 
 #include "CPP_Log.h" // Header to include for logging function

CPP_Log::Log(CLog::INFO, "My Message to screen", 5); // "My Message to screen" output in green text for 5 seconds
CPP_Log::Log(CLog::WARN, "My Message to screen"); // Message output in yellow text for default 5 seconds duration
CPP_Log::Log(CLog::ERROR, "My Message to screen", 5); // Message output in red text

 */

enum class CLog //Logging types: INFO - Information, WARN - Warning, ERROR - Error, OTHER - Miscellaneous (default)
{
	INFO,
	WARN,
	ERROR,
	OTHER
};

class CTRLALTESCAPE_API CPP_Log
{
public:

	static void Log(CLog Type = CLog::OTHER, const FString& TEXT(Message) = TEXT("Log Message Not Set."), const float& Duration = 5.f);
	static void Log(CLog Type = CLog::OTHER, const FString& TEXT(Message) = TEXT("Log Message Not Set."), const bool& Value = false, const float& Duration = 5.f);
	//static void Log(CLog Type = CLog::OTHER, const FString& TEXT(Message) = TEXT("Log Message Not Set."), bool Value, const float& Duration = 5.f);
	//static void Log(CLog Type = CLog::OTHER, const FString& TEXT(Message) = TEXT("Log Message Not Set."), int Value, const float& Duration = 5.f);




	 CPP_Log();
	~CPP_Log();

};

// Possibly extend to Blueprints later 
/* Suspend development to look at later
// Very useful though not urgent
// Code notes follow...
	// Define enumerator
	UENUM(BlueprintType) // Expose to Blueprints
		enum class LogType : uint8
	{
		LogCrit     UMETA(DisplayName = "Critical"),
		LogDebug    UMETA(DisplayName = "Debug"),
		LogGen      UMETA(DisplayName = "General"),
		LogWarn     UMETA(DisplayName = "Warn")
	};
	

	//enum class logType { CRIT, DEBUG, GEN, WARN};
	void Log(uint8 LogType = 1, const char& Message);
	*/
