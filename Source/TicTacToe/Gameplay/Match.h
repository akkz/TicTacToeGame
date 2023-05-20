// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ChessBoard.h"
#include "CoreMinimal.h"
#include "MatchDefine.h"
#include "GameFramework/Actor.h"
#include "Match.generated.h"

UCLASS()
class TICTACTOE_API AMatch : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMatch();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 当前是否为玩家回合
	UPROPERTY(BlueprintReadOnly)
	bool bPlayerRound = true;

	// 玩家棋子是否为白色
	UPROPERTY(BlueprintReadOnly)
	bool bPlayerWhite = true;

	// 是否为竞速模式
	UPROPERTY(BlueprintReadOnly)
	bool bFastMode = false;

	// 回合时间，x轴代表游戏时间， y轴代表回合时间
	UPROPERTY(EditAnywhere)
	UCurveFloat* RoundTimeConfig;
	FTimerHandle RoundTimerHandle;

	// 当前回合时间
	UPROPERTY(BlueprintReadOnly)
	float CurrentRoundTime = 0;
	UPROPERTY(BlueprintReadOnly)
	float CurrentRoundStartTime = 0;

	// 棋盘缓存
	UPROPERTY(BlueprintReadOnly)
	AChessBoard* ChessBoard;

	// 比赛状态缓存
	UPROPERTY(BlueprintReadOnly)
	EMatchState MatchState = EMatchState::WAITING;

	// 比赛开始动画播放时间
	UPROPERTY(EditAnywhere)
	float LoadingTime = 0;
	
	/**
	 * 蓝图处理比赛准备开始
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnMatchReady();

	/**
	 * 蓝图处理比赛开始的逻辑
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnMatchStart();

	/**
	 * 蓝图处理比赛结束的逻辑
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnMatchFinished();

public:

	/**
	 * 当前是否玩家操作回合
	 * @return 
	 */
	UFUNCTION(BlueprintPure)
	bool IsPlayerRound();

	UFUNCTION(BlueprintCallable)
	void StartMatch(bool bPlayerFirst, bool bInFastMode = false);

	UFUNCTION(BlueprintCallable)
	void ResetMatch();

	UFUNCTION(BlueprintCallable)
	void SwitchRound();

	UFUNCTION(BlueprintPure)
	bool IsMatchRunning();
};
