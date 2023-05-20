// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessBoard.h"

#include "Chess.h"
#include "MatchDefine.h"
#include "TicTacToe/TicTacToe.h"

#define CHESSBOARD_WIDTH 3
#define CHESSBOARD_INDEX(X,Y) X+Y*CHESSBOARD_WIDTH

// Sets default values
AChessBoard::AChessBoard()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AChessBoard::UpdateChessPreviewInChessBoardPosition(FVector WorldLocation)
{
	FVector RelatedLocation = GetActorTransform().InverseTransformPosition(WorldLocation);

	// 寻找合适的位置
	int32 Index = GetBestLocationInChessBoard(RelatedLocation);
	if (Index < 0)
	{
		return;
	}
	FVector Location = GetActorTransform().TransformPosition(ChessPosition[Index]);

	// 放置棋子
	if (!IsValid(PlaceHolderActorInstance) && IsValid(PlaceHolderActorType))
	{
		PlaceHolderActorInstance = GetWorld()->SpawnActor<AActor>(PlaceHolderActorType, Location, GetActorRotation());
	}
	if (IsValid(PlaceHolderActorInstance))
	{
		PlaceHolderActorInstance->SetActorHiddenInGame(false);
		PlaceHolderActorInstance->SetActorLocation(Location);
	}
}

void AChessBoard::PlaceChessInChessBoardPosition(FVector WorldLocation, bool bWhite)
{
	FVector RelatedLocation = GetActorTransform().InverseTransformPosition(WorldLocation);

	// 寻找合适的位置
	int32 Index = GetBestLocationInChessBoard(RelatedLocation);
	if (Index < 0)
	{
		return;
	}
	
	PlaceChessInIndex(Index, bWhite ? EChessBoardState::WHITE : EChessBoardState::BLACK);
}

bool AChessBoard::CanPlaceChess(FVector WorldLocation)
{
	FVector RelatedLocation = GetActorTransform().InverseTransformPosition(WorldLocation);

	// 寻找合适的位置
	int32 Index = GetBestLocationInChessBoard(RelatedLocation);
	if (Index < 0)
	{
		return false;
	}

	return ChessBoard[Index] == EChessBoardState::EMPTY;
}

EChessBoardResult AChessBoard::GetCurrentState()
{
	// 检查胜负
	// 检查行列
	EChessBoardResult Result = EChessBoardResult::NONE;
	for (int i = 0; i < CHESSBOARD_WIDTH; ++i)
	{
		Result = InnerState2Result(ChessBoard[CHESSBOARD_INDEX(i, 0)], ChessBoard[CHESSBOARD_INDEX(i, 1)], ChessBoard[CHESSBOARD_INDEX(i, 2)]);
		if (Result != EChessBoardResult::NONE) return Result;
		Result = InnerState2Result(ChessBoard[CHESSBOARD_INDEX(0, i)], ChessBoard[CHESSBOARD_INDEX(1, i)], ChessBoard[CHESSBOARD_INDEX(2, i)]);
		if (Result != EChessBoardResult::NONE) return Result;
	}
	
	// 检查交叉
	Result = InnerState2Result(ChessBoard[CHESSBOARD_INDEX(0, 0)], ChessBoard[CHESSBOARD_INDEX(1, 1)], ChessBoard[CHESSBOARD_INDEX(2, 2)]);
	if (Result != EChessBoardResult::NONE) return Result;
	Result = InnerState2Result(ChessBoard[CHESSBOARD_INDEX(0, 2)], ChessBoard[CHESSBOARD_INDEX(1, 1)], ChessBoard[CHESSBOARD_INDEX(2, 0)]);
	if (Result != EChessBoardResult::NONE) return Result;

	// 检查游戏是否结束
	bool bEnd = true;
	for (EChessBoardState State : ChessBoard)
	{
		if (State == EChessBoardState::EMPTY)
		{
			bEnd = false;
			break;
		}
	}
	if (bEnd) return EChessBoardResult::DRAW;

	return EChessBoardResult::NONE;
}

// Called when the game starts or when spawned
void AChessBoard::BeginPlay()
{
	Super::BeginPlay();

	ResetChessBoard();
}

void AChessBoard::AutoMove(bool bIsWhite, int32 MaxDepth)
{
	int32 BestMove = -1;
	MiniMax(0, bIsWhite, bIsWhite, BestMove, MaxDepth);

	if (BestMove != -1)
	{
		PlaceChessInIndex(BestMove, bIsWhite ? EChessBoardState::WHITE : EChessBoardState::BLACK);
	}
	else
	{
		RandomMove(bIsWhite);
	}
}

void AChessBoard::RandomMove(bool bIsWhite)
{
	TArray<int32> EmptySlot;
	
	for (int i = 0; i < CHESSBOARD_WIDTH; ++i)
	{
		for (int j = 0; j < CHESSBOARD_WIDTH; ++j)
		{
			int32 Index = i + j * CHESSBOARD_WIDTH;

			// 如果可以下棋，那么缓存
			if (ChessBoard[Index] == EChessBoardState::EMPTY)
			{
				EmptySlot.Add(Index);
			}
		}
	}

	int32 RandIndex = EmptySlot[FMath::RandRange(0, EmptySlot.Num()-1)];
	PlaceChessInIndex(RandIndex, bIsWhite ? EChessBoardState::WHITE : EChessBoardState::BLACK);

	return;
}

void AChessBoard::ResetChessBoard()
{
	ChessBoard.Reset(CHESSBOARD_WIDTH * CHESSBOARD_WIDTH);
	for (int i = CHESSBOARD_WIDTH * CHESSBOARD_WIDTH; i > 0; i--)
	{
		ChessBoard.Add(EChessBoardState::EMPTY);
	}
	
	for (FOperatorCache& Cache : ChessCache)
	{
		if (IsValid(Cache.ChessActor))
		{
			Cache.ChessActor->Destroy();
		}
	}
	ChessCache.Reset();
}

uint8 AChessBoard::GetPlacedChessAmount()
{
	return ChessCache.Num();
}

void AChessBoard::RemoveFirstCacheOperate()
{
	if (ChessCache.Num() == 0)
	{
		UE_LOG(LogTicTacToe, Warning, TEXT("[ChessBoard::RemoveFirstCacheOperate] Fail to remove first operate in cache! No cache data."))
		return;
	}

	FOperatorCache OperatorCache = ChessCache[0];
	ChessCache.RemoveAt(0);

	if (IsValid(OperatorCache.ChessActor))
	{
		OperatorCache.ChessActor->Destroy();
	}
	if (OperatorCache.PlaceIndex >= 0 && OperatorCache.PlaceIndex < ChessBoard.Num())
	{
		ChessBoard[OperatorCache.PlaceIndex] = EChessBoardState::EMPTY;
	}
}

int32 AChessBoard::MiniMax(int32 Depth, bool bCurrentWhite, bool bIsWhite, int32& BestMoveIndex, int32 MaxDepth)
{
	// 胜负检查
	EChessBoardResult ChessBoardResult = GetCurrentState();
	if (ChessBoardResult == EChessBoardResult::BLACK_WIN || ChessBoardResult == EChessBoardResult::WHITE_WIN)
	{
		if (bCurrentWhite != bIsWhite)
		{
			return 10 - Depth;
		}
		else
		{
			return Depth - 10;
		}
	}

	if (ChessBoardResult == EChessBoardResult::DRAW)
	{
		return 0;
	}

	// 迭代深度检查
	if (Depth >= MaxDepth)
	{
		// 想不出来的情况按照最坏情况处理
		return -10;
	}

	int32 BestScore = bCurrentWhite == bIsWhite ? -10 : 10;

	for (int i = 0; i < CHESSBOARD_WIDTH; ++i)
	{
		for (int j = 0; j < CHESSBOARD_WIDTH; ++j)
		{
			int32 Index = i + j * CHESSBOARD_WIDTH;

			// 如果可以下棋，那么尝试走一步
			if (ChessBoard[Index] == EChessBoardState::EMPTY)
			{
				ChessBoard[Index] = bIsWhite ? EChessBoardState::WHITE : EChessBoardState::BLACK;
				int32 CurrentScore = MiniMax(Depth + 1, bCurrentWhite, !bIsWhite, BestMoveIndex, MaxDepth);
				
				if (bCurrentWhite == bIsWhite)
				{
					// 己方行为，取最高分
					if (CurrentScore > BestScore)
					{
						BestScore = CurrentScore;

						// Depth为0时，缓存最佳己方的步骤
						if (Depth == 0)
						{
							BestMoveIndex = Index;
						}
					}
				}
				else
				{
					// 对手行为，取最低分
					if (CurrentScore < BestScore)
					{
						BestScore = CurrentScore;
					}
				}

				ChessBoard[Index] = EChessBoardState::EMPTY;
			}
		}
	}

	return BestScore;
}

EChessBoardResult AChessBoard::InnerState2Result(EChessBoardState State1, EChessBoardState State2, EChessBoardState State3) const
{
	uint8 State = static_cast<uint8>(State1) + static_cast<uint8>(State2) + static_cast<uint8>(State3);

	if (State == 6)
	{
		return EChessBoardResult::WHITE_WIN;
	}

	if (State == 0)
	{
		return EChessBoardResult::BLACK_WIN;
	}

	return EChessBoardResult::NONE;
}

int32 AChessBoard::GetBestLocationInChessBoard(FVector RelatedLocation)
{
	float MinDistance = UE_MAX_FLT;
	int32 Index = -1;
	for (int i = 0; i < ChessPosition.Num(); ++i)
	{
		FVector& Position = ChessPosition[i];
		
		float SquaredLength = (RelatedLocation - Position).SquaredLength();
		if (MinDistance > SquaredLength)
		{
			MinDistance = SquaredLength;
			Index = i;
		}
	}

	return Index;
}

EChessBoardResult AChessBoard::PlaceChessInIndex(int32 Index, EChessBoardState State)
{
	// 状态更新
	if (ChessBoard[Index] != EChessBoardState::EMPTY)
	{
		UE_LOG(LogTicTacToe, Warning, TEXT("[MatchSubsystem::UpdateChessBoardState] Chess board state has been set while update chess board state. Index = %d, state %d"), Index, State);
		return EChessBoardResult::ERROR;
	}
	ChessBoard[Index] = State;

	// 隐藏PlaceHolder
	if (IsValid(PlaceHolderActorInstance))
	{
		PlaceHolderActorInstance->SetActorHiddenInGame(true);
	}

	// 棋子放置
	FVector Location = GetActorTransform().TransformPosition(ChessPosition[Index]) + ChessSpawnOffset;

	TSubclassOf<AActor> ChessActorType = State == EChessBoardState::WHITE ? ChessActorType_White : ChessActorType_Black;
	if (IsValid(ChessActorType))
	{
		AChess* Chess = GetWorld()->SpawnActor<AChess>(ChessActorType, Location, GetActorRotation());

		// 缓存操作
		FOperatorCache OperatorCache;
		OperatorCache.ChessActor = Chess;
		OperatorCache.PlaceIndex = Index;
		ChessCache.Add(OperatorCache);
	}

	return GetCurrentState();
}

