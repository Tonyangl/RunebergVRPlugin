// Copyright (C) 2016, 2017 Runeberg (github: 1runeberg, UE4 Forums: runeberg)

/*
The MIT License (MIT)
Copyright (c) 2016, 2017 runeberg (github: 1runeberg, UE4 Forums: runeberg)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "RunebergVR_Gaze.generated.h"


// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FComponentGazeHasHitSignature, FHitResult, GazeHit, float, PercentActive);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FComponentGazeActivateSignature, FHitResult, GazeHit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FComponentGazeLostSignature, FHitResult, LastGazeHit);


// Read only variables
USTRUCT()
struct FGazeReadOnly
{
	GENERATED_USTRUCT_BODY()

	/** Whether or not gaze mode is on */
	bool IsGazing = false;

	/** Gaze has hit */
	bool GazeHasHit = false;

};

// Front Gaze variables
USTRUCT()
struct FFrontGaze
{
	GENERATED_USTRUCT_BODY()

	/** Whether or not gaze mode is on */
	bool StopGazeAfterHit = true;

	/** How long do you need to gaze */
	float GazeCurrentDuration = 0.f;

	/** Actor tag to check for in target(s) */
	FName TargetTag;

	/** Collision type to check for */
	TEnumAsByte<ECollisionChannel> TargetCollisionType;

	/** Target Static Mesh */
	UStaticMesh* TargetStaticMesh = nullptr;

	/** Target Material to apply to the static mesh */
	UMaterialInterface* TargetMaterial = nullptr;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RUNEBERGVRPLUGIN_API URunebergVR_Gaze : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URunebergVR_Gaze();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Read only variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR")
	FGazeReadOnly RuntimeReadOnly;

	// Front Gaze Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR")
	FFrontGaze FrontGazeVariables;

	// Start gaze
	UFUNCTION(BlueprintCallable, Category = "VR")
	void StartGaze(float Gaze_Range = 300.f, float Gaze_TargetDuration = 3.f, bool DrawDebugLine = false);

	// End Gaze
	UFUNCTION(BlueprintCallable, Category = "VR")
	void EndGaze();

	// Blueprint event - When gaze hits a valid object
	UPROPERTY(BlueprintAssignable, Category = "VR")
	FComponentGazeHasHitSignature OnGazeHit;

	// Blueprint event - When gaze activates action (duration met)
	UPROPERTY(BlueprintAssignable, Category = "VR")
	FComponentGazeActivateSignature OnGazeActivate;

	// Blueprint event - When gaze ends
	UPROPERTY(BlueprintAssignable, Category = "VR")
	FComponentGazeLostSignature OnGazeLost;

private:
	/** How far can we gaze */
	float GazeRange = 500.f;

	/** How long do you need to gaze */
	float GazeTargetDuration = 3.f;

	UStaticMeshComponent* TargetMeshComponent;
	bool bDrawDebugLine = false;

	// Whether we have hit something with the line trace for uneven terrain
	bool bTerrainHit = false;
};
