// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Body.h"
#include "GameFramework/Actor.h"
#include "Mob.generated.h"

UCLASS()
class TROLLGUELIKE_API AMob : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	//AMob();
	//AMob(AMob&rhs);
	//AMob& operator=(AMob*rhs);
	//~AMob();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	Body myBody;
	
};
