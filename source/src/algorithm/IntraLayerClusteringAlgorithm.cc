/*
 *
 * IntraLayerClusteringAlgorithm.cc source template automatically generated by a class generator
 * Creation date : sam. f�vr. 8 2014
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
 * @author remi
 * @copyright CNRS , IPNL
 */


// arborpfa
#include "arborpfa/algorithm/IntraLayerClusteringAlgorithm.h"

// pandora
#include "Pandora/AlgorithmHeaders.h"

using namespace pandora;

namespace arborpfa
{


StatusCode IntraLayerClusteringAlgorithm::Run()
{

	const CaloHitList *pCaloHitList = NULL;

	PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetCurrentCaloHitList(*this, pCaloHitList));

	if(pCaloHitList->empty())
		return STATUS_CODE_INVALID_PARAMETER;

	OrderedCaloHitList orderedCaloHitList;
	PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, orderedCaloHitList.Add(*pCaloHitList));

	// loop over layers
	for(OrderedCaloHitList::iterator iter = orderedCaloHitList.begin(), iterEnd = orderedCaloHitList.end(); iter != iterEnd; ++iter)
	{

		PseudoLayer layer = iter->first;
		CaloHitList *pLayerCaloHitList = iter->second;

		// loop over calo hit in layer
		for(CaloHitList::iterator iter = pLayerCaloHitList->begin(), endIter = pLayerCaloHitList->end() ; iter != endIter ; ++iter)
		{

			CaloHit *pCaloHit = *iter;
			Cluster *pCluster = NULL;

			if( std::find(m_alreadyUsedCaloHitList.begin(), m_alreadyUsedCaloHitList.end(), pCaloHit) != m_alreadyUsedCaloHitList.end())
				continue;

			m_alreadyUsedCaloHitList.insert(pCaloHit);

			// create a cluster ...
			PANDORA_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::Cluster::Create(*this, pCaloHit, pCluster));

			// ... and build it
			PANDORA_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, this->RecursiveClustering(pLayerCaloHitList, pCluster, pCaloHit));

		}

	}

	m_alreadyUsedCaloHitList.clear();

	return STATUS_CODE_SUCCESS;
}


//---------------------------------------------------------------------------------------------------------------

StatusCode IntraLayerClusteringAlgorithm::RecursiveClustering(CaloHitList *pCaloHitList, Cluster *pCluster, CaloHit *pCaloHit)
{

	for(CaloHitList::iterator iter = pCaloHitList->begin() , endIter = pCaloHitList->end() ; iter != endIter ; ++iter)
	{

		if(pCaloHit == *iter)
			continue;

		CaloHit *pOtherCaloHit = *iter;

		const CartesianVector hitPosition(pCaloHit->GetPositionVector());
		const CartesianVector otherHitPosition(pOtherCaloHit->GetPositionVector());
		const float separationDistance = (hitPosition - otherHitPosition).GetMagnitude();

		if(separationDistance > m_intraLayerMaxDistance)
			continue;

		if(std::find(m_alreadyUsedCaloHitList.begin(), m_alreadyUsedCaloHitList.end(), pOtherCaloHit) != m_alreadyUsedCaloHitList.end())
			continue;

		m_alreadyUsedCaloHitList.insert(pOtherCaloHit);

		PANDORA_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::AddCaloHitToCluster(*this, pCluster, pOtherCaloHit));
		PANDORA_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, this->RecursiveClustering(pCaloHitList , pCluster, pOtherCaloHit));

	}

	return STATUS_CODE_SUCCESS;

}

//----------------------------------------------------------------------------------------------------------------

StatusCode IntraLayerClusteringAlgorithm::ReadSettings(const TiXmlHandle xmlHandle)
{

 PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle,
     "IntraLayerMaxDistance", m_intraLayerMaxDistance));

	return STATUS_CODE_SUCCESS;
}


}
