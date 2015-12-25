// Fill out your copyright notice in the Description page of Project Settings.

#include "Trollguelike.h"
#include "Shambala.h"


// Sets default values
AShambala::AShambala()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> MyMobBP(TEXT("Blueprint'/Game/MyMob.MyMob'"));
	static ConstructorHelpers::FObjectFinder<UBlueprint> RuibesMapBP(TEXT("Blueprint'/Game/NewFolder/MyRuinesMap.MyRuinesMap'"));
	if (MyMobBP.Object != NULL)
	{
		MyMob = (UClass*)MyMobBP.Object->GeneratedClass;
	}
	if (RuibesMapBP.Object != NULL)
	{
		BPRuinesMap = (UClass*)RuibesMapBP.Object->GeneratedClass;
		flag = 0;
	}

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShambala::BeginPlay()
{
	SetMyMap();
	SetMyHero();
	Super::BeginPlay();
	
}

// Called every frame
void AShambala::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	switch (TheSeed[flag]->CanActiv())
	{
	case 0: break;
	case 1:TheSeed[flag]->Activ(); break;
	case 2: break;
	default: break;
	}
}

void AShambala::SetMyMap()
{
	UWorld* const World = GetWorld();
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Owner = this;
	if (World)
	{
				FVector position(0, 0, 0);
				FRotator rotator(0, 0, 0);
				TheSeed.push_back(GetWorld()->SpawnActor<ARuinesMap>(BPRuinesMap, position, rotator, SpawnInfo));
		SetMyHero();
	}
}

void AShambala::SetMyHero()
{
	UWorld* const World = GetWorld();
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Owner = this;
	int cx, 
		int cy;
	TheSeed[flag]->CreatMyHero(cx, cy);
	if (World)
	{
		int rad = 64;
		FVector position(cx*rad, cy*rad, 0);
		FRotator rotator(0, 0, 0);
		int lvl = 1;
		int role=9;
		MyLovelyHero = (GetWorld()->SpawnActor<AMob>(MyMob, position, rotator, SpawnInfo));
		MyLovelyHero->myBody = (Body(role, cx, cy, 1));
		MyLovelyHero->myBody.role = role;
	}
}

void AShambala::Restart()
{
	for (auto const&z : TheSeed)
	{
		z->Destroy();
	}
	TheSeed.clear();
	SetMyMap();
	SetMyHero();
	//TheSeed.push_back(RuinesMap());
	flag = 0;
	//TheSeed[flag].tempTransx



	MyLovelyHero->Destroy();
	SetMyHero();
	//glTranslatef((TheSeed[flag].tempTransx), (TheSeed[flag].tempTransy), 0);

	//glTranslatef(-(MyLovelyHero.cy * 50 - 350), -(MyLovelyHero.cx * 50 - 350), 0);
}


int AShambala::Activ()
{
	{
		if (TheSeed[flag].ioflag == 1)
		{
			if (int(TheSeed.size()) > (flag + 1))
			{
				TheSeed[flag].ioflag = 0;
				flag++;
				TheSeed[flag].PushUp(MyLovelyHero);
				MyLovelyHero.tiktak = 0;
				return 1;
			}
			else
			{
				TheSeed[flag].ioflag = 0;
				flag++;
				TheSeed.push_back(RuinesMap(flag + 1));
				TheSeed[flag].PushUp(MyLovelyHero);
				return 1;
			}
		}
		if (TheSeed[flag].ioflag == 2)
		{
			TheSeed[flag].ioflag = 0;
			flag--;
			TheSeed[flag].PushDown(MyLovelyHero);
			MyLovelyHero.tiktak = 0;
			return 1;
		}
	}
	TheSeed[flag].WhoDie();
	if (MyLovelyHero.hp <= 0) {
		Restart(); flagMenu = 1; return 0;
	}
	if (MyLovelyHero.tiktak <= 0)
	{
		HeroSteps();
		return 0;
	}
	else
		MyLovelyHero.tiktak -= 0.2;
	TheSeed[flag].Activ();
	return 0;
}


void AShambala::HeroSteps()
{
	static int fhp;
	if (fhp != MyLovelyHero.hp)(TheSeed[flag].steps.clear());
	fhp = MyLovelyHero.hp;

	if (TheSeed[flag].steps.size() == 0)
	{
		int temp = 0;
		if (flags == 1) { temp = TheSeed[flag].Move(MyLovelyHero.cx, MyLovelyHero.cy + 1, &MyLovelyHero); flags = 0; }
		if (flags == 2) { temp = TheSeed[flag].Move(MyLovelyHero.cx + 1, MyLovelyHero.cy, &MyLovelyHero); flags = 0; }
		if (flags == 3) { temp = TheSeed[flag].Move(MyLovelyHero.cx, MyLovelyHero.cy - 1, &MyLovelyHero); flags = 0; }
		if (flags == 4) { temp = TheSeed[flag].Move(MyLovelyHero.cx - 1, MyLovelyHero.cy, &MyLovelyHero); flags = 0; }

	}
	else
	{
		TheSeed[flag].Step(); flags = 0;
	}
}