

#include "TauCetiF2.h"
#include "SaveGameCarrier.h"




USaveGameCarrier::USaveGameCarrier() {
	ContainsFullSaveData = false;
	SaveLoaded = false;


	SaveName = TEXT("Pr�zdn� save");
	WorldName = TEXT("");
	//PlayedTime(0);
	
	
	
	TimeOfDay = 0;
}