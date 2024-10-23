#include "Structs.h"
Warnings myError = Warnings::NO_WARNING;
int squads::count = 0;
bool hasWarn(Warnings myError)
{
	return (myError != Warnings::NO_WARNING);
}