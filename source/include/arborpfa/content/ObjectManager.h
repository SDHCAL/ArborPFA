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

// arbor
#include "arborpfa/arbor/ArborTypes.h"
#include "arborpfa/content/Object.h"


namespace arbor
{

class ArborAlgorithm;

/** 
 * @brief ObjectManager class
 */ 
class ObjectManager : public pandora::AlgorithmObjectManager<arbor::Object>
{
 public:
 /**
  * @brief Ctor
  */
 ObjectManager();

 /**
  * @brief Dtor 
  */
 virtual ~ObjectManager();

protected:

 /**
  *
  */
 pandora::StatusCode CreateTemporaryListAndSetCurrent(const ArborAlgorithm *pAlgorithm, std::string &temporaryListName);

 /**
  *
  */
 pandora::StatusCode ReplaceCurrentAndAlgorithmInputLists(const ArborAlgorithm *pAlgorithm, const std::string &newListName);

 /**
  *
  */
// pandora::StatusCode MoveObjectsToTemporaryListAndSetCurrent(const Algorithm *const pAlgorithm, const std::string &originalListName,
//		    std::string &temporaryListName, const ClusterList &clustersToMove);

 /**
  *
  */
 pandora::StatusCode CreateObject(Object *&pObject, pandora::CaloHit *pInputCaloHit);


 /* Re-clustering related methods */
 /**
  *
  */
 pandora::StatusCode InitializeReclustering(const ArborAlgorithm *pAlgorithm, const ClusterList &clusterList,
 		const std::string &initialMetaName);

 /**
  *
  */
 pandora::StatusCode EndReclustering(const std::string &finalMetaDataName);

 /**
  *
  */
 pandora::StatusCode PrepareReclusterMetaData(const ArborAlgorithm *pAlgorithm, const std::string &newReclusterMetaData, bool copyInitialMetaData);

 /**
  *
  */
 pandora::StatusCode CreateReclusterMetaData(const std::string &metaDataName, bool keepPrevious = true);

 /**
  *
  */
// pandora::StatusCode RemoveCurrentReclusterMetaData();

 /**
  *
  */
 pandora::StatusCode GetReclusteringObjectList(const ObjectList *&pObjectList, std::string &listName) const;


 ReclusterMetaData                  *m_pInitialReclusterMetaData;
 ReclusterMetaData                  *m_pCurrentReclusterMetaData;
 std::string                         m_currentReclusterMetaDataName;
 ReclusterMetaDataMap                m_reclusterMetaDataMap;
 ObjectList                         *m_pReclusteringObjectList;
 bool                               m_reclusteringInitialized;
 bool                               m_firstReclusteringProcess;

 static const std::string          m_reclusteringListName;

 // friend class field
 friend class ArborImpl;
 friend class ArborContentApiImpl;
 friend class ArborAlgorithm;
}; 

} 

#endif  //  OBJECTMANAGER_H
