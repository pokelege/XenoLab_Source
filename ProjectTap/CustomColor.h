
#pragma once
#include "ProjectTap.h"
#include "Math/Vector.h"

UENUM()
enum class CustomColor : uint8
{
	None = 0,
	Tomato = 1 ,
	LawnGreen = 2,
	FireBrick = 3,
	CornflowerBlue = 4,
	HotPink = 5,
	PapayaWhip = 6,
	Amethyst = 7
};

struct ColorHelpers
{
	static FVector Tomato;
	static FVector LawnGreen;
	static FVector FireBrick;
	static FVector CornflowerBlue;
	static FVector HotPink;
	static FVector PapayaWhip;
	static FVector Amethyst;

	static FVector GetColorFromEnum( const CustomColor color );
}; 