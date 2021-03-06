/*
 *
 * ArborAlgorithm.cc source template automatically generated by a class generator
 * Creation date : lun. f�vr. 10 2014
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


#include "arborpfa/algorithm/ArborAlgorithm.h"

#include "arborpfa/content/ObjectManager.h"
#include "arborpfa/content/ClusterManager.h"

namespace arbor
{

ArborAlgorithm::ArborAlgorithm() :
		m_pArbor(NULL)
{

}

//--------------------------------------------------------------------------------------------------------------------

ArborAlgorithm::~ArborAlgorithm()
{

}

//--------------------------------------------------------------------------------------------------------------------

Arbor *ArborAlgorithm::GetArbor() const
{
	if(NULL == m_pArbor)
		throw pandora::StatusCodeException(pandora::STATUS_CODE_NOT_INITIALIZED);

	return m_pArbor;
}

//--------------------------------------------------------------------------------------------------------------------

ArborContentApiImpl *ArborAlgorithm::GetArborContentApiImpl() const
{
	if(NULL == m_pArbor)
		throw pandora::StatusCodeException(pandora::STATUS_CODE_NOT_INITIALIZED);

	return m_pArbor->GetArborContentApiImpl();
}

//--------------------------------------------------------------------------------------------------------------------

pandora::StatusCode ArborAlgorithm::RegisterArbor(Arbor *pArbor)
{
	if(NULL == pArbor)
		return pandora::STATUS_CODE_FAILURE;

	m_pArbor = pArbor;

	return pandora::STATUS_CODE_SUCCESS;
}

//--------------------------------------------------------------------------------------------------------------------

pandora::StatusCode ArborAlgorithm::Run()
{
	if(NULL == m_pArbor)
		return pandora::STATUS_CODE_NOT_INITIALIZED;

	PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, m_pArbor->m_pObjectManager->RegisterAlgorithm(this));
	PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, m_pArbor->m_pClusterManager->RegisterAlgorithm(this));

	pandora::StatusCode statusCode = this->RunArborAlgorithm();

	PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, m_pArbor->m_pObjectManager->ResetAlgorithmInfo(this, true));
	PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, m_pArbor->m_pClusterManager->ResetAlgorithmInfo(this, true));

	return statusCode;
}


} 

