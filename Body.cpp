// Fill out your copyright notice in the Description page of Project Settings.

#include "Trollguelike.h"
#include "Body.h"


// Sets default values

Body::Body()
{
	/*//inventory.addr();
	hp = 1; ag = 1; str = 1; intel = 1; tiktak = 0; exp = 0;
	switch (role = rand() % 3 + 1) // 1 воин. 2 лучник . 3 маг.
	{
	case 1:str += 2;   break;
	case 2:ag += 2;    break;
	case 3:intel += 2; break;
	}


	int temp = 1 + rand() % 6;		// задаем рандомную надбавку на статы. =)
	for (int count = 0; count < temp; count++)
	{
		int i = rand() % 4 + 1;
		if (i == 1)maxhp++;
		if (i == 2)ag++;
		if (i == 3)str++;
		if (i == 4)intel++;
	}
	sKoef();*/
}




Body::Body(int&_role, int x, int y, int lvl) :cx(x), cy(y) //раскидываем статы, в зависимости от уровня подземелья.
{
	exp = lvl * 10;
	hp = 1; ag = 1; str = 1; intel = 1; tiktak = 0;
	switch (role = FMath::RandHelper(3)+1) // 1 воин. 2 лучник . 3 маг.
	{
	case 1:str += 2;   break;
	case 2:ag += 2;    break;
	case 3:intel += 2; break;
	}

	_role = role;
	int temp = lvl + rand() % 6;		// задаем рандомную надбавку на статы.
	for (int count = 0; count < temp; count++)
	{
		int i = FMath::RandHelper(4); + 1;
		if (i == 1)maxhp++;
		if (i == 2)ag++;
		if (i == 3)str++;
		if (i == 4)intel++;
	}
	sKoef();
}





Body::Body(const Body&rhs)
{
	cx = rhs.cx;
	cy = rhs.cy;
	hp = rhs.hp;
	ag = rhs.str;
	exp = rhs.exp;
	str = rhs.str;
	mana = rhs.mana;
	role = rhs.role;
	shotS = rhs.shotS;
	maxhp = rhs.maxhp;
	intel = rhs.intel;
	moveS = rhs.moveS;
	fizCDS = rhs.fizCDS;
	tiktak = rhs.tiktak;
	maxmana = rhs.maxmana;
	attackS = rhs.attackS;
	colldownS = rhs.colldownS;
	jumpSpeed = rhs.jumpSpeed;
	meditationS = rhs.meditationS;
	rundiagonalS = rhs.rundiagonalS;
}

Body::Body(Body&&rhs)
{
	cx = std::move(rhs.cx);
	cy = std::move(rhs.cy);
	hp = std::move(rhs.hp);
	ag = std::move(rhs.str);
	exp = std::move(rhs.exp);
	str = std::move(rhs.str);
	mana = std::move(rhs.mana);
	role = std::move(rhs.role);
	shotS = std::move(rhs.shotS);
	maxhp = std::move(rhs.maxhp);
	intel = std::move(rhs.intel);
	moveS = std::move(rhs.moveS);
	fizCDS = std::move(rhs.fizCDS);
	tiktak = std::move(rhs.tiktak);
	maxmana = std::move(rhs.maxmana);
	attackS = std::move(rhs.attackS);
	colldownS = std::move(rhs.colldownS);
	jumpSpeed = std::move(rhs.jumpSpeed);
	meditationS = std::move(rhs.meditationS);
	rundiagonalS = std::move(rhs.rundiagonalS);
}



Body& Body::operator=(const Body*rhs)
{
	cx = rhs->cx;
	cy = rhs->cy;
	hp = rhs->hp;
	ag = rhs->str;
	exp = rhs->exp;
	str = rhs->str;
	mana = rhs->mana;
	role = rhs->role;
	shotS = rhs->shotS;
	maxhp = rhs->maxhp;
	intel = rhs->intel;
	moveS = rhs->moveS;
	fizCDS = rhs->fizCDS;
	tiktak = rhs->tiktak;
	maxmana = rhs->maxmana;
	attackS = rhs->attackS;
	colldownS = rhs->colldownS;
	jumpSpeed = rhs->jumpSpeed;
	meditationS = rhs->meditationS;
	rundiagonalS = rhs->rundiagonalS;
	return *this;
}

Body& Body::operator=(Body&&rhs)
{
	cx = std::move(rhs.cx);
	cy = std::move(rhs.cy);
	hp = std::move(rhs.hp);
	ag = std::move(rhs.str);
	exp = std::move(rhs.exp);
	str = std::move(rhs.str);
	mana = std::move(rhs.mana);
	role = std::move(rhs.role);
	shotS = std::move(rhs.shotS);
	maxhp = std::move(rhs.maxhp);
	intel = std::move(rhs.intel);
	moveS = std::move(rhs.moveS);
	fizCDS = std::move(rhs.fizCDS);
	tiktak = std::move(rhs.tiktak);
	maxmana = std::move(rhs.maxmana);
	attackS = std::move(rhs.attackS);
	colldownS = std::move(rhs.colldownS);
	jumpSpeed = std::move(rhs.jumpSpeed);
	meditationS = std::move(rhs.meditationS);
	rundiagonalS = std::move(rhs.rundiagonalS);
	return *this;
}



void Body::sKoef()
{
	colldownS = (50 - intel / 2)*0.01;						// скорость на колдунство.
	attackS = (50 - (ag / 2))*0.01;						// скорость физ атаки.
	fizCDS = (100 - ((ag / 4) + (str / 4)))*0.01;		// скорость физ умений.
	moveS = (50 - (ag / 4) + (hp / 7))*0.01;			// скорость ходьбы.
	rundiagonalS = moveS*1.5;								// скорость ходьбы на искосок.
	meditationS = 0.50;									// скорость медитации восстановления маны и хп.
	shotS = (70 - (ag / 2))*0.01;						// выстрела скорость.
	jumpSpeed = (70 - (ag / 2))*0.01;					// скорость прыжка назад или вперед.
}


