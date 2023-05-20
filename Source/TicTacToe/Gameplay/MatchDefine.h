// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MatchDefine.generated.h"


UENUM(BlueprintType)
enum class EChessBoardState : uint8
{
	BLACK = 0		UMETA(DisplayName = "黑棋"),
	EMPTY = 1		UMETA(DisplayName = "空白"),
	WHITE = 2		UMETA(DisplayName = "白棋"),
};

UENUM(BlueprintType)
enum class EChessBoardResult : uint8
{
	NONE		UMETA(DisplayName = "无结果"),
	BLACK_WIN	UMETA(DisplayName = "黑棋赢"),
	WHITE_WIN	UMETA(DisplayName = "白棋赢"),
	DRAW		UMETA(DisplayName = "平局"),
	ERROR		UMETA(DisplayName = "错误"),
};

UENUM(BlueprintType)
enum class EMatchState : uint8
{
	WAITING		UMETA(DisplayName = "等待开始"),
	STARING		UMETA(DisplayName = "开始中"),
	RUNNING		UMETA(DisplayName = "进行中"),
	PLAYER_WIN	UMETA(DisplayName = "玩家获胜"),
	PLAYER_LOST	UMETA(DisplayName = "玩家失败"),
	DRAW		UMETA(DisplayName = "平局"),
};

USTRUCT(BlueprintType)
struct TICTACTOE_API FOperatorCache
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	int32 PlaceIndex;
	
	UPROPERTY(BlueprintReadOnly)
	AActor* ChessActor;
};

/**
 * 
 */
UCLASS()
class TICTACTOE_API UMatchDefine : public UObject
{
	GENERATED_BODY()
};
