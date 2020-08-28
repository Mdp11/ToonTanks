// Mattia De Prisco 2020

#include "PlayerControllerBase.h"

void APlayerControllerBase::SetPlayerEnabledState(const bool PlayerEnabled)
{
    if (PlayerEnabled)
    {
        GetPawn()->EnableInput(this);
    }
    else
    {
        GetPawn()->DisableInput(this);
    }
}
