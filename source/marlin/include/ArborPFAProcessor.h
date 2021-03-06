/**
 *  @file   ArborPFA/include/ArborPFAProcessor.h
 * 
 *  @brief  Header file for the arbor pfa processor class.
 * 
 *  $Log: $
 */

#ifndef ARBOR_PFA_PROCESSOR_H
#define ARBOR_PFA_PROCESSOR_H 1

#include "marlin/Processor.h"

#include "CaloHitCreator.h"
#include "GeometryCreator.h"
#include "MCParticleCreator.h"
#include "PfoCreator.h"
#include "TrackCreator.h"

namespace pandora {class Pandora;}
namespace arbor {class Arbor;}

//------------------------------------------------------------------------------------------------------------------------------------------

/**
 *  @brief  ArborPFAProcessor class
 */
class ArborPFAProcessor : public marlin::Processor
{
public:

	typedef std::vector<std::string> StringVector;

	/**
		*  @brief  Settings class
		*/
	class Settings
	{
	public:
		std::string     m_pandoraSettingsXmlFile;           ///< The pandora settings xml file
		int             m_nEventsToSkip;                    ///< Number of events to skip at start of reconstruction job
	};

	/**
		*  @brief  Default constructor
		*/
	ArborPFAProcessor();

	/**
		*  @brief  Create new processor
		*/
	virtual Processor *newProcessor();

	/**
		*  @brief  Initialize, called at startup
		*/
	virtual void init();

	/**
		*  @brief  Process run header
		*
		*  @param  pLCRunHeader the lc run header
		*/
	virtual void processRunHeader(lcio::LCRunHeader *pLCRunHeader);

	/**
		*  @brief  Process event, main entry point
		*
		*  @param  pLCEvent the lc event
		*/
	virtual void processEvent(EVENT::LCEvent *pLCEvent);

	/**
		*  @brief  Checks for event
		*
		*  @param  pLCEvent the lc event
		*/
	virtual void check(EVENT::LCEvent *pLCEvent);

	/**
		*  @brief  End, called at shutdown
		*/
	virtual void end();

	/**
		*  @brief  Get address of the pandora instance
		*
		*  @return address of the pandora instance
		*/
	static pandora::Pandora *GetPandora();

	/**
		*  @brief  Get address of the arbor instance
		*
		*  @return address of the arbor instance
		*/
	static arbor::Arbor *GetArbor();

	/**
		*  @brief  Get address of the current lcio event
		*
		*  @return address of the current lcio event
		*/
	static EVENT::LCEvent *GetCurrentEvent();

private:

	/**
		*  @brief  Register user algorithm factories, energy correction functions and particle id functions,
		*          insert user code here
		*/
	pandora::StatusCode RegisterUserComponents() const;

	/**
		*  @brief  Process steering file parameters, insert user code here
		*/
	void ProcessSteeringFile();

	/**
		*  @brief  Copy some steering parameters between settings objects
		*/
	void FinaliseSteeringParameters();

	/**
		*  @brief  Reset the arbor pfa processor
		*/
	void Reset();

 /**
  *  @brief  The energy correction function for SDHCAL detector that should be used in arbor and pandora
  *
  *  @param pCluster a pandora cluster on which the energy function should be performed
  *  @param energyCorrection the energy correction that is assigned to the cluster
  */
// static void SDHCALEnergyCorrectionFunction(const pandora::Cluster *const pCluster, float &energyCorrection);


	static pandora::Pandora        *m_pPandora;                     ///< Address of the pandora instance
	static arbor::Arbor            *m_pArbor;                       ///< Address of the arbor instance
	static EVENT::LCEvent           *m_pLcioEvent;                   ///< Address of the current lcio event

	GeometryCreator                *m_pGeometryCreator;             ///< The geometry creator
	CaloHitCreator                 *m_pCaloHitCreator;              ///< The calo hit creator
	TrackCreator                   *m_pTrackCreator;                ///< The track creator
	MCParticleCreator              *m_pMCParticleCreator;           ///< The mc particle creator
	PfoCreator                     *m_pPfoCreator;                  ///< The pfo creator

	Settings                        m_settings;                     ///< The settings for the arbor pfa processor
	GeometryCreator::Settings       m_geometryCreatorSettings;      ///< The geometry creator settings
	TrackCreator::Settings          m_trackCreatorSettings;         ///< The track creator settings
	CaloHitCreator::Settings        m_caloHitCreatorSettings;       ///< The calo hit creator settings
	MCParticleCreator::Settings     m_mcParticleCreatorSettings;    ///< The mc particle creator settings
	PfoCreator::Settings            m_pfoCreatorSettings;           ///< The pfo creator settings

	std::string                     m_detectorName;                 ///< The detector name
	unsigned int                    m_nRun;                         ///< The run number
	unsigned int                    m_nEvent;                       ///< The event number

// static FloatVector             m_sdhcalEnergyFactors;          ///< The sdhcal energy factor conversion for single hits
// static FloatVector             m_sdhcalEnergyConstants;        ///< The 9 parameters used by the sdhcal energy correction function
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline marlin::Processor *ArborPFAProcessor::newProcessor()
{
 return new ArborPFAProcessor;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline pandora::Pandora *ArborPFAProcessor::GetPandora()
{
	if (NULL == m_pPandora)
	{
		throw pandora::StatusCodeException(pandora::STATUS_CODE_NOT_INITIALIZED);
	}

	return m_pPandora;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline arbor::Arbor *ArborPFAProcessor::GetArbor()
{
	if (NULL == m_pArbor)
		throw pandora::StatusCodeException(pandora::STATUS_CODE_NOT_INITIALIZED);

	return m_pArbor;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline EVENT::LCEvent *ArborPFAProcessor::GetCurrentEvent()
{
	if (NULL == m_pLcioEvent)
		throw pandora::StatusCodeException(pandora::STATUS_CODE_NOT_INITIALIZED);

	return m_pLcioEvent;
}

#endif // #ifndef ARBOR_PFA_PROCESSOR_H
