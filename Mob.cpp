// Fill out your copyright notice in the Description page of Project Settings.

#include "Trollguelike.h"
#include "Mob.h"
#include "Engine.h"


// Sets default values
/*
AMob::AMob()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Constructor"));
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}
*/
/*
AMob::~AMob()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Destructor"));
}
*/
/*
AMob::AMob(AMob&rhs) {
	myBody = rhs.myBody;
}


AMob& AMob::operator=(const AMob*rhs)
{
	myBody = *rhs.myBody;
	return *this;
}
*/
// Called when the game starts or when spawned
void AMob::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMob::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

