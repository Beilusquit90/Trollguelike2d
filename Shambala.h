// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <vector>
#include <RuinesMap.h>
#include <Mob.h>
#include "GameFramework/Actor.h"
#include "Shambala.generated.h"

UCLASS()
class TROLLGUELIKE_API AShambala : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShambala();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
	void SetMyMap();
	void SetMyHero();

	int Activ();
	void Restart();

	void HeroSteps();
//	void MousePress(int button, int  state, int x, int y);
	//void sKeyboard(int keyx, int x, int y);
	//void Keyboard(unsigned char keyx, int x, int y);

private:

	std::vector<ARuinesMap*>TheSeed;
	int flag;

	AMob* MyLovelyHero;
	
	TSubclassOf<class AMob>MyMob;
	TSubclassOf<class ARuinesMap>BPRuinesMap;
};

extern int flags;