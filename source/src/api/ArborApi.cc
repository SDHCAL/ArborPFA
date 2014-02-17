/*
 *
 * ArborApi.cc source template automatically generated by a class generator
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


// arborpfa
#include "arborpfa/api/ArborApi.h"
#include "arborpfa/api/ArborApiImpl.h"
#include "arborpfa/arbor/Arbor.h"

// pandora
#include "Api/PandoraApi.h"

using namespace pandora;

namespace arborpfa
{


//---------------------------------------------------------------------------------------------------------------

pandora::StatusCode ArborApi::PrepareEvent(const Arbor &arbor)
{
	return arbor.GetArborApiImpl()->PrepareEvent();
}

//---------------------------------------------------------------------------------------------------------------

pandora::StatusCode ArborApi::GetCurrentConnectorList(const Arbor &arbor, const ConnectorList *&pConnectorList)
{
	std::string listName;
	return arbor.GetArborApiImpl()->GetCurrentConnectorList(pConnectorList, listName);
}

//---------------------------------------------------------------------------------------------------------------

pandora::StatusCode ArborApi::GetConnectorList(const Arbor &arbor, const std::string &listName, const ConnectorList *&pConnectorList)
{
	return arbor.GetArborApiImpl()->GetConnectorList(listName, pConnectorList);
}

//---------------------------------------------------------------------------------------------------------------

pandora::StatusCode ArborApi::Reset(const Arbor &arbor)
{
	return arbor.GetArborApiImpl()->ResetEvent();
}

//---------------------------------------------------------------------------------------------------------------

pandora::StatusCode ArborApi::RegisterAlgorithmFactory(const pandora::Pandora &pandora, Arbor &arbor, const std::string &algorithmType, ArborAlgorithmFactory *pFactory)
{
	return arbor.GetArborApiImpl()->RegisterAlgorithmFactory(pandora, arbor, algorithmType, pFactory);
}

} 
