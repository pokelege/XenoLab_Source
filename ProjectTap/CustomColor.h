
#pragma once
#include "ObjectBase.h"
#include "Math/Vector.h"

UENUM( CustomColor )
enum class CustomColor : uint8
{
	Tomato = 0 ,
	LawnGreen = 1,
	FireBrick = 2,
	CornflowerBlue = 3,
	HotPink = 4,
	PapayaWhip = 5,
	Amethyst = 6
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