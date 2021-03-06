  /// \file ArborContentApi.h
/*
 *
 * ArborContentApi.h header template automatically generated by a class generator
 * Creation date : dim. f�vr. 9 2014
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


#ifndef ARBORCONTENTAPI_H
#define ARBORCONTENTAPI_H

// pandora
#include "Pandora/PandoraInternal.h"
#include "Pandora/StatusCodes.h"

// arborpfa
#include "arborpfa/arbor/ArborTypes.h"
#include "arborpfa/content/Object.h"

namespace arbor
{

class ArborAlgorithm;
class ArborAlgorithmFactory;
class Object;

/** 
 * @brief ArborContentApi class
 */ 
class ArborContentApi 
{
 public:

	 class Object
	 {
	  public:

	 		/**
	 		 *
	 		 */
	 	 static pandora::StatusCode Create(const ArborAlgorithm &algorithm, arbor::Object *&pObject, pandora::CaloHit *pInputCaloHit);
	 };

	 class Cluster
	 {
	  public:

	 		/**
	 		 *
	 		 */
	 	 static pandora::StatusCode Create(const ArborAlgorithm &algorithm, arbor::Cluster *&pCluster, arbor::Object *pSeedObject);
	 };

	 class Branch
	 {
	  public:

	 		/**
	 		 *
	 		 */
	 	 static pandora::StatusCode Create(const ArborAlgorithm &algorithm, arbor::Branch *&pBranch, arbor::Tree *pTree);
	 };

	 /*
	  * Object related methods
	  */
	 static pandora::StatusCode GetCurrentObjectList(const ArborAlgorithm &arborAlgorithm, const ObjectList *&pObjectList);


	 static pandora::StatusCode GetCurrentObjectList(const ArborAlgorithm &arborAlgorithm, const ObjectList *&pObjectList, std::string &listName);


	 static pandora::StatusCode GetCurrentObjectListName(const ArborAlgorithm &arborAlgorithm, std::string &listName);


	 static pandora::StatusCode GetObjectList(const ArborAlgorithm &arborAlgorithm, const std::string &listName, const ObjectList *&pObjectList);


  static pandora::StatusCode CreateTemporaryObjectListAndSetCurrent(const ArborAlgorithm &algorithm, const ObjectList *&pObjectList, std::string &temporaryListName);


	 static pandora::StatusCode SaveObjectList(const ArborAlgorithm &algorithm, const std::string &newListName);


	 static pandora::StatusCode SaveObjectList(const ArborAlgorithm &algorithm, const std::string &newObjectListName, const ObjectList &objectsToSave);


	 static pandora::StatusCode SaveObjectList(const ArborAlgorithm &algorithm, const std::string &oldObjectListName, const std::string &newObjectListName);


  static pandora::StatusCode SaveObjectList(const ArborAlgorithm &algorithm, const std::string &oldObjectListName, const std::string &newObjectListName, const ObjectList &connectorsToSave);


	 static pandora::StatusCode ReplaceCurrentObjectList(const ArborAlgorithm &algorithm, const std::string &newListName);


  static pandora::StatusCode TemporarilyReplaceCurrentObjectList(const ArborAlgorithm &algorithm, const std::string &newObjectListName);


  static pandora::StatusCode DropCurrentObjectList(const ArborAlgorithm &algorithm);


  static pandora::StatusCode RemoveAllConnections(const ArborAlgorithm &algorithm, const ObjectList &objectList);

	 /*
	  * Cluster related methods
	  */

	 static pandora::StatusCode GetCurrentClusterList(const ArborAlgorithm &arborAlgorithm, const ClusterList *&pClusterList);


	 static pandora::StatusCode GetCurrentClusterList(const ArborAlgorithm &arborAlgorithm, const ClusterList *&pClusterList, std::string &listName);


	 static pandora::StatusCode GetCurrentClusterListName(const ArborAlgorithm &arborAlgorithm, std::string &listName);


	 static pandora::StatusCode GetClusterList(const ArborAlgorithm &arborAlgorithm, const std::string &listName, const ClusterList *&pClusterList);


  static pandora::StatusCode CreateTemporaryClusterListAndSetCurrent(const ArborAlgorithm &algorithm, const ClusterList *&pClusterList, std::string &temporaryListName);


	 static pandora::StatusCode SaveClusterList(const ArborAlgorithm &algorithm, const std::string &newListName);


	 static pandora::StatusCode SaveClusterList(const ArborAlgorithm &algorithm, const std::string &newClusterListName, const ClusterList &clustersToSave);


	 static pandora::StatusCode SaveClusterList(const ArborAlgorithm &algorithm, const std::string &oldClusterListName, const std::string &newClusterListName);


  static pandora::StatusCode SaveClusterList(const ArborAlgorithm &algorithm, const std::string &oldClusterListName, const std::string &newClusterListName, const ClusterList &arborClustersToSave);


	 static pandora::StatusCode ReplaceCurrentClusterList(const ArborAlgorithm &algorithm, const std::string &newListName);


  static pandora::StatusCode TemporarilyReplaceCurrentClusterList(const ArborAlgorithm &algorithm, const std::string &newClusterListName);


  static pandora::StatusCode DropCurrentClusterList(const ArborAlgorithm &algorithm);


  static pandora::StatusCode DeleteCluster(const ArborAlgorithm &algorithm, arbor::Cluster *pCluster);


  static pandora::StatusCode MergeAndDeleteClusters(const ArborAlgorithm &algorithm, arbor::Cluster *pClusterToEnlarge, arbor::Cluster *pClusterToDelete);

  /**
   * Branch related method
   */

  static pandora::StatusCode RemoveBranchFromTree(const ArborAlgorithm &algorithm, arbor::Tree *pTree, arbor::Branch *pBranch);

  /**
   *
   */
  static pandora::StatusCode MoveTree(const ArborAlgorithm &arborAlgorithm, arbor::Cluster *pOldCluster, arbor::Cluster *pNewCluster, Tree *pTree);

  /**
   *
   */
  static pandora::StatusCode AddPreviousObjectToBranch(const ArborAlgorithm &arborAlgorithm, arbor::Branch *pBranch, arbor::Object *pObject);

  /**
   *
   */
  static pandora::StatusCode AddNextObjectToBranch(const ArborAlgorithm &arborAlgorithm, arbor::Branch *pBranch, arbor::Object *pObject);

  /**
   * Algorithm related methods
   */

  static pandora::StatusCode RunObjectCreationAlgorithm(const ArborAlgorithm &arborAlgorithm, const std::string &arborObjectAlgorithmName,
  		const ObjectList *&pObjectList);


  static pandora::StatusCode RunObjectCreationAlgorithm(const ArborAlgorithm &arborAlgorithm, const std::string &arborObjectAlgorithmName,
  		const ObjectList *&pObjectList, std::string &newObjectListName);


  static pandora::StatusCode RunClusterCreationAlgorithm(const ArborAlgorithm &arborAlgorithm, const std::string &arborClusterAlgorithmName,
  		const ClusterList *&pClusterList);


  static pandora::StatusCode RunClusterCreationAlgorithm(const ArborAlgorithm &arborAlgorithm, const std::string &arborClusterAlgorithmName,
  		const ClusterList *&pClusterList, std::string &newClusterListName);


   // Energy function related methods

  /**
   *  @brief  Get the current energy function name
   *
   *  @param  arborAlgorithm  the algorithm from which the function is called
   *  @param  energyFunctionName  the current energy function name to get by reference
   */
  static pandora::StatusCode GetCurrentEnergyFunctionName(const ArborAlgorithm &arborAlgorithm, std::string &energyFunctionName);

  /**
   *  @brief  Set the current energy function by name
   *
   *  @param  arborAlgorithm  the algorithm from which the function is called
   *  @param  energyFunctionName  the energy function name
   */
  static pandora::StatusCode SetCurrentEnergyFunction(const ArborAlgorithm &arborAlgorithm, const std::string &energyFunctionName);

  /**
   *  @brief  Get the cluster energy estimated from the current energy function
   *
   *  @param  arborAlgorithm  the algorithm from which the function is called
   *  @param  pCluster  the address of the cluster from which we extract the energy
   *  @param  energy  the estimated energy to get by reference
   */
  static pandora::StatusCode GetEnergy(const ArborAlgorithm &arborAlgorithm, const arbor::Cluster *pCluster, float &energy);

  /**
   *  @brief  Get the cluster energy estimated from a given energy function
   *
   *  @param  arborAlgorithm  the algorithm from which the function is called
   *  @param  energyFunctionName  the energy function name
   *  @param  pCluster  the address of the cluster from which we extract the energy
   *  @param  energy  the estimated energy to get by reference
   */
  static pandora::StatusCode GetEnergy(const ArborAlgorithm &arborAlgorithm, const std::string &energyFunctionName, const arbor::Cluster *pCluster, float &energy);

  /**
   *  @brief  Get the cluster energy estimated from the current energy function
   *
   *  @param  arborAlgorithm  the algorithm from which the function is called
   *  @param  pCaloHitList  the address of the calo hit list from which we extract the energy
   *  @param  energy  the estimated energy to get by reference
   */
  static pandora::StatusCode GetEnergy(const ArborAlgorithm &arborAlgorithm, const pandora::CaloHitList *const pCaloHitList, float &energy);

  /**
   *  @brief  Get the calo hit list energy estimated from a given energy function
   *
   *  @param  arborAlgorithm  the algorithm from which the function is called
   *  @param  energyFunctionName  the energy function name
   *  @param  pCaloHitList  the address of the calo hit list from which we extract the energy
   *  @param  energy  the estimated energy to get by reference
   */
  static pandora::StatusCode GetEnergy(const ArborAlgorithm &arborAlgorithm, const std::string &energyFunctionName, const pandora::CaloHitList *const pCaloHitList, float &energy);

  /**
   *  @brief  Get the energy resolution at a given energy from the current energy function
   *
   *  @param  arborAlgorithm  the algorithm from which the function is called
   *  @param  energy  the energy reference point for the energy resolution
   *  @param  energyResolution  the energy resolution to get by reference
   */
  static pandora::StatusCode GetEnergyResolution(const ArborAlgorithm &arborAlgorithm, float energy,		float &energyResolution);

  /**
   *  @brief  Get the energy resolution at a given energy from the given energy function
   *
   *  @param  arborAlgorithm  the algorithm from which the function is called
   *  @param  energyFunctionName  the energy function name
   *  @param  energy  the energy reference point for the energy resolution
   *  @param  energyResolution  the energy resolution to get by reference
   */
  static pandora::StatusCode GetEnergyResolution(const ArborAlgorithm &arborAlgorithm, const std::string &energyFunctionName, float energy,		float &energyResolution);

  /**
   *
   */
  static pandora::StatusCode InitializeReclustering(const ArborAlgorithm &algorithm, const pandora::TrackList &trackList,
  		const arbor::ClusterList &clusterList, std::string &originalClusterListName);

  /**
   *
   */
  static pandora::StatusCode EndReclustering(const ArborAlgorithm &algorithm, const std::string &selectedClusterListName,
  		const std::string &originalTrackListName);

}; 

} 

#endif  //  ARBORCONTENTAPI_H
