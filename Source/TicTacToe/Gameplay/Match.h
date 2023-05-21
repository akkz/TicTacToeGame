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

	// 回合时间，x轴代表游戏时间， y轴代表回合时间
	UPROPERTY(EditAnywhere)
	UCurveFloat* RoundTimeConfig;
	FTimerHandle RoundTimerHandle;

	// 比赛开始动画播放时间
	UPROPERTY(EditAnywhere)
	float LoadingTime = 0;

	

	// 当前是否为玩家回合
	UPROPERTY(BlueprintReadOnly)
	bool bPlayerRound = true;

	// 玩家棋子是否为白色
	UPROPERTY(BlueprintReadOnly)
	bool bPlayerWhite = true;

	// 是否为竞速模式
	UPROPERTY(BlueprintReadOnly)
	bool bFastMode = false;
	
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

	// 比赛开始时间
	UPROPERTY(BlueprintReadOnly)
	float MatchStartTime;
	
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

	/**
	 * 蓝图处理比赛轮次的逻辑
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnSwitchRound();

public:

	/**
	 * 当前是否玩家操作回合
	 * @return 
	 */
	UFUNCTION(BlueprintPure)
	bool IsPlayerRound();

	/**
	 * 开始比赛
	 * @param bPlayerFirst 
	 * @param bInFastMode 
	 */
	UFUNCTION(BlueprintCallable)
	void StartMatch(bool bPlayerFirst, bool bInFastMode = false);

	/**
	 * 重置比赛
	 */
	UFUNCTION(BlueprintCallable)
	void ResetMatch();

	/**
	 * 首次开局轮次
	 */
	UFUNCTION(BlueprintCallable)
	void StartRound();

	/**
	 * 切换轮次
	 */
	UFUNCTION(BlueprintCallable)
	void SwitchRound();

	UFUNCTION(BlueprintPure)
	bool IsMatchRunning();

	UFUNCTION(BlueprintPure)
	float GetRoundTime();
};
