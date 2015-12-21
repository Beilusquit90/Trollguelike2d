// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Body.h"
#include "Mob.h"
#include "GameFramework/Actor.h"
#include "Arrow.generated.h"


UCLASS()
class TROLLGUELIKE_API AArrow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArrow();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	AArrow(Body* rhs, int dir,int r);

//	AArrow(const AArrow & rhs);
	//AArrow& operator=(const AArrow&rhs);

	AArrow(AArrow && rhs);
	AArrow & operator=(AArrow&&rhs);

	void SetInfo(AMob*rhs,int dir, int r);

	AMob* Attacker;
	int direction;

	int dmg;
	int range;
	int cx, cy;
	int element;		//потом реализую, огонь вода ко ко ко.
	double tiktak;
	int life;
	

};
