// Fill out your copyright notice in the Description page of Project Settings.


#pragma once


class AMob;


class  Body
{
public:
	// Sets default values for this actor's properties
	Body();
	Body(int&_role, int x, int y, int lvl);// получает в качестве параметра левел на котором ГГ, что бы расчитать статы мобов. =)



	Body(const Body & rhs);
	Body& operator=(const Body&rhs);
	Body& operator=(Body&&rhs);
	Body(Body&&rhs);
	

	void sKoef();
	int exp;
	int role;
	int hp;
	int maxhp;
	int ag;
	int str;
	int intel;
	int cx;
	int cy;
	double tiktak;		//Таймер... от него зависит то, сколько ты пропустишь.
	double colldownS;	// скорость на колдунство.
	double attackS;		// скорость физ атаки.
	double fizCDS;		// скорость физ умений.
	double moveS;		// скорость ходьбы.
	double rundiagonalS; // скорость ходьбы на искосок.
	double meditationS;  // скорость медитации восстановления маны и хп.
	double shotS;		// скорость выстрела.
	double jumpSpeed;	// скорость прыжка назад или вперед.

						//std::vector<bag>inventory;
	int mana;
	int maxmana;
	AMob *MyKiller;		//ну ты понял че.
};
