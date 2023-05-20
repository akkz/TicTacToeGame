﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Chess.h"


// Sets default values
AChess::AChess()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("Root"));
	SetRootComponent(StaticMeshComponent);
}

// Called when the game starts or when spawned
void AChess::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AChess::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

