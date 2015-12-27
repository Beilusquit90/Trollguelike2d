// Fill out your copyright notice in the Description page of Project Settings.

#include "Trollguelike.h"
#include "Arrow.h"


// Sets default values
AArrow::AArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AArrow::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArrow::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AArrow::SetInfo(AMob*rhs, int dir, int r)
{
	if(rhs)
	Attacker = rhs;
	cx = Attacker->myBody.cx;
	cy = Attacker->myBody.cy;
	life = 1;
	tiktak = 0;
	direction = dir;
	range = r;
}

/*
AArrow::AArrow(Body*rhs, int dir,int r) :direction(dir), Attacker(rhs), range(r)
{
	cx = Attacker->cx;
	cy = Attacker->cy;
	life = 1;
	tiktak = 0;

		//dmg = (Attacker->ag / 2) + (Attacker->str / 4);


//	if (eff == fireball)
	//{
		//dmg = Attacker->intel;
		//if (Attacker->intel > 25)r = 7;	else r = 5;
	//}

}*/


/*
AArrow::AArrow(const AArrow&rhs)
{
	Attacker = rhs.Attacker;
	direction = rhs.direction;
	dmg = rhs.dmg;
	range = rhs.range;
	cx = rhs.cx, cy = rhs.cy;
	element = rhs.element;		//потом реализую, огонь вода ко ко ко.
	tiktak = rhs.tiktak;
	life = rhs.life;
}

AArrow& AArrow::operator=(const AArrow&rhs)
{
	
	Attacker = rhs.Attacker;
	direction = rhs.direction;
	dmg = rhs.dmg;
	range = rhs.range;
	cx = rhs.cx, cy = rhs.cy;
	element = rhs.element;		//потом реализую, огонь вода ко ко ко.
	tiktak = rhs.tiktak;
	life = rhs.life;
	return *this;
}
*/
AArrow::AArrow(AArrow&&rhs)
{
	Attacker = std::move(rhs.Attacker);
	direction = std::move(rhs.direction);
	dmg = std::move(rhs.dmg);
	range = std::move(rhs.range);
	cx = std::move(rhs.cx);
	cy = std::move(rhs.cy);
	element = std::move(rhs.element);		//потом реализую, огонь вода ко ко ко.
	tiktak = std::move(rhs.tiktak);
	life = std::move(rhs.life);
}

AArrow& AArrow::operator=(AArrow&&rhs)
{
	Attacker = std::move(rhs.Attacker);
	direction = std::move(rhs.direction);
	dmg = std::move(rhs.dmg);
	range = std::move(rhs.range);
	cx = std::move(rhs.cx);
	cy = std::move(rhs.cy);
	element = std::move(rhs.element);		//потом реализую, огонь вода ко ко ко.
	tiktak = std::move(rhs.tiktak);
	life = std::move(rhs.life);
	return *this;
}

