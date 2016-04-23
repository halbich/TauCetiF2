

#include "TauCetiF2.h"
#include "MinMaxTree.h"




UMinMaxTree* UMinMaxTree::Init(UMinMaxBox* box)
{
	ensure(box != nullptr);
	InitBox(box->Min, box->Max);
	return this;
}