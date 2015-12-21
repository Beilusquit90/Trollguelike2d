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


#define rad 64
// Sets default values
ARuinesMap::ARuinesMap()
{
	
	static ConstructorHelpers::FObjectFinder<UBlueprint> MyWallBP(TEXT("Blueprint'/Game/NewFolder/NewBlueprint.NewBlueprint'"));
	static ConstructorHelpers::FObjectFinder<UBlueprint> MyMobBP(TEXT("Blueprint'/Game/MyMob.MyMob'"));
	static ConstructorHelpers::FObjectFinder<UBlueprint> MyArrowBP(TEXT("Blueprint'/Game/NewFolder/MyArrow.MyArrow'"));
	


	if (MyArrowBP.Object != NULL)
	{
		BPArrow = (UClass*)MyArrowBP.Object->GeneratedClass;
	}
	if (MyWallBP.Object != NULL)
	{
		BpWall = (UClass*)MyWallBP.Object->GeneratedClass;
	}
	if (MyMobBP.Object != NULL)
	{
		MyMob = (UClass*)MyMobBP.Object->GeneratedClass;
	}
	
	lvl = 1;
	for (int i = 0; i < sizeMap; i++)
		for (int j = 0; j < sizeMap; j++)
		{
			levelSize[i][j] = 0;
			MA[i][j] = 0;
		}
	CreateLvl();
		int temp = (rand() % 15) + 10;
	for (int i = 0; i < temp; i++)
	{
	NewMapMan();
	}

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.



	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Конец конструктора"));
	PrimaryActorTick.bCanEverTick = true;
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
	if (vMob.size())
		Activ();
	Super::Tick(DeltaTime);
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
			UWorld* const World = GetWorld();
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.Owner = this;
			FVector position(cx*rad, cy*rad, 0);
			FRotator rotator(0, 0, 0);
			int lvl = 1;
			MyHero = (GetWorld()->SpawnActor<AMob>(MyMob, position, rotator, SpawnInfo));
			MyHero->myBody=(Body(levelSize[cx][cy], cx, cy, lvl));
			MyHero->myBody.role=9; 
			
			//AMap[cx][cy] = MyHero;
			levelSize[cx][cy] = 9;
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Hero role: X: %i"), MyHero->myBody.role));
			break;
		}
	}
}

void ARuinesMap::Activ()
{
	WhoDie();
	if (MyHero->myBody.hp <= 0)
		MyHero->Destroy();
	ClearTactikMap();
	AI(MyHero);
	for (auto const&z : vArrow)
	{
		if (z->tiktak <= 0)
			fly(z);
		else z->tiktak -= 0.2;
	}
	for (auto const&z : vMob)
	{ if(z==MyHero)
	{
		FRotator rotator(0, 0, 0);
		z->SetActorRotation(rotator);
	}
		if (z->myBody.tiktak <= 0)
		{
			if (z->myBody.role == 9)
				rMove(z);
			else
				AI(z);
		}
			else z->myBody.tiktak -= 0.2;
	}
	WhoDie();
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
				FVector position(i*rad, j*rad, 0);
				FRotator rotator(0, 0, 90);
				
				switch (levelSize[i][j])
				{
				case 0: break; 
				case 9: break;
				case 999: {Walls.push_back(GetWorld()->SpawnActor<AActor>(BpWall, position, rotator, SpawnInfo)); break;	}
				default:
					{
						vMob.push_back(GetWorld()->SpawnActor<AMob>(MyMob, position, rotator, SpawnInfo));
						//AMap[i][j] = (vMob.back());
						vMob.back()->myBody= Body(levelSize[i][j], i, j, lvl);
						//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Some variable values: X: %i"), AMap[i][j]->myBody.role));
						break;
					}

				}

			}
		CreatMyHero();
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
#define attack Attack(x, y, rhs);	return 1;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Some variable values: X: %i, Y: %i"), Mob.cx,Mob.cx));
	int diag = 0;
	if (std::abs(Mob.cx - x) == 1 && std::abs(Mob.cy - y) == 1)
		diag = 1;

	if (x >= 0 && x<sizeMap)
		if (rhs->myBody.role != 9)
		{
			if (y >= 0 && y < sizeMap)
			{
				switch (levelSize[x][y])
				{
				case 0: // free patch
				{	levelSize[Mob.cx][Mob.cy] = 0;	 // очищаем карту где стояли.
			//	AMap[x][y] = AMap[Mob.cx][Mob.cy]; // перемещаем указатель в точку которую шагаем
				//AMap[Mob.cx][Mob.cy] = nullptr;	 // очищаем на всякий случай старый указатель
				Mob.cx = x; 	Mob.cy = y;		 // присваиваем в теле новые координаты
				levelSize[Mob.cx][Mob.cy] = Mob.role; // создаем на карте новую отметку
				FVector position(Mob.cx*rad, Mob.cy*rad, 0);
				rhs->SetActorLocation(position);	// переносим саму модельку.

				if (diag == 1)
					Mob.tiktak += Mob.rundiagonalS;
				else
					Mob.tiktak += Mob.moveS;
				return 1; } break;

				case 1: {attack} break; //mob
				case 2: {attack} break; //mob
				case 3: {attack} break; //mob
				case 9: {attack} break; //Hero
				case 666: if (Mob.role == 9) { ioflag = 1; levelSize[MyHero->myBody.cx][MyHero->myBody.cy] = 0; return 1; }break;
				case 777: if (Mob.role == 9) { ioflag = 2; levelSize[MyHero->myBody.cx][MyHero->myBody.cy] = 0; return 1; }break;
				case 999: {return 0; } break;
				default:GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Some variable values: levelSize[x][y]: %i"), levelSize[x][y])); break; break;
				}
			}
		}
		else
		{

			switch (levelSize[x][y])
			{
			case 0: // free patch
			{	levelSize[Mob.cx][Mob.cy] = 0;	 // очищаем карту где стояли.
												 //	AMap[x][y] = AMap[Mob.cx][Mob.cy]; // перемещаем указатель в точку которую шагаем
												 //AMap[Mob.cx][Mob.cy] = nullptr;	 // очищаем на всякий случай старый указатель
			Mob.cx = x; 	Mob.cy = y;		 // присваиваем в теле новые координаты
			levelSize[Mob.cx][Mob.cy] = Mob.role; // создаем на карте новую отметку
			FVector position(Mob.cx*rad, Mob.cy*rad, 0);
			rhs->SetActorLocation(position);	// переносим саму модельку.

			if (diag == 1)
				Mob.tiktak += Mob.rundiagonalS;
			else
				Mob.tiktak += Mob.moveS;
			return 1; } break;

			case 1: {attack} break; //mob
			case 2: {attack} break; //mob
			case 3: {attack} break; //mob
			case 9: {return 0; } break; //Hero
			case 666: if (Mob.role == 9) { ioflag = 1; levelSize[MyHero->myBody.cx][MyHero->myBody.cy] = 0; return 1; }break;
			case 777: if (Mob.role == 9) { ioflag = 2; levelSize[MyHero->myBody.cx][MyHero->myBody.cy] = 0; return 1; }break;
			case 999: {return 0; } break;
			default:GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Some variable values: levelSize[x][y]: %i"), levelSize[x][y])); break; break;
			}
		}
return 0;
#undef attack
#undef Mob 
}

void ARuinesMap::rMove(AMob * x) // Random moved.
{
#define Mob x->myBody
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Some variable values: X: %i, Y: %i"), Mob.cx, Mob.cx));
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

void ARuinesMap::AI(AMob*rhs)
{
	
	if (TactikMap[rhs->myBody.cx][rhs->myBody.cy] != 1)
	{
		//lowHp(rhs);
		switch (Scaner(rhs, 30))
		{
		case 0:rMove(rhs); break;	// когда мы не видим врага.
		case 1:
			switch (rhs->myBody.role)// 1 воин. 2 лучник . 3 маг.
			{
			case 1: AIPF(rhs); break;
			case 2: //if (Scaner(rhs, 7)) { CTM(); Archer(); AFP(rhs); break; }  GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Role create: X: %i"),rhs->myBody.role));
					//else
					 AIPF(rhs); break;
			case 3: AIPF(rhs); break;
			case 9: rMove(rhs);
			default:  break;
			}

		default:
			break;
		}
	}
	else
	{
		//std::cout << "DODGE" << std::endl;
		if (TactikMap[rhs->myBody.cx][rhs->myBody.cy + 1] != 1)Move(rhs->myBody.cx, rhs->myBody.cy + 1, rhs);
		else if (TactikMap[rhs->myBody.cx + 1][rhs->myBody.cy + 1] != 1)Move(rhs->myBody.cx + 1, rhs->myBody.cy + 1, rhs);
		else if (TactikMap[rhs->myBody.cx + 1][rhs->myBody.cy] != 1)Move(rhs->myBody.cx + 1, rhs->myBody.cy, rhs);
		else if (TactikMap[rhs->myBody.cx + 1][rhs->myBody.cy - 1] != 1)Move(rhs->myBody.cx + 1, rhs->myBody.cy - 1, rhs);
		else if (TactikMap[rhs->myBody.cx][rhs->myBody.cy - 1] != 1)Move(rhs->myBody.cx, rhs->myBody.cy - 1, rhs);
		else if (TactikMap[rhs->myBody.cx - 1][rhs->myBody.cy - 1] != 1)Move(rhs->myBody.cx - 1, rhs->myBody.cy - 1, rhs);
		else if (TactikMap[rhs->myBody.cx - 1][rhs->myBody.cy] != 1)Move(rhs->myBody.cx - 1, rhs->myBody.cy, rhs);
		else if (TactikMap[rhs->myBody.cx - 1][rhs->myBody.cy + 1] != 1)Move(rhs->myBody.cx - 1, rhs->myBody.cy + 1, rhs);
		//else //AIPF(rhs);
	}
}

int ARuinesMap::AIPF(AMob*rhs)//AI Path Founder
{
#define zTOP_LEFT myPatch[i - 1][j + 1]
#define zTOP_RIGHT myPatch[i + 1][j + 1]
#define zTOP myPatch[i][j + 1]
#define zBOT_LEFT myPatch[i - 1][j - 1]
#define zBOT_RIGHT myPatch[i + 1][j - 1]
#define zBOT myPatch[i][j - 1]
#define zLEFT myPatch[i - 1][j]
#define zRIGHT myPatch[i + 1][j]

	int myPatch[sizeMap][sizeMap];
	for (int i = 0; i < sizeMap; i++)
		for (int j = 0; j < sizeMap; j++)
			myPatch[i][j] = -3;

	for (int i = 0; i < sizeMap; i++)
		for (int j = 0; j < sizeMap; j++)
			if (levelSize[i][j])myPatch[i][j] = -9;

	for (int i = 0; i < sizeMap; i++)
		for (int j = 0; j < sizeMap; j++)
			if (TactikMap[i][j])myPatch[i][j] = -9;

	myPatch[rhs->myBody.cx][rhs->myBody.cy] = -3;
	myPatch[MyHero->myBody.cx][MyHero->myBody.cy] = 0;

	for (int z = 0; myPatch[rhs->myBody.cx][rhs->myBody.cy] == (-3); z++)
	{
		for (int i = 0; i < sizeMap; i++)
			for (int j = 0; j < sizeMap; j++)
				if (myPatch[i][j] == z)
				{
					if (zTOP == -3)	zTOP = (z + 1);
					if (zTOP_RIGHT == -3)	zTOP_RIGHT = (z + 1);
					if (zRIGHT == -3)	zRIGHT = (z + 1);
					if (zBOT_RIGHT == -3)	zBOT_RIGHT = (z + 1);
					if (zBOT == -3)	zBOT = (z + 1);
					if (zBOT_LEFT == -3)	zBOT_LEFT = (z + 1);
					if (zLEFT == -3)	zLEFT = (z + 1);
					if (zTOP_LEFT == -3)	zTOP_LEFT = (z + 1);
				}
		if (z>15) { rMove(rhs); return 0; }
	}

	int temp[sizeMap][sizeMap];
	for (int i = sizeMap - 1; i >= 0; i--)
		for (int j = sizeMap - 1; j >= 0; j--)
			temp[i][j] = 0;

	for (int i = sizeMap - 1; i >= 0; i--)
		for (int j = sizeMap - 1; j >= 0; j--)
			if (levelSize[i][j] == 999) temp[i][j] = 9;


	int tx = rhs->myBody.cx;
	int ty = rhs->myBody.cy;

	for (; myPatch[tx][ty] != 0;)
	{
		int t = myPatch[tx][ty] - 1;

		if (myPatch[tx][ty + 1] == t) {
			ty++; temp[tx][ty] = 1;
		}
		else if (myPatch[tx + 1][ty] == t) {
			tx++; temp[tx][ty] = 1;
		}
		else if (myPatch[tx][ty - 1] == t) {
			ty--; temp[tx][ty] = 1;
		}
		else if (myPatch[tx - 1][ty] == t) {
			tx--; temp[tx][ty] = 1;
		}
		else if (myPatch[tx + 1][ty + 1] == t) {
			tx++; ty++; temp[tx][ty] = 1;
		}
		else if (myPatch[tx + 1][ty - 1] == t) {
			tx++; ty--; temp[tx][ty] = 1;
		}
		else if (myPatch[tx - 1][ty - 1] == t) {
			tx--; ty--; temp[tx][ty] = 1;
		}
		else if (myPatch[tx - 1][ty + 1] == t) {
			tx--; ty++; temp[tx][ty] = 1;
		}
	}

	if (temp[rhs->myBody.cx][rhs->myBody.cy + 1] == 1) { Move(rhs->myBody.cx, rhs->myBody.cy + 1, rhs); }
	else if (temp[rhs->myBody.cx + 1][rhs->myBody.cy + 1] == 1) { Move(rhs->myBody.cx + 1, rhs->myBody.cy + 1, rhs); }
	else if (temp[rhs->myBody.cx + 1][rhs->myBody.cy] == 1) { Move(rhs->myBody.cx + 1, rhs->myBody.cy, rhs); }
	else if (temp[rhs->myBody.cx + 1][rhs->myBody.cy - 1] == 1) { Move(rhs->myBody.cx + 1, rhs->myBody.cy - 1, rhs); }
	else if (temp[rhs->myBody.cx][rhs->myBody.cy - 1] == 1) { Move(rhs->myBody.cx, rhs->myBody.cy - 1, rhs); }
	else if (temp[rhs->myBody.cx - 1][rhs->myBody.cy - 1] == 1) { Move(rhs->myBody.cx - 1, rhs->myBody.cy - 1, rhs); }
	else if (temp[rhs->myBody.cx - 1][rhs->myBody.cy] == 1) { Move(rhs->myBody.cx - 1, rhs->myBody.cy, rhs); }
	else if (temp[rhs->myBody.cx - 1][rhs->myBody.cy + 1] == 1) { Move(rhs->myBody.cx - 1, rhs->myBody.cy + 1, rhs); }
	//else std::cout << "ERROR IN AIPF(Body*rhs) func!" << std::endl;
	return 0;
}

int ARuinesMap::AFP(AMob*rhs)	// Archer logic->
{
	int xx = MyHero->myBody.cx - rhs->myBody.cx;
	int yy = MyHero->myBody.cy - rhs->myBody.cy;

	if (ShotMap[rhs->myBody.cx][rhs->myBody.cy] == 1)
	{
		if (xx == 0 && yy > 0 && CheckDir(0, rhs) != 0)Shot(rhs, 0);
		else if (xx > 0 && yy > 0 && CheckDir(1, rhs) != 0)Shot(rhs, 1);
		else if (xx > 0 && yy == 0 && CheckDir(2, rhs) != 0)Shot(rhs, 2);
		else if (xx > 0 && yy < 0 && CheckDir(3, rhs) != 0)Shot(rhs, 3);
		else if (xx == 0 && yy < 0 && CheckDir(4, rhs) != 0)Shot(rhs, 4);
		else if (xx < 0 && yy < 0 && CheckDir(5, rhs) != 0)Shot(rhs, 5);
		else if (xx < 0 && yy == 0 && CheckDir(6, rhs) != 0)Shot(rhs, 6);
		else if (xx < 0 && yy > 0 && CheckDir(7, rhs) != 0)Shot(rhs, 7);
		else if (Scaner(rhs, 1)) { Move(MyHero->myBody.cx, MyHero->myBody.cy, rhs); }
		else { /*std::cout << "AIPF next" << std::endl;*/ AIPF(rhs); }
	}

	else if (ShotMap[rhs->myBody.cx][rhs->myBody.cy + 1] == 1 && levelSize[rhs->myBody.cx][rhs->myBody.cy + 1] == 0)Move(rhs->myBody.cx, rhs->myBody.cy + 1, rhs);
	else if (ShotMap[rhs->myBody.cx + 1][rhs->myBody.cy + 1] == 1 && levelSize[rhs->myBody.cx + 1][rhs->myBody.cy + 1])Move(rhs->myBody.cx + 1, rhs->myBody.cy + 1, rhs);
	else if (ShotMap[rhs->myBody.cx + 1][rhs->myBody.cy] != 1 && levelSize[rhs->myBody.cx + 1][rhs->myBody.cy])Move(rhs->myBody.cx + 1, rhs->myBody.cy, rhs);
	else if (ShotMap[rhs->myBody.cx + 1][rhs->myBody.cy - 1] == 1 && levelSize[rhs->myBody.cx + 1][rhs->myBody.cy - 1])Move(rhs->myBody.cx + 1, rhs->myBody.cy - 1, rhs);
	else if (ShotMap[rhs->myBody.cx][rhs->myBody.cy - 1] == 1 && levelSize[rhs->myBody.cx][rhs->myBody.cy - 1])Move(rhs->myBody.cx, rhs->myBody.cy - 1, rhs);
	else if (ShotMap[rhs->myBody.cx - 1][rhs->myBody.cy - 1] == 1 && levelSize[rhs->myBody.cx - 1][rhs->myBody.cy - 1])Move(rhs->myBody.cx - 1, rhs->myBody.cy - 1, rhs);
	else if (ShotMap[rhs->myBody.cx - 1][rhs->myBody.cy] == 1 && levelSize[rhs->myBody.cx - 1][rhs->myBody.cy])Move(rhs->myBody.cx - 1, rhs->myBody.cy, rhs);
	else if (ShotMap[rhs->myBody.cx - 1][rhs->myBody.cy + 1] == 1 && levelSize[rhs->myBody.cx - 1][rhs->myBody.cy + 1])Move(rhs->myBody.cx - 1, rhs->myBody.cy + 1, rhs);
	else {  /*std::cout << "AIPF next" << std::endl;*/ AIPF(rhs); }

	return 0;
}

void ARuinesMap::CTM() // clear tactik map
{
	for (int i = 0; i < sizeMap; i++)
		for (int j = 0; j < sizeMap; j++)
			if (levelSize[i][j] == 999) ShotMap[i][j] = levelSize[i][j];
			else ShotMap[i][j] = 0;
}

int ARuinesMap::Scaner(AMob*rhs, int r)
{
	int radius = r;
	if (abs((rhs->myBody.cx) - (MyHero->myBody.cx)) < radius && abs((rhs->myBody.cy) - (MyHero->myBody.cy)) < radius)
		return 1;
	else
		return 0;
}

int ARuinesMap::Shot(AMob*rhs, int dir)
{
#define rad 64
	//if (rhs->inventory->arrow > 0)
	//{
	int r;
	if (rhs->myBody.ag > 25)r = 7;
	else r = 5;


	UWorld* const World = GetWorld();
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Owner = this;

	if (World)
	{
		FVector position(rhs->myBody.cx*rad, rhs->myBody.cy*rad, 0);
		FRotator rotator(0, 0, 90);
		vArrow.push_back(GetWorld()->SpawnActor<AArrow>(BPArrow, position, rotator, SpawnInfo));
		vArrow.back()->SetInfo(rhs, dir, r);
	}
	else
	return 0;
	
	MA[rhs->myBody.cx][rhs->myBody.cy] = 1;
	rhs->myBody.tiktak += rhs->myBody.shotS;
	return 3;
}

void ARuinesMap::Archer()
{
	for (int i = 0; i < sizeMap; i++)
		for (int j = 0; j < sizeMap; j++)
			if (levelSize[i][j] == 999) ShotMap[i][j] = levelSize[i][j];
			else ShotMap[i][j] = 0;
			ShotMap[MyHero->myBody.cx][MyHero->myBody.cy] = 1;
			int xx = MyHero->myBody.cx;
			int yy = MyHero->myBody.cy;
			for (int i = 0; i < 7 && ShotMap[xx][++yy] != 999; i++) ShotMap[xx][yy] = 1;
			yy = MyHero->myBody.cy; xx = MyHero->myBody.cx;
			for (int i = 0; i < 7 && ShotMap[++xx][++yy] != 999; i++) ShotMap[xx][yy] = 1;
			yy = MyHero->myBody.cy; xx = MyHero->myBody.cx;
			for (int i = 0; i < 7 && ShotMap[++xx][yy] != 999; i++) ShotMap[xx][yy] = 1;
			yy = MyHero->myBody.cy; xx = MyHero->myBody.cx;
			for (int i = 0; i < 7 && ShotMap[++xx][--yy] != 999; i++) ShotMap[xx][yy] = 1;
			yy = MyHero->myBody.cy; xx = MyHero->myBody.cx;
			for (int i = 0; i < 7 && ShotMap[xx][--yy] != 999; i++) ShotMap[xx][yy] = 1;
			yy = MyHero->myBody.cy; xx = MyHero->myBody.cx;
			for (int i = 0; i < 7 && ShotMap[--xx][--yy] != 999; i++) ShotMap[xx][yy] = 1;
			yy = MyHero->myBody.cy; xx = MyHero->myBody.cx;
			for (int i = 0; i < 7 && ShotMap[--xx][yy] != 999; i++) ShotMap[xx][yy] = 1;
			yy = MyHero->myBody.cy; xx = MyHero->myBody.cx;
			for (int i = 0; i < 7 && ShotMap[--xx][++yy] != 999; i++) ShotMap[xx][yy] = 1;	
}

int ARuinesMap::CheckDir(int dir, AMob*rhs)	// смотрит что бы мобы не постреляли друг друга-> типо стреляем так, что бы все могли увернуться->
{
	int x = rhs->myBody.cx;
	int y = rhs->myBody.cy;

	for (int i = 0; i < 4; i++)
	{
		switch (dir)
		{
		case 0:	y++; break;
		case 1:	x++; y++; break;
		case 2:	x++; break;
		case 3:	x++; y--; break;
		case 4:	y--; break;
		case 5:	x--; y--; break;
		case 6:	x--; break;
		case 7:	x--; y++; break;
		default: break;
		}
		if (levelSize[x][y] == 1 || levelSize[x][y] == 2 || levelSize[x][y] == 3) 	return 0;
		
	}
	return 1;
}

void ARuinesMap::Attack(int _x, int _y, AMob*rhs)
{
	if (levelSize[_x][_y])
	{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Attack")));
			for (auto*z : vMob)if (z->myBody.cx == _x || z->myBody.cx == _y)
			{
				z->myBody.hp -= rhs->myBody.str;
			//	z->myBody.MyKiller* = &rhs;
			}
			//AMap[_x][_y]->myBody.hp -= rhs->myBody.str;
			//AMap[_x][_y]->myBody.MyKiller = rhs;
			rhs->myBody.tiktak += rhs->myBody.attackS;
	}
}

int ARuinesMap::fly(AArrow*bird)
{

	bird->range--;
	if (bird->range == 0) {	bird->life = 0; return 0;}

#define dxb dx = bird->cx
#define dyb dy = bird->cx
	bird->tiktak += 0.2;
	int dx=0, dy=0;
	switch (bird->direction)
	{
	case 0: { dxb	 ;	dyb + 1 ; }	break;
	case 1: { dxb + 1;	dyb + 1 ; }	break;
	case 2: { dxb + 1;	dyb		; }	break;
	case 3: { dxb + 1;	dyb - 1 ; }	break;
	case 4: { dxb	 ;	dyb - 1 ; }	break;
	case 5: { dxb - 1;	dyb - 1 ; }	break;
	case 6: { dxb - 1;	dyb		; }	break;
	case 7: { dxb - 1;	dyb + 1	; }	break;
	default: /*std::cout << "Danger in Fly(Mamap&Bird)"<< std::endl;*/  break;
	}
#undef dxb 
#undef dyb 
#define bcx bird->cx
#define bcy bird->cy
#define MobAttack {for (auto*z : vMob)if (z->myBody.cx == dx || z->myBody.cx == dy)	{z->myBody.hp -= bird->dmg; bird->life = 0; z->myBody.MyKiller = bird->Attacker; return 0;} }break;


	if (mMap[dx][dy]) // если столкнется с другой стрелой
	{
		MA[bcx][bcy] = 0;
		MA[dx][dy] = 0;
		mMap[bcx][bcy]->life = 0;
		mMap[dx][dy]->life = 0;
		return 0;
	}
	else
	if (levelSize[dx][dy] == 999 || levelSize[dx][dy] == 666 || levelSize[dx][dy] == 777) { // если столкнется со стеной или дверьми
		bird->life = 0; return 0;
	}
	else
		if (levelSize[dx][dy])
		{
			switch (levelSize[dx][dy])
			{
			case 0:{MA[bcx][bcy] = 0;	mMap[dx][dy] = mMap[bcx][bcy];
					mMap[bcx][bcy] = nullptr;	bcx = dx;	bcy = dy;
					MA[dx][dy] = 1;	DangerBirds(dx, dy, bird);	//set point on tactical map
					FVector position(bcx*rad, bcy*rad, 0);
					bird->SetActorLocation(position);	return 0; }
			case 1:return 0; break;//MobAttack
			case 2:return 0; break;//MobAttack
			case 3:return 0; break;//MobAttack
			case 9:MobAttack
			case 666: {bird->life = 0; return 0; } break;
			case 777: {bird->life = 0; return 0; } break;
			case 999: {bird->life = 0; return 0; } break;
			default: break;
			}
		}
	return 0;
#undef  MobAttack
#undef bcx 
#undef bcy
}

void ARuinesMap::DangerBirds(int dx, int dy, AArrow*bird)
{
#define forAllgood (int i = 0; levelSize[dx][dy] != 999 && levelSize[dx][dy] != 9 && i < bird->range; i++) 
#define Tmap TactikMap[dx][dy]
	Tmap = 1;
	switch (bird->direction)
	{
	case 0:	for forAllgood	{dy++; Tmap = 1;}break;
	case 2:	for forAllgood{	dx++; Tmap = 1;	}break;
	case 4:	for forAllgood{	dy--; Tmap = 1;	}break;
	case 6:	for forAllgood{	dx--; Tmap = 1;	}break;
	case 1:	for forAllgood{	dx++; dy++; Tmap = 1;}break;
	case 3:	for forAllgood{	dx++; dy--; Tmap = 1;}break;
	case 5:	for forAllgood{	dx--; dy--; Tmap = 1;}break;
	case 7:	for forAllgood{	dx--; dy++; Tmap = 1;}break;
	default:/*std::cout << "Abonormal Dirrections in DangerBirds() func." << std::endl;*/ break;
	}
}

void ARuinesMap::ClearTactikMap() // Clear Tactc map for archers and another
{
	for (int i = 0; i < sizeMap; i++)
		for (int j = 0; j < sizeMap; j++)
			if (levelSize[i][j] == 999)TactikMap[i][j] = 9;
			else TactikMap[i][j] = 0;
}

void ARuinesMap::WhoDie()
{
	switch (FMath::RandHelper(2))
	{
	case 0: {flyDeath(); BodyDeath(); } break;
	case 1: {BodyDeath(); flyDeath(); } break;
	default: GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("DEFAULT? WTF")));
	}	
}

void ARuinesMap::flyDeath() // Lets check, who or "Arrow" die
{
	for (int i = vArrow.size() - 1; i >= 0; i--)
	{
		if (vArrow[i]->life <= 0)
		{
			int cx = vArrow[i]->cx;
			int cy = vArrow[i]->cy;
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Arrow die")));
			mMap[cx][cy] = 0;
			MA[cx][cy] = 0;
			auto const &z = vArrow[i];
			vArrow.erase(vArrow.begin() + i);
			z->Destroy();
		}
	}
}

void ARuinesMap::BodyDeath()  // Lets check, who or "Mob" die
{
	std::vector<int>temp;
	for (int i = 0; i <= vMob.size()-1; i++)
	{
		/*if (vMob[i]->myBody.hp <= 0)
		{
			int cx = vMob[i]->myBody.cx;
			int cy = vMob[i]->myBody.cy;
			AMap[cx][cy] = nullptr;
			
			auto const &z = vMob[i];
			vMob.erase(vMob.begin() + i);
			z->Destroy();
			levelSize[cx][cy] = 0;
		}*/
		
		if (vMob[i]->myBody.hp <= 0)
			temp.push_back(i);
		}

	for (int i = temp.size() - 1; i >= 0; i--)
	{
		int cx = vMob[temp[i]]->myBody.cx;
		int cy = vMob[temp[i]]->myBody.cy;
		 GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Role who die %i"), vMob[temp[i]]->myBody.role));
		vMob[temp[i]]->Destroy();
		levelSize[cx][cy] = 0;
		
		//AMap[cx][cy] = nullptr;
		vMob.erase(vMob.begin() + temp[i]);
	}
	
}

/*
void AMaMap::SpawnArrow(AMob*rhs, int dir, int r)
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> MyArrowBP(TEXT("Blueprint'/Game/NewFolder/MyArrow.MyArrow'"));
	if (MyArrowBP.Object != NULL)
	{
		BpArrow = (UClass*)MyArrowBP.Object->GeneratedClass;
	}




	UWorld* const World = GetWorld();
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Owner = this;

	if (World)
	{
		FVector position(rhs->myBody.cx*rad, rhs->myBody.cy*rad, 0);
		FRotator rotator(0, 0, 90);
		//myArrow=(GetWorld()->SpawnActor<AArrow>(BpArrow, position, rotator, SpawnInfo));
	}
}
*/