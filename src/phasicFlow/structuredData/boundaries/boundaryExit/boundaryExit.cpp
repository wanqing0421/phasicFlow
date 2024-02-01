/*------------------------------- phasicFlow ---------------------------------
      O        C enter of
     O O       E ngineering and
    O   O      M ultiscale modeling of
   OOOOOOO     F luid flow       
------------------------------------------------------------------------------
  Copyright (C): www.cemf.ir
  email: hamid.r.norouzi AT gmail.com
------------------------------------------------------------------------------  
Licence:
  This file is part of phasicFlow code. It is a free software for simulating 
  granular and multiphase flows. You can redistribute it and/or modify it under
  the terms of GNU General Public License v3 or any other later versions. 
 
  phasicFlow is distributed to help others in their research in the field of 
  granular and multiphase flows, but WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

-----------------------------------------------------------------------------*/

#include "boundaryExit.hpp"
#include "internalPoints.hpp"
#include "dictionary.hpp"


pFlow::boundaryExit::boundaryExit
(
	const dictionary& dict,
	const plane&      bplane,
	internalPoints&   internal
)
:
	boundaryBase(dict, bplane, internal)
{
	exitMarginLength_ = max( 
		dict.getValOrSet("exitMarginLength",0.0), 0.0);
	checkForExitInterval_ = max(
		dict.getValOrSet("checkForExitInterval", 1), 1);
}

bool pFlow::boundaryExit::beforeIteration
(
	uint32 iterNum, 
	real t,
	real dt
)
{
	// nothing have to be done
	if(empty())
	{
		return true;
	}

	uint32 s = size();
	deviceViewType1D<uint32> deleteFlags("deleteFlags",s+1); 
	fill(deleteFlags, 0, s+1, 0u);

	auto points = thisPoints();
	auto p = boundaryPlane().infPlane();

	uint32 numDeleted = 0;	

	Kokkos::parallel_reduce
	(
		"boundaryExit::beforeIteration",
		deviceRPolicyStatic(0,s),
		LAMBDA_HD(uint32 i, uint32& delToUpdate)
		{
			if(p.pointInNegativeSide(points(i)))
			{
				deleteFlags(i)=1;
				delToUpdate++;
			}
		}, 
		numDeleted
	);
		
	// no point is deleted
	if(numDeleted == 0 )
	{
		return true;
	}

	return this->removeIndices(numDeleted, deleteFlags);
}

bool pFlow::boundaryExit::iterate
(
	uint32 iterNum, 
	real t
)
{
	return true;
}

bool pFlow::boundaryExit::afterIteration
(
	uint32 iterNum, 
	real t
)
{
	return true;
}