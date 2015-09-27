/*
 *
 * ReclusterHelper.h header template automatically generated by a class generator
 * Creation date : jeu. mars 5 2015
 *
 * This file is part of ArborPFA libraries.
 * 
 * ArborPFA is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * based upon these libraries are permitted. Any copy of these libraries
 * must include this copyright notice.
 * 
 * ArborPFA is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with ArborPFA.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * @author Remi Ete
 * @copyright CNRS , IPNL
 */


#ifndef RECLUSTERHELPER_H
#define RECLUSTERHELPER_H

// pandora
#include "Pandora/PandoraInternal.h"
#include "Pandora/StatusCodes.h"

// arbor
#include "arborpfa/arbor/ArborTypes.h"

namespace arbor
{

class ArborAlgorithm;

/** 
 * @brief ReclusterHelper class
 */ 
class ReclusterHelper 
{
public:

	/**
	 * @brief  ReclusterResult class
	 */
	class ReclusterResult
	{
		/**
		 *
		 */
		ReclusterResult();

		/**
		 *
		 */
		float GetChi2() const;

		/**
		 *
		 */
		unsigned int GetUnassociatedNTracks() const;

		/**
		 *
		 */
		float GetChargedEnergyExcess() const;

		/**
		 *
		 */
		float GetNeutralEnergy() const;

		/**
		 *
		 */
		void SetChi2(float chi2);

		/**
		 *
		 */
		void SetUnAssociatedNTracks(unsigned int nTracks);

		/**
		 *
		 */
		void SetChargedEnergyExcess(float energyExcess);

		/**
		 *
		 */
		void SetNeutralEnergy(float neutralEnergy);

	private:

		float           m_chi2;
		float           m_chargedEnergyExcess;
		float           m_neutralEnergy;
		unsigned int   m_unassociatedNTracks;
	};


	/**
	 *
	 */
	static pandora::StatusCode ExtractReclusterResult(const ArborAlgorithm &algorithm, const arbor::ClusterList &reclusterList,
			ReclusterHelper::ReclusterResult &reclusterResult);

}; 

} 

#endif  //  RECLUSTERHELPER_H