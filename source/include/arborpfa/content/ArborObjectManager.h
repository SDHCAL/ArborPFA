/*
 *
 * ArborObjectManager.h header template automatically generated by a class generator
 * Creation date : jeu. f�vr. 13 2014
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


#ifndef ARBOROBJECTMANAGER_H
#define ARBOROBJECTMANAGER_H

#include "Managers/AlgorithmObjectManager.h"
#include "Pandora/PandoraInternal.h"

#include "arborpfa/content/ArborObject.h"


namespace arborpfa
{

class ArborAlgorithm;

/** 
 * @brief ArborObjectManager class
 */ 
class ArborObjectManager : public pandora::AlgorithmObjectManager<ArborObject>
{
 public:
 /**
  * @brief Ctor
  */
 ArborObjectManager();

 /**
  * @brief Dtor 
  */
 virtual ~ArborObjectManager();

protected:

 /**
  *
  */
 pandora::StatusCode CreateTemporaryListAndSetCurrent(const ArborAlgorithm *pAlgorithm, std::string &temporaryListName);


 /**
  *
  */
 pandora::StatusCode	ReplaceCurrentAndAlgorithmInputLists(const ArborAlgorithm *algorithm, const std::string &newListName);

 /**
  *
  */
 pandora::StatusCode CreateArborObject(ArborObject *&pArborObject, ArborObject::Type objectType, void *pObject);

 // members
 bool m_canCreateConnectors;

 // friend class field
 friend class ArborImpl;
 friend class ArborContentApiImpl;
}; 

} 

#endif  //  ARBOROBJECTMANAGER_H
