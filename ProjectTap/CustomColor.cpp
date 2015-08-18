#include "ProjectTap.h"
#include "CustomColor.h"

FVector ColorHelpers::Tomato = FVector(1.0f, 0.388f, 0.278f);
FVector ColorHelpers::LawnGreen = FVector(0.486f, 0.988f, 0);
FVector ColorHelpers::FireBrick = FVector(0.698f, 0.125f, 0.125f);
FVector ColorHelpers::CornflowerBlue = FVector(0.392f, 0.584f, 0.929f);
FVector ColorHelpers::HotPink = FVector(1.0f, 0.412f, 0.706f);
FVector ColorHelpers::PapayaWhip = FVector(1.0f, 0.937f, 0.835f);
FVector ColorHelpers::Amethyst = FVector(0.6f, 0.4f, 0.8f);

FVector ColorHelpers::GetColorFromEnum(const CustomColor color)
{
	switch (color)
	{
	case(CustomColor::LawnGreen) :
		return LawnGreen;
	case(CustomColor::FireBrick) :
		return FireBrick;
	case(CustomColor::CornflowerBlue) :
		return CornflowerBlue;
	case(CustomColor::HotPink) :
		return HotPink;
	case(CustomColor::PapayaWhip) :
		return PapayaWhip;
	case(CustomColor::Amethyst) :
		return Amethyst;
	default:
		return Tomato;
	}
}