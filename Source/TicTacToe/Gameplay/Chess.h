// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chess.generated.h"

UCLASS()
class TICTACTOE_API AChess : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AChess();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Chess颜色
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn=true))
	FColor ChessColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StaticMeshComponent;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
