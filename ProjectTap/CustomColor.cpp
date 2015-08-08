#include "ProjectTap.h"
#include "CustomColor.h"

FVector ColorHelpers::Tomato = FVector(1.0f, 0.388f, 0.278f);
FVector ColorHelpers::LawnGreen = FVector(0.486f, 0.988f, 0);
FVector ColorHelpers::PowderBlue = FVector(0.69f, 0.878f, 0.902f);
FVector ColorHelpers::CornflowerBlue = FVector(0.392f, 0.584f, 0.929f);
FVector ColorHelpers::GetColorFromEnum(const CustomColor color)
{
	switch (color)
	{
	case(CustomColor::LawnGreen) :
		return LawnGreen;
	case(CustomColor::PowderBlue) :
		return PowderBlue;
	case(CustomColor::CornflowerBlue) :
		return CornflowerBlue;
	default:
		return Tomato;
	}
}