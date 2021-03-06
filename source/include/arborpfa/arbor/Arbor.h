/*
 *
 * Arbor.h header template automatically generated by a class generator
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


#ifndef ARBOR_H
#define ARBOR_H

// pandora
#include "Pandora/PandoraInternal.h"
#include "Pandora/PandoraInputTypes.h"

namespace pandora
{
 class Pandora;
}

namespace arbor
{

class ObjectManager;
class ClusterManager;
class ArborPluginManager;
class ArborApiImpl;
class ArborContentApiImpl;
class ArborImpl;

/** 
 * @brief Arbor class
 */ 
class Arbor 
{
 public:

		/**
			* @brief Ctor
			*/
		Arbor();

		/**
			* @brief Dtor
			*/
		virtual ~Arbor();

  /**
   *  @brief  Get the arbor api impl
   *
   *  @return Address of the arbor api impl
   */
  ArborApiImpl *GetArborApiImpl() const;

  /**
   *  @brief  Get the arbor content api impl
   *
   *  @return Address of the arbor content api impl
   */
  ArborContentApiImpl *GetArborContentApiImpl() const;

  /**
   *
   */
  pandora::Pandora *GetPandora() const;

 protected:

		/**
		 * @brief Reset function registered in PandoraApi
		 */
		static void Reset();

		/**
		 *
		 */
		pandora::StatusCode RegisterResetFunction();

		/**
		 *
		 */
		pandora::StatusCode PrepareEvent();

		/**
		 *
		 */
		pandora::StatusCode ResetEvent();

		arbor::ObjectManager        *m_pObjectManager;
		arbor::ClusterManager       *m_pClusterManager;
		arbor::ArborPluginManager   *m_pArborPluginManager;

  ArborApiImpl              *m_pArborApiImpl;             ///< The arbor api implementation
  ArborContentApiImpl       *m_pArborContentApiImpl;     ///< The arbor content api implementation
  ArborImpl                 *m_pArborImpl;                ///< The arbor implementation
  pandora::Pandora          *m_pPandora;                  ///< The wrapped instance of pandora

  friend class ArborApiImpl;
  friend class ArborContentApiImpl;
  friend class ArborImpl;
  friend class ArborAlgorithm;
  friend class ArborPluginManager;
}; 


} 

#endif  //  ARBOR_H
