/*
 *
 * MipPointObject.cc source template automatically generated by a class generator
 * Creation date : jeu. f�vr. 20 2014
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


#include "arborpfa/content/MipPointObject.h"
#include "arborpfa/content/CaloHitHelper.h"

#include "Objects/Cluster.h"
#include "Helpers/GeometryHelper.h"

using namespace pandora;

namespace arborpfa
{

MipPointObject::MipPointObject(Cluster *pCluster, MipPointObject::Type type) :
		m_pCluster(pCluster),
		m_mipPointType(type),
		m_clusterAlreadyCreated(false),
		m_alreadyMergeContent(false)
{

	m_type = ArborObject::MIP_POINT;

	if(NULL == m_pCluster)
		throw StatusCodeException(STATUS_CODE_INVALID_PARAMETER);

	if(MipPointObject::MIP_START_POINT == m_mipPointType)
	{
	 m_position = pCluster->GetCentroid(pCluster->GetInnerPseudoLayer());
	 m_granularity = GeometryHelper::GetHitTypeGranularity(pCluster->GetInnerLayerHitType());
	 m_pseudoLayer = pCluster->GetInnerPseudoLayer();
	}
	else
	{
		m_position = pCluster->GetCentroid(pCluster->GetOuterPseudoLayer());
		m_granularity = GeometryHelper::GetHitTypeGranularity(pCluster->GetOuterLayerHitType());
		m_pseudoLayer = pCluster->GetOuterPseudoLayer();
	}

}



MipPointObject::~MipPointObject() 
{
	/* nop */
}



pandora::StatusCode MipPointObject::MergeContentInCluster(const pandora::Algorithm &algorithm, pandora::Cluster *pCluster)
{
 if(m_alreadyMergeContent)
 	return STATUS_CODE_UNCHANGED;

 CaloHitList clusterCaloHits;
 m_pCluster->GetOrderedCaloHitList().GetCaloHitList(clusterCaloHits);

 std::cout << "Try to delete a cluster(mip)" << std::endl;
 PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::DeleteCluster(algorithm, m_pCluster));

 for(CaloHitList::const_iterator iter = clusterCaloHits.begin() , endIter = clusterCaloHits.end() ; endIter != iter ; ++iter)
 {
 	if(!PandoraContentApi::IsCaloHitAvailable(algorithm, *iter))
 		continue;

 	PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::AddCaloHitToCluster(algorithm, pCluster, *iter));
 }

	m_alreadyMergeContent = true;

	return STATUS_CODE_SUCCESS;
}



pandora::StatusCode MipPointObject::CreateCluster(const pandora::Algorithm &algorithm, pandora::Cluster *&pCluster)
{

	if(m_clusterAlreadyCreated)
		return STATUS_CODE_UNCHANGED;

//	MipPointObject *pOtherPoint = NULL;
//
//	if(this == m_pEndingPoint)
//		pOtherPoint = m_pStartingPoint;
//	else
//		pOtherPoint = m_pEndingPoint;

//	if(pOtherPoint->m_clusterAlreadyCreated)
//		pCluster =

	pCluster = NULL;

	CaloHitList clusterCaloHits;
	m_pCluster->GetOrderedCaloHitList().GetCaloHitList(clusterCaloHits);

	PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::DeleteCluster(algorithm, m_pCluster));
	PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::Cluster::Create(algorithm, &clusterCaloHits, pCluster));





	m_clusterAlreadyCreated = true;

	return STATUS_CODE_SUCCESS;
}


unsigned int MipPointObject::GetPseudoLayer() const
{
	if(MipPointObject::MIP_START_POINT == m_mipPointType)
	 return m_pCluster->GetInnerPseudoLayer();
	else
		return m_pCluster->GetOuterPseudoLayer();
}


pandora::StatusCode MipPointObject::SetAssociatedStartingPoint(MipPointObject *pStartPoint)
{
	if(NULL != m_pStartingPoint)
		return STATUS_CODE_ALREADY_INITIALIZED;

	m_pStartingPoint = pStartPoint;

	return STATUS_CODE_SUCCESS;
}



pandora::StatusCode MipPointObject::SetAssociatedEndingPoint(MipPointObject *pEndPoint)
{
	if(NULL != m_pEndingPoint)
		return STATUS_CODE_ALREADY_INITIALIZED;

	m_pEndingPoint = pEndPoint;

	return STATUS_CODE_SUCCESS;
}


} 
