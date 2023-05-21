// Fill out your copyright notice in the Description page of Project Settings.


#include "Match.h"

#include "ChessBoard.h"
#include "Kismet/GameplayStatics.h"
#include "TicTacToe/TicTacToe.h"


// Sets default values
AMatch::AMatch()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AMatch::BeginPlay()
{
	Super::BeginPlay();

	BP_OnMatchReady();
}

bool AMatch::IsPlayerRound()
{
	return bPlayerRound && IsMatchRunning();
}

void AMatch::StartMatch(bool bPlayerFirst, bool bInFastMode)
{
	UWorld* World = GetWorld();
	check(World);

	MatchStartTime = World->GetTimeSeconds();
	
	ChessBoard = Cast<AChessBoard>(UGameplayStatics::GetActorOfClass(this, AChessBoard::StaticClass()));
	check(ChessBoard);
	ChessBoard->ResetChessBoard();
	
	bPlayerRound = bPlayerFirst;
	bPlayerWhite = bPlayerFirst;

	bFastMode = bInFastMode;

	MatchState = EMatchState::STARING;
	
	BP_OnMatchStart();

	if (LoadingTime == 0)
	{
		MatchState = EMatchState::RUNNING;
		StartRound();
	}
	else
	{
		FTimerHandle LoadingTimerHandle;
		World->GetTimerManager().SetTimer(LoadingTimerHandle, FTimerDelegate::CreateWeakLambda(this, [this]()
		{
			MatchState = EMatchState::RUNNING;
			StartRound();
		}), LoadingTime, false);
	}
}

void AMatch::ResetMatch()
{
	bPlayerRound = false;
	bPlayerWhite = false;
	bFastMode = false;
	MatchState = EMatchState::WAITING;

	if (IsValid(ChessBoard))
	{
		ChessBoard->ResetChessBoard();
	}

	BP_OnMatchReady();
}

void AMatch::StartRound()
{
	UWorld* World = GetWorld();
	check(World);

	check(ChessBoard);

	// Fast模式玩法
	if (bFastMode)
	{
		// 清理旧的Timer
		World->GetTimerManager().ClearTimer(RoundTimerHandle);
		
		uint8 PlacedChessAmount = ChessBoard->GetPlacedChessAmount();
		if (PlacedChessAmount == 7)
		{
			// 移除第一次放置的棋子
			ChessBoard->RemoveFirstCacheOperate();
			ChessBoard->RemoveFirstCacheOperate();
		}

		// 更新时间
		CurrentRoundStartTime = World->GetTimeSeconds() - MatchStartTime;
		CurrentRoundTime = RoundTimeConfig ? RoundTimeConfig->GetFloatValue(CurrentRoundStartTime) : -1;

		// 创建新Timer
		if (CurrentRoundTime > 0)
		{
			World->GetTimerManager().SetTimer(RoundTimerHandle, FTimerDelegate::CreateWeakLambda(this, [this]()
			{
				ChessBoard->RandomMove(!(bPlayerRound ^ bPlayerWhite));
			
				SwitchRound();
			}), CurrentRoundTime, false);
		}
	}
}

void AMatch::SwitchRound()
{
	UWorld* World = GetWorld();
	check(World);
	
	// 检查游戏是否结束了
	EChessBoardResult CurrentChessBoardResult = ChessBoard->GetCurrentState();
	if (CurrentChessBoardResult == EChessBoardResult::BLACK_WIN || CurrentChessBoardResult == EChessBoardResult::WHITE_WIN || CurrentChessBoardResult == EChessBoardResult::DRAW)
	{
		// 比赛状态更新
		if (CurrentChessBoardResult == EChessBoardResult::DRAW)
		{
			// 平局检查
			MatchState = EMatchState::DRAW;
		}
		else
		{
			// 玩家胜负检查
			MatchState = (bPlayerWhite ^ (CurrentChessBoardResult == EChessBoardResult::WHITE_WIN)) ? EMatchState::PLAYER_LOST : EMatchState::PLAYER_WIN;
		}

		// 清理旧的Timer
		World->GetTimerManager().ClearTimer(RoundTimerHandle);

		BP_OnMatchFinished();
		
		// 如果已经结束，则直接返回，不处理了
		return;
	}

	BP_OnSwitchRound();
	
	// 更新轮次
	bPlayerRound = !bPlayerRound;

	StartRound();
}

bool AMatch::IsMatchRunning()
{
	return MatchState == EMatchState::RUNNING;
}

float AMatch::GetRoundTime()
{
	UWorld* World = GetWorld();
	check(World);

	return World->GetTimeSeconds() - MatchStartTime - CurrentRoundStartTime;
}

