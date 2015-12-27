// Fill out your copyright notice in the Description page of Project Settings.

#include "Trollguelike.h"
#include "Floor.h"


// Sets default values
AFloor::AFloor()
{
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		FConstructorStatics()
			: PlaneMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane'"))

		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh0"));
	BlockMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	BlockMesh->SetRelativeScale3D(FVector(1.f, 1.f, 0.25f));
	BlockMesh->SetRelativeLocation(FVector(0.f, 0.f, 25.f));
	this->SetActorRotation(FQuat(0, 0, 0,0));
	//BlockMesh->SetRelativeRotation
	//BlockMesh->SetMaterial(0, ConstructorStatics.BlueMaterial.Get());
	BlockMesh->AttachTo(DummyRoot);
	BlockMesh->OnClicked.AddDynamic(this, &AFloor::BlockClicked);
	

	// Save a pointer to the orange material

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFloor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFloor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AFloor::BlockClicked(UPrimitiveComponent* ClickedComp)
{

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("AFloor::BlockClicked(UPrimitiveComponent* ClickedComp)")));
	// Check we are not already active
//	if (!bIsActive)
	//{
		//bIsActive = true;

		// Change material
		

		// Tell the Grid
		//if (OwningGrid != NULL)
		//{
			//OwningGrid->AddScore();
		//}
	//}
}