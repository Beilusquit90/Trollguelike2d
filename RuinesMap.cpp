// Fill out your copyright notice in the Description page of Project Settings.

#include "Trollguelike.h"
#include "RuinesMap.h"
#include "Body.h"
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"
#include <queue>
#include <Engine.h>

// Sets default values
ARuinesMap::ARuinesMap()
{
	
	static ConstructorHelpers::FObjectFinder<UBlueprint> MyWallBP(TEXT("Blueprint'/Game/NewFolder/NewBlueprint.NewBlueprint'"));
	static ConstructorHelpers::FObjectFinder<UBlueprint> MyMobBP(TEXT("Blueprint'/Game/MyMob.MyMob'"));
	

	if (MyWallBP.Object != NULL)
	{
		BpWall = (UClass*)MyWallBP.Object->GeneratedClass;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("MyWallBP не равен нулю"));
	}
	if (MyMobBP.Object != NULL)
	{
		MyMob = (UClass*)MyMobBP.Object->GeneratedClass;
	}

	restartFlag = 0;
	tempTransx = 0;
	tempTransy = 0;


	mflag = 0;
	ioflag = 0;
	lvl = 1;
	for (int i = 0; i < sizeMap; i++)
		for (int j = 0; j < sizeMap; j++)
		{
			levelSize[i][j] = 0;
			MA[i][j] = 0;
		}
	CreateLvl();
		int temp = (rand() % 6) + 5;
	for (int i = 0; i < temp; i++)
	{
	NewMapMan();
	}

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.



	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Конец конструктора"));
	PrimaryActorTick.bCanEverTick = true;
}


void ARuinesMap::NewMapMan()
{

	int i = 1, cx, cy, role=1;
	for (;;)
	{
		cx = FMath::RandHelper(sizeMap);
		cy = FMath::RandHelper(sizeMap);
		role = FMath::RandHelper(3)+1;
		if (levelSize[cx][cy] == 0)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Some variable values: X: %i, Y: %i"), cx, cy));
		
			levelSize[cx][cy] = role;
			break;
		}
	}
}

void ARuinesMap::CreatMyHero()
{
	int i = 1, cx, cy, role=9;
	for (;;)
	{
		cx = FMath::RandHelper(sizeMap);
		cy = FMath::RandHelper(sizeMap);
		if (levelSize[cx][cy] == 0)
		{
		//	vBody.push_back(Body(role, cx, cy, lvl));
			//MyHero=&vBody.back();
			levelSize[cx][cy] = 9;
			break;
		}
	}
}


// Called when the game starts or when spawned
void ARuinesMap::BeginPlay()
{
	SpawnSprites();
	Super::BeginPlay();
}

// Called every frame
void ARuinesMap::Tick(float DeltaTime)
{
	static bool flag = 0;
	
	//FPlatformProcess::Sleep(1000);
	if(vMob.size())
		test(flag);
	Super::Tick(DeltaTime);
}

void ARuinesMap::test(bool &flag)
{
	
	for (auto const&z : vMob)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Some variable values: X: %i, Y: %i"), z->myBody.cx, z->myBody.cy));
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Some variable values: X: %i, Y: %i"), z->myBody->cx, z->myBody->cy));
		rMove(z); flag = 1;
		
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Some variable values: I: %i"), i));
	}
}

void ARuinesMap::CreateLvl()
{
	for (int x = 0; x < sizeMap; x++)
		for (int y = 0; y < sizeMap; y++)
			levelSize[x][y] = 999;

	MapGen();

	for (int x = 0; x < sizeMap; x++)
		for (int y = 0; y < sizeMap; y++)
			if (levelSize[x][y] != 999)
				levelSize[x][y] =0;
	//std::cout << "MAP GEN FINISH" << std::endl;
}

void ARuinesMap::MapGen()
{
	//std::cout << "MAP GEN" << std::endl;
	int x, y;
	int temp = rand() % 20 + 27;	// рандомно выбираем количество комнат.

	for (int count = 0; count < 10000 && temp != 0; count++)
	{
		int flagm = 0;
		int rSizex = rand() % 12 + 7; // размер по оси икс и игрик, делаем от 2 до 8.
		int rSizey = rand() % 12 + 7;

		x = rand() % (sizeMap - 1);			//выбираем рандомную точку на карте, для создания комнаты.
		y = rand() % (sizeMap - 1);
		if (x != (sizeMap - 2))x++;
		if (y != (sizeMap - 2))y++;

		int tempxpp;				//будет хранить наибольную координату икс
		int tempxmm;				//будет хранить наименьшую координату икс
		int tempypp;				//будет хранить наибольную координату игрик
		int tempymm;				//будет хранить наименьшую координату игрик
		if ((x - (rSizex / 2)) <= 0) tempxmm = 1;	// если комната хочет выйти за предел, обрезаем её.
		else tempxmm = (x - (rSizex / 2));			//если нет, то выдаем ей значение 
		if ((x + (rSizex / 2)) >= sizeMap - 1) tempxpp = sizeMap - 2;
		else tempxpp = (x + (rSizex / 2));
		if ((y - (rSizey / 2)) <= 0) tempymm = 1;
		else tempymm = y - (rSizex / 2);
		if ((y + (rSizey / 2)) >= sizeMap) tempypp = sizeMap - 2;
		else tempypp = y + (rSizex / 2);


		if (count != 0)
		{
			for (int i = tempxmm; i < tempxpp; i++)
				for (int j = tempymm; j < tempypp; j++)
					if (levelSize[i][j] != 999) { flagm = 1; break; }
		}

		if (flagm == 0)
		{
			for (int i = tempxmm; i < tempxpp; i++)
				for (int j = tempymm; j < tempypp; j++)
				{
					levelSize[i][j] = 0;
				}


			int flagxy = 0;
			for (int i = tempxmm - 1; i < tempxpp + 1; i++)
				for (int j = tempymm - 1; j < tempypp + 1; j++)
				{
					if (i == (tempxmm - 1) || i == (tempxpp + 1))
						if (levelSize[i][j] != 999)
							flagxy++;

					if (j == (tempymm - 1) || j == (tempypp + 1))
						if (levelSize[i][j] != 999)
							flagxy++;

				}

			if (flagxy == 0)
			{
				RL.push_back(Point(x, y));
			}
			temp--;
		}
	}

	for (int c1 = 0; c1 < sizeMap; c1++)
		for (int c2 = 0; c2 < sizeMap; c2++)
		{
			if (c1 == 0 || c1 == sizeMap - 1 || c2 == 0 || c2 == sizeMap - 1)
				levelSize[c1][c2] = 999;
		}

	PatchFound();
	checkdiag();

	for (int c1 = 0; c1 < sizeMap; c1++)
		for (int c2 = 0; c2 < sizeMap; c2++)
		{
			if (c1 == 0 || c1 == sizeMap - 1 || c2 == 0 || c2 == sizeMap - 1)
				levelSize[c1][c2] = 999;
		}
}

void ARuinesMap::SpawnSprites()
{
	UWorld* const World = GetWorld();
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Owner = this;

	if (World)
	{
		for (int i = 0; i < sizeMap; i++)
			for (int j = 0; j < sizeMap; j++)
			{
#define rad 64
				FVector position(i*rad, j*rad, 0);
				FRotator rotator(0, 0, 90);

				switch (levelSize[i][j])
				{
				case 0: break; 
				case 999: {Walls.push_back(GetWorld()->SpawnActor<AActor>(BpWall, position, rotator, SpawnInfo)); break;	}
				default:
					{
						vMob.push_back(GetWorld()->SpawnActor<AMob>(MyMob, position, rotator, SpawnInfo));
						AMap[i][j] = vMob.back();
						AMap[i][j]->myBody = Body(levelSize[i][j], i, j, lvl);
						//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Some variable values: X: %i, Y: %i"), AMap[i][j]->myBody.cx, AMap[i][j]->myBody.cy));
						break;
					}
#undef	rad
				}

			}
	}
}

void ARuinesMap::checkdiag()
{
	for (int i = 1; i < sizeMap - 1; i++)
		for (int j = 1; j < sizeMap - 1; j++)
		{
			if (levelSize[i][j] == 999)
				if (levelSize[i + 1][j + 1] == 999)
					if (levelSize[i + 1][j] == 0 && levelSize[i][j + 1] == 0)
						if (rand() % 2)
							levelSize[i][j] = 0;
						else
							levelSize[i + 1][j + 1] = 0;
		}

	for (int i = 1; i < sizeMap - 1; i++)
		for (int j = 1; j < sizeMap - 1; j++)
		{
			if (levelSize[i][j] == 0)
				if (levelSize[i + 1][j + 1] == 0)
					if (levelSize[i + 1][j] == 999 && levelSize[i][j + 1] == 999)
						if (rand() % 2)
							levelSize[i + 1][j] = 0;
						else
							levelSize[i][j + 1] = 0;
		}
}

void ARuinesMap::PatchFound()
{
	//std::cout << "RL.Size" << RL.size() << std::endl;
	int v = RL.size();
	for (auto &xxx : RL)
	{
		Point *x = &xxx;
		Point *xx = &xxx;
		xx++;
		if (v == 1)break;
		generatePassage(*x, *xx);
		v--;
	}
}

void ARuinesMap::generatePassage(const Point &start, const Point &finish)
{
	// для хранения направления на "родительскую" клетку
	std::vector<int> parents(sizeMap * sizeMap, -1);
	// приоритетная очередь доступных клеток, отсортирована по "стоимости"
	std::priority_queue<Point> active;
	active.push(start);

	// направления возможных перемещений
	static const int directions[4][2] = { { 1, 0 },{ 0, 1 },{ -1, 0 },{ 0, -1 } };
	while (!active.empty())
	{
		// берем самую "дешевую" клетку из списка доступных
		const Point point = active.top();
		active.pop();

		if (point == finish)
			break;

		// продолжаем поиск в доступных направлениях
		for (int i = 0; i < 4; ++i) {
			Point p(point.x - directions[i][0], point.y - directions[i][1]);
			if (p.x < 0 || p.y < 0 || p.x >= sizeMap || p.y >= sizeMap)
				continue;

			// если мы еще не посещали заданную клетку
			if (parents[p.x + p.y * sizeMap] < 0) {
				// вычисляем "стоимость" указанной клетки
				//p.cost = calcCost(p, finish);
				p.cost = levelSize[p.x][p.y];
				active.push(p);

				parents[p.x + p.y * sizeMap] = i;
			}
		}
	}

	// путь найден - теперь прокладываем его на карте, начиная с конца
	Point point = finish;
	while (!(point == start)) {
		levelSize[point.x][point.y] = 0;

		const int *directon = directions[parents[point.x + point.y * sizeMap]];
		point.x += directon[0];
		point.y += directon[1];
	}
}

int ARuinesMap::Move(int x, int y, AMob*rhs)
{

#define Mob rhs->myBody
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Some variable values: X: %i, Y: %i"), Mob->cx,Mob->cx));
	int diag = 0;
	if (std::abs(Mob.cx - x) == 1 && std::abs(Mob.cy - y) == 1)
		diag = 1;

	if (x >= 0 && x<sizeMap)
		if (y >= 0 && y<sizeMap)
		{
			/*
			if (levelSize[x][y] == 666 || levelSize[x][y] == 777)
				if (Mob == MyHero)
				{
					if (levelSize[x][y] == 666) { ioflag = 1; levelSize[MyHero->cx][MyHero->cy] = 0; }
					if (levelSize[x][y] == 777) { ioflag = 2; levelSize[MyHero->cx][MyHero->cy] = 0; }
					//std::cout << "Go another level" << levelSize[x][y] << std::endl;
					return 1;
				}*/

			if (levelSize[x][y] == 999) { /*GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Стена"));*/ return 0; }
			if (levelSize[x][y] == 0)	//проверка на свободный путь.
			{
				levelSize[Mob.cx][Mob.cy] = NULL;	 // очищаем карту где стояли.
				AMap[x][y] = AMap[Mob.cx][Mob.cy]; // перемещаем указатель в точку которую шагаем
				AMap[Mob.cx][Mob.cy] = nullptr;	 // очищаем на всякий случай старый указатель
				Mob.cx = x; 	Mob.cy = y;		 // присваиваем в теле новые координаты
				levelSize[Mob.cx][Mob.cy] = Mob.role; // создаем на карте новую отметку
				int rad = 64;
				FVector position(Mob.cx*rad,Mob.cy*rad, 0);
				rhs->SetActorLocation(position);	// переносим саму модельку.

				if (diag == 1)
					Mob.tiktak += Mob.rundiagonalS;
				else
					Mob.tiktak += Mob.moveS;
				return 1;
			}
			/*else
				if (Mob->role == 9)
				{
					//Attack(x, y, rhs);
					return 1;

				}
				else
				{
					if (levelSize[x][y] == 9)
					{
						//Attack(x, y, rhs);
						return 1;
					}
				}*/
		}
	return 0;
#undef Mob 
}

void ARuinesMap::rMove(AMob * x) // заставляет убогих, совершать рандомное движение. На крайний случай, anotherMove
{
#define Mob x->myBody
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Some variable values: X: %i, Y: %i"), Mob->cx, Mob->cx));
		int temp = FMath::RandHelper(8);
		switch (temp)
		{
		case 0: {Move(Mob.cx, Mob.cy + 1, x);		 break; }//	Mob.tiktak += Mob.moveS;	 break;  // вроде вверх
		case 1: {Move(Mob.cx + 1, Mob.cy + 1, x);	 break; }//	Mob.tiktak += Mob.rundiagonalS;; break;
		case 2: {Move(Mob.cx + 1, Mob.cy, x);		 break; }//		Mob.tiktak += Mob.moveS;	 break;
		case 3: {Move(Mob.cx + 1, Mob.cy - 1, x);	 break; } //	Mob.tiktak += Mob.rundiagonalS;  break;
		case 4: {Move(Mob.cx, Mob.cy - 1, x);		 break; } //		Mob.tiktak += Mob.moveS;	 break;
		case 5: {Move(Mob.cx - 1, Mob.cy - 1, x);	 break; } //	Mob.tiktak += Mob.rundiagonalS;  break;
		case 6: {Move(Mob.cx - 1, Mob.cy, x);		 break; } //			Mob.tiktak += Mob.moveS; break;
		case 7: {Move(Mob.cx - 1, Mob.cy + 1, x);     break; } //		Mob.tiktak += Mob.rundiagonalS;  break;
		default: {				/*		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("DEFAULT"));	*/			   break; }
		}
#undef Mob
}