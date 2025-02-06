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
/**
 * @class pFlow::sphereParticles
 *
 * @brief Class for managing spherical particles
 *
 * This is a top-level class that contains the essential components for
 * defining spherical prticles in a DEM simulation.
 */

#ifndef __grainParticles_hpp__
#define __grainParticles_hpp__

#include "indexContainer.hpp"
#include "particles.hpp"
#include "property.hpp"
#include "grainShape.hpp"
#include "boundaryGrainParticlesList.hpp"
#include "systemControl.hpp"

namespace pFlow
{

class grainParticles : public particles
{
public:

	using ShapeType = grainShape;

private:

	/// reference to shapes
	const ShapeType&       grains_;

	/// property id on device
	uint32PointField_D     propertyId_;

	/// diameter / boundig sphere size of particles on device
	realPointField_D       grainDiameter_;

	realPointField_D       coarseGrainFactor_;


	/// mass of particles field
	realPointField_D       mass_;

	/// pointField of inertial of particles
	realPointField_D       I_;

	/// pointField of rotational Velocity of particles on device
	realx3PointField_D     rVelocity_;

	/// pointField of rotational acceleration of particles on device
	realx3PointField_D     rAcceleration_;

	/// boundaries 
	boundaryGrainParticlesList boundaryGrainParticles_;

	/// rotational velocity integrator
	uniquePtr<integration> rVelIntegration_ = nullptr;

	/// timer for acceleration computations
	Timer                  accelerationTimer_;

	/// timer for integration computations (prediction step)
	Timer                  intPredictTimer_;

	/// timer for integration computations (correction step)
	Timer                  intCorrectTimer_;

	Timer  				   fieldUpdateTimer_;

private:
	

	bool getParticlesInfoFromShape(
	  const wordVector& shapeNames,
	  uint32Vector&     propIds,
	  realVector&       diams,
	  realVector&       coarseGrainFactors,
	  realVector&       m,
	  realVector&       Is,
	  uint32Vector&     shIndex
	);
	
protected:

	Timer& accelerationTimer()
	{
		return accelerationTimer_;
	}

	Timer& intCorrectTimer()
	{
		return intCorrectTimer_;
	}

	integration& rVelIntegration()
	{
		return rVelIntegration_();
	}

public:

	/// construct from systemControl and property
	grainParticles(systemControl& control, const grainShape& gShape);

	~grainParticles() override = default;

	/**
	 * Insert new particles in position with specified shapes
	 *
	 * This function is involked by inserted object to insert new set of
	 * particles into the simulation. \param position position of new particles
	 * \param shape shape of new particles
	 * \param setField initial value of the selected fields for new particles
	 */
	/*bool insertParticles
	(
    const realx3Vector& position,
	    const wordVector&  shapes,
	    const setFieldList& setField
	) override ;*/

    // TODO: make this method private later 
    bool initializeParticles();

	/// const reference to shapes object
	const auto& grains() const
	{
		return grains_;
	}

	/// const reference to inertia pointField
	const auto& I() const
	{
		return I_;
	}

	/// reference to inertia pointField
	auto& I()
	{
		return I_;
	}

	const auto& rVelocity() const
	{
		return rVelocity_;
	}

	auto& rVelocity()
	{
		return rVelocity_;
	}

	bool hearChanges(
	  const timeInfo& ti,
	  const message& msg,
	  const anyList& varList
	) override
	{
		notImplementedFunction;
		return false;
	}

	const uint32PointField_D& propertyId() const override
	{
		return propertyId_;
	}

	const realPointField_D& diameter() const override
	{
		return grainDiameter_;
	}

	const realPointField_D& grainDiameter()const
	{
		return grainDiameter_;
	}

	const realPointField_D& coarseGrainFactor() const 
	{
		return coarseGrainFactor_;
	}


	const realPointField_D& mass() const override
	{
		return mass_;
	}

	/// before iteration step
	bool beforeIteration() override;

	/// iterate particles
	bool iterate() override;

	bool insertParticles(
	  const realx3Vector& position,
	  const wordVector&   shapesNames,
	  const anyList&      setVarList
	) override;

	realx3PointField_D& rAcceleration() override
	{
		return rAcceleration_;
	}

	const realx3PointField_D& rAcceleration() const override
	{
		return rAcceleration_;
	}

	const realPointField_D& boundingSphere() const override
	{
		return diameter();
	}

	word         shapeTypeName() const override;

	const shape& getShapes() const override;

	void boundingSphereMinMax(real& minDiam, real& maxDiam) const override;

};// grainParticles

} // pFlow

#endif //__sphereParticles_hpp__
