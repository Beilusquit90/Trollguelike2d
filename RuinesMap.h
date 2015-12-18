// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
const int sizeMap = 50;
#include <vector>
#include "Body.h"
#include "Mob.h"
#include "GameFramework/Actor.h"
#include "RuinesMap.generated.h"


UCLASS()
class TROLLGUELIKE_API ARuinesMap : public AActor
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	ARuinesMap();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	class Point
	{
	public:
		Point(int _x, int _y) :x(_x), y(_y) {}
		int x, y, cost;

		bool operator==(const Point &p) const {
			return x == p.x && y == p.y;
		}

		bool operator<(const Point &p) const {
			return cost > p.cost;
		}
	};  // нужно для поиска пути




	//void StepsFind(const Point &start, const Point &finish);
	void CreateLvl();
	void MapGen();
	void SpawnSprites();
	void checkdiag();
	void PatchFound();
	void generatePassage(const Point &start, const Point &finish);

	void NewMapMan();
	void CreatMyHero();
	int Move(int x, int y, AMob*rhs);
	void rMove(AMob * x);
	void test(bool &flag);

	float tempmx;
	float tempmy;

	float tempTransx;
	float tempTransy;
	bool restartFlag;
	int ioflag;
	int mflag;

private:
	Body*MyHero;

	int levelSize[sizeMap][sizeMap];			//массив содержащий отметки существ.
	int MMA[sizeMap][sizeMap];				//Массовая магия, особая так сказать...
	int MA[sizeMap][sizeMap];					//карта магии и стрел.
	int Patch[sizeMap][sizeMap];
	AMob*AMap[sizeMap][sizeMap];
	std::vector<Point>RL;
	
	int startx;
	int starty;
	int lvl;

	std::vector<AActor*>Walls;
	std::vector<AMob*>vMob;
	TSubclassOf<class AActor>BpWall;
	TSubclassOf<class AMob>MyMob;
};
