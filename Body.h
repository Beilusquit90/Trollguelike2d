// Fill out your copyright notice in the Description page of Project Settings.


#pragma once


class AMob;


class  Body
{
public:
	// Sets default values for this actor's properties
	Body();
	Body(int&_role, int x, int y, int lvl);// �������� � �������� ��������� ����� �� ������� ��, ��� �� ��������� ����� �����. =)



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
	double tiktak;		//������... �� ���� ������� ��, ������� �� ����������.
	double colldownS;	// �������� �� ����������.
	double attackS;		// �������� ��� �����.
	double fizCDS;		// �������� ��� ������.
	double moveS;		// �������� ������.
	double rundiagonalS; // �������� ������ �� �������.
	double meditationS;  // �������� ��������� �������������� ���� � ��.
	double shotS;		// �������� ��������.
	double jumpSpeed;	// �������� ������ ����� ��� ������.

						//std::vector<bag>inventory;
	int mana;
	int maxmana;
	AMob *MyKiller;		//�� �� ����� ��.
};
