#pragma once
#include "ObjectBase.h"
#include "Math/Vector.h"

UENUM( CustomColor )
enum class CustomColor : uint8
{
	Tomato = 0 ,
	LawnGreen = 1,
	PowderBlue = 2,
	CornflowerBlue = 3
};

struct ColorHelpers
{
	static FVector Tomato;
	static FVector LawnGreen;
	static FVector PowderBlue;
	static FVector CornflowerBlue;
	static FVector GetColorFromEnum( const CustomColor color );
}; 