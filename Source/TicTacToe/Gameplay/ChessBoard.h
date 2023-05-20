// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MatchDefine.h"
#include "ChessBoard.generated.h"

UCLASS(Blueprintable, ClassGroup="GameplayActor")
class TICTACTOE_API AChessBoard : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AChessBoard();

	/**
	 * 预览当前位置棋子的放置
	 * @param WorldLocation 
	 */
	UFUNCTION(BlueprintCallable)
	void UpdateChessPreviewInChessBoardPosition(FVector WorldLocation);

	/**
	 * 放置棋子
	 * @param WorldLocation 
	 */
	UFUNCTION(BlueprintCallable)
	void PlaceChessInChessBoardPosition(FVector WorldLocation, bool bWhite);

	/**
	 * 放置棋子
	 */
	UFUNCTION(BlueprintCallable)
	bool CanPlaceChess(FVector WorldLocation);

	/**
	 * 查询当前胜负状态
	 * @return 
	 */
	UFUNCTION(BlueprintPure)
	EChessBoardResult GetCurrentState();

	/**
	 * 自动挑选最合适的步骤
	 */
	UFUNCTION(BlueprintCallable)
	void AutoMove(bool bIsWhite, int32 MaxDepth = 9);

	/**
	 * 随机放在某个空位上
	 */
	UFUNCTION(BlueprintCallable)
	void RandomMove(bool bIsWhite);

	/**
	 * 重置棋盘
	 */
	UFUNCTION(BlueprintCallable)
	void ResetChessBoard();

	/**
	 * 获取已经放置的棋子数量
	 * @return 
	 */
	UFUNCTION(BlueprintPure)
	uint8 GetPlacedChessAmount();

	/**
	 * 移除缓存中第一个放置的棋子
	 * @return 
	 */
	UFUNCTION(BlueprintCallable)
	void RemoveFirstCacheOperate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 棋盘
	UPROPERTY(BlueprintReadOnly)
	TArray<EChessBoardState> ChessBoard;

	// 棋子Actor放置位置
	UPROPERTY(EditAnywhere)
	TArray<FVector> ChessPosition;
	// 棋子预览Actor类型
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> PlaceHolderActorType;
	UPROPERTY()
	AActor* PlaceHolderActorInstance = nullptr;
	// 棋子类型
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ChessActorType_White;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ChessActorType_Black;
	// 棋子缓存
	UPROPERTY(BlueprintReadWrite)
	TArray<FOperatorCache> ChessCache;
	// 棋子spawn位置偏移，用于模拟物理效果
	UPROPERTY(EditAnywhere)
	FVector ChessSpawnOffset;

	/**
	 * 自动下棋步骤评分
	 * @param Depth 迭代深度
	 * @param bIsPlayer 当前是否为玩家操作
	 * @param bIsPlayerWhite 玩家是否是白棋
	 * @param BestMoveIndex 最佳放置位置（返回值）
	 * @param MaxDepth 控制AI智商 
	 */
	int32 MiniMax(int32 Depth, bool bIsPlayer, bool bIsPlayerWhite, int32& BestMoveIndex, int32 MaxDepth = 9);

	/**
	 * 内部State到Result转换方法
	 * @param State1 
	 * @param State2 
	 * @param State3 
	 * @return 
	 */
	inline EChessBoardResult InnerState2Result(EChessBoardState State1, EChessBoardState State2, EChessBoardState State3) const;

	/**
	 * 棋子坐标
	 * @param RelatedLocation 
	 * @return 
	 */
	int32 GetBestLocationInChessBoard(FVector RelatedLocation);
	
	/**
	 * 下棋操作
	 * @param Index 
	 * @param State 
	 */
	EChessBoardResult PlaceChessInIndex(int32 Index, EChessBoardState State);
	
};
