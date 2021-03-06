/**
 *  @file   ArborPFA/src/ArborPFAProcessor.cc
 * 
 *  @brief  Implementation of the arbor pfa processor class.
 * 
 *  $Log: $
 */

// -- marlin headers
#include "marlin/Global.h"
#include "marlin/Exceptions.h"

// -- gear headers
#include "gear/BField.h"

// -- pandora sdk headers
#include "Api/PandoraApi.h"

// -- fine granularity content headers (pandora)
#include "FineGranularityContent.h"

// -- arbor headers
#include "arborpfa/arbor/Arbor.h"
#include "arborpfa/api/ArborApi.h"

// -- local classes headers
#include "ExternalClusteringAlgorithm.h"
#include "ArborPFAProcessor.h"
#include "SimpleBFieldCalculator.h"

// -- std headers
#include <cstdlib>


ArborPFAProcessor aArborPFAProcessor;


pandora::Pandora *ArborPFAProcessor::m_pPandora   = NULL;
arbor::Arbor     *ArborPFAProcessor::m_pArbor     = NULL;
EVENT::LCEvent   *ArborPFAProcessor::m_pLcioEvent = NULL;
//FloatVector       ArborPFAProcessor::m_sdhcalEnergyFactors   = FloatVector();
//FloatVector       ArborPFAProcessor::m_sdhcalEnergyConstants = FloatVector();

//------------------------------------------------------------------------------------------------------------------------------------------

ArborPFAProcessor::ArborPFAProcessor() :
    Processor("ArborPFAProcessor"),
    m_pGeometryCreator(NULL),
    m_pCaloHitCreator(NULL),
    m_pTrackCreator(NULL),
    m_pMCParticleCreator(NULL),
    m_pPfoCreator(NULL),
    m_nRun(0),
    m_nEvent(0)
{
    _description = "Arbor reconstructs clusters and particle flow objects using oriented tree topology algorithms";
    this->ProcessSteeringFile();
}

//------------------------------------------------------------------------------------------------------------------------------------------

void ArborPFAProcessor::init()
{
	try
	{
		streamlog_out(MESSAGE) << "ArborPFAProcessor - Init" << std::endl;
		this->FinaliseSteeringParameters();

		m_pArbor = new arbor::Arbor();
		m_pPandora = m_pArbor->GetPandora();

		m_pGeometryCreator = new GeometryCreator(m_geometryCreatorSettings);
		m_pCaloHitCreator = new CaloHitCreator(m_caloHitCreatorSettings);
		m_pTrackCreator = new TrackCreator(m_trackCreatorSettings);
		m_pMCParticleCreator = new MCParticleCreator(m_mcParticleCreatorSettings);
		m_pPfoCreator = new PfoCreator(m_pfoCreatorSettings);

		PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, this->RegisterUserComponents());
		PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, m_pGeometryCreator->CreateGeometry());
		PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, PandoraApi::ReadSettings(*m_pPandora, m_settings.m_pandoraSettingsXmlFile));
	}
	catch (pandora::StatusCodeException &statusCodeException)
	{
		streamlog_out(ERROR) << "Failed to initialize marlin arbor: " << statusCodeException.ToString() << std::endl;
		throw statusCodeException;
	}
	catch (gear::Exception &exception)
	{
		streamlog_out(ERROR) << "Failed to initialize marlin arbor: gear exception " << exception.what() << std::endl;
		throw exception;
	}
	catch (...)
	{
		streamlog_out(ERROR) << "Failed to initialize marlin arbor: unrecognized exception" << std::endl;
		throw;
	}
}

//------------------------------------------------------------------------------------------------------------------------------------------

void ArborPFAProcessor::processRunHeader(LCRunHeader *pLCRunHeader)
{
    m_detectorName = pLCRunHeader->getDetectorName();
    streamlog_out(DEBUG5) << "Detector Name " << m_detectorName << ", Run " << ++m_nRun <<  std::endl;
}

//------------------------------------------------------------------------------------------------------------------------------------------

void ArborPFAProcessor::processEvent(LCEvent *pLCEvent)
{
	static int eventCounter = 0;
	m_pLcioEvent = pLCEvent;

	if (eventCounter < m_settings.m_nEventsToSkip)
	{
		++eventCounter;
		throw marlin::SkipEventException(this);
	}

	try
	{
		streamlog_out(DEBUG5) << "ArborPFAProcessor, Run " << m_nRun << ", Event " << ++m_nEvent << std::endl;

		// create input objects
		PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, m_pMCParticleCreator->CreateMCParticles(pLCEvent));
		PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, m_pTrackCreator->CreateTrackAssociations(pLCEvent));
		PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, m_pTrackCreator->CreateTracks(pLCEvent));
		PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, m_pMCParticleCreator->CreateTrackToMCParticleRelationships(pLCEvent));
		PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, m_pCaloHitCreator->CreateCaloHits(pLCEvent));
		PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, m_pMCParticleCreator->CreateCaloHitToMCParticleRelationships(pLCEvent));

		// prepare and process event
  PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, arbor::ArborApi::PrepareEvent(*m_pArbor));
		PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, PandoraApi::ProcessEvent(*m_pPandora));

		// write the output in event interface
		PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, m_pPfoCreator->CreateParticleFlowObjects(pLCEvent));

		// reset the Api
		PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, arbor::ArborApi::Reset(*m_pArbor));
		PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, PandoraApi::Reset(*m_pPandora));
		this->Reset();
	}
	catch (pandora::StatusCodeException &statusCodeException)
	{
		streamlog_out(ERROR) << "Marlin arbor failed to process event: " << statusCodeException.ToString() << std::endl;
		throw statusCodeException;
	}
	catch (gear::Exception &exception)
	{
		streamlog_out(ERROR) << "Marlin arbor failed to process event: gear exception " << exception.what() << std::endl;
		throw exception;
	}
	catch (EVENT::Exception &exception)
	{
		streamlog_out(ERROR) << "Marlin arbor failed to process event: lcio exception " << exception.what() << std::endl;
		throw exception;
	}
	catch (...)
	{
		streamlog_out(ERROR) << "Marlin arbor failed to process event: unrecognized exception" << std::endl;
		throw;
	}
}

//------------------------------------------------------------------------------------------------------------------------------------------

void ArborPFAProcessor::check(LCEvent */*pLCEvent*/)
{
    streamlog_out(DEBUG) << "ArborPFAProcessor - Check" << std::endl;
}

//------------------------------------------------------------------------------------------------------------------------------------------

void ArborPFAProcessor::end()
{
    delete m_pArbor;
    delete m_pGeometryCreator;
    delete m_pCaloHitCreator;
    delete m_pTrackCreator;
    delete m_pMCParticleCreator;
    delete m_pPfoCreator;

    streamlog_out(MESSAGE) << "ArborPFAProcessor - End" << std::endl;
}

//------------------------------------------------------------------------------------------------------------------------------------------

pandora::StatusCode ArborPFAProcessor::RegisterUserComponents() const
{
    // Register content from external pandora libraries
    PANDORA_RETURN_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, PandoraApi::SetPseudoLayerCalculator(*m_pPandora,
        new FineGranularityPseudoLayerCalculator()));

    PANDORA_RETURN_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, PandoraApi::SetShowerProfileCalculator(*m_pPandora,
        new FineGranularityShowerProfileCalculator()));

    PANDORA_RETURN_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, FineGranularityContent::RegisterAlgorithms(*m_pPandora));
    PANDORA_RETURN_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, FineGranularityContent::RegisterHelperFunctions(*m_pPandora));

    // register arbor algorithms and plugins
				PANDORA_RETURN_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, arbor::ArborApi::RegisterArborAlgorithms(*m_pArbor));
				PANDORA_RETURN_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, arbor::ArborApi::RegisterArborPlugins(*m_pArbor));

//				// energy correction for sdhcal
//    PANDORA_RETURN_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, PandoraApi::RegisterEnergyCorrectionFunction(*m_pPandora,
//        "SDHCALEnergyCorrection", pandora::HADRONIC, &ArborPFAProcessor::SDHCALEnergyCorrectionFunction));

    // Register BField calculator
    PANDORA_RETURN_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, PandoraApi::SetBFieldCalculator(*m_pPandora,
        new SimpleBFieldCalculator()));

    // External algorithm that convert lcio clusters in pandora clusters
    PANDORA_RETURN_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, PandoraApi::RegisterAlgorithmFactory(*m_pPandora, "ExternalClustering",
        new ExternalClusteringAlgorithm::Factory));

    return pandora::STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------

void ArborPFAProcessor::ProcessSteeringFile()
{
    registerProcessorParameter("PandoraSettingsXmlFile",
                            "The pandora settings xml file",
                            m_settings.m_pandoraSettingsXmlFile,
                            std::string());

    // Input collections
    registerInputCollections(LCIO::TRACK,
                            "TrackCollections", 
                            "Names of the Track collections used for clustering",
                            m_trackCreatorSettings.m_trackCollections,
                            StringVector());

    registerInputCollections(LCIO::VERTEX,
                            "KinkVertexCollections", 
                            "Name of external kink Vertex collections",
                            m_trackCreatorSettings.m_kinkVertexCollections,
                            StringVector());

    registerInputCollections(LCIO::VERTEX,
                            "ProngVertexCollections", 
                            "Name of external prong Vertex collections",
                            m_trackCreatorSettings.m_prongVertexCollections,
                            StringVector());

    registerInputCollections(LCIO::VERTEX,
                            "SplitVertexCollections", 
                            "Name of external split Vertex collections",
                            m_trackCreatorSettings.m_splitVertexCollections,
                            StringVector());

    registerInputCollections(LCIO::VERTEX,
                            "V0VertexCollections", 
                            "Name of external V0 Vertex collections",
                            m_trackCreatorSettings.m_v0VertexCollections,
                            StringVector());

    registerInputCollections(LCIO::CALORIMETERHIT,
                            "ECalCaloHitCollections", 
                            "Name of the ECAL calo hit collections",
                            m_caloHitCreatorSettings.m_eCalCaloHitCollections,
                            StringVector());

    registerInputCollections(LCIO::CALORIMETERHIT,
                            "HCalCaloHitCollections", 
                            "Name of the HCAL calo hit collections",
                            m_caloHitCreatorSettings.m_hCalCaloHitCollections,
                            StringVector());

    registerInputCollections(LCIO::CALORIMETERHIT,
                            "LCalCaloHitCollections", 
                            "Name of the LCAL calo hit collections",
                            m_caloHitCreatorSettings.m_lCalCaloHitCollections,
                            StringVector());

    registerInputCollections(LCIO::CALORIMETERHIT,
                            "LHCalCaloHitCollections", 
                            "Name of the LHCAL calo hit collections",
                            m_caloHitCreatorSettings.m_lHCalCaloHitCollections,
                            StringVector());

    registerInputCollections(LCIO::CALORIMETERHIT,
                            "MuonCaloHitCollections", 
                            "Name of the muon calo hit collections",
                            m_caloHitCreatorSettings.m_muonCaloHitCollections,
                            StringVector());

    registerInputCollections(LCIO::MCPARTICLE,
                            "MCParticleCollections", 
                            "Name of mc particle collections",
                            m_mcParticleCreatorSettings.m_mcParticleCollections,
                            StringVector());

    registerInputCollections(LCIO::LCRELATION, 
                            "RelCaloHitCollections",
                            "SimCaloHit to CaloHit Relations Collection Name",
                            m_mcParticleCreatorSettings.m_lcCaloHitRelationCollections,
                            StringVector());

    registerInputCollections(LCIO::LCRELATION, 
                            "RelTrackCollections",
                            "Track to MCParticle Relations Collection Name",
                            m_mcParticleCreatorSettings.m_lcTrackRelationCollections,
                            StringVector());

    // Absorber properties
    registerProcessorParameter("AbsorberRadiationLength",
                            "The absorber radation length",
                            m_geometryCreatorSettings.m_absorberRadiationLength,
                            float(1.));

    registerProcessorParameter("AbsorberInteractionLength",
                            "The absorber interaction length",
                            m_geometryCreatorSettings.m_absorberInteractionLength,
                            float(1.));

    // Name of PFO collection written by MarlinArbor
    registerOutputCollection( LCIO::CLUSTER,
                              "ClusterCollectionName",
                              "Cluster Collection Name",
                              m_pfoCreatorSettings.m_clusterCollectionName,
                              std::string("ArborPFAClusters"));

    registerOutputCollection( LCIO::RECONSTRUCTEDPARTICLE,
                              "PFOCollectionName",
                              "PFO Collection Name",
                              m_pfoCreatorSettings.m_pfoCollectionName,
                              std::string("ArborPFAPFOs"));

    registerOutputCollection( LCIO::LCGENERICOBJECT,
                              "ReclusterMonitoringCollectionName",
                              "Recluster Monitoring Collection Name",
                              m_pfoCreatorSettings.m_reclusterMonitoringCollectionName,
                              std::string("ArborPFAReclusterMonitoring"));

    registerOutputCollection( LCIO::LCRELATION,
                              "ReclusterRelationsCollectionName",
                              "Recluster Relations Collection Name",
                              m_pfoCreatorSettings.m_reclusterRelationsCollectionName,
                              std::string("ArborPFAReclusterRelations"));

    // Calibration constants
    registerProcessorParameter("ECalToMipCalibration",
                            "The calibration from deposited ECal energy to mip",
                            m_caloHitCreatorSettings.m_eCalToMip,
                            float(1.));

    registerProcessorParameter("HCalToMipCalibration",
                            "The calibration from deposited HCal energy to mip",
                            m_caloHitCreatorSettings.m_hCalToMip,
                            float(1.));

    registerProcessorParameter("ECalMipThreshold",
                            "Threshold for creating calo hits in the ECal, units mip",
                            m_caloHitCreatorSettings.m_eCalMipThreshold,
                            float(0.));

    registerProcessorParameter("MuonToMipCalibration",
                            "The calibration from deposited Muon energy to mip",
                            m_caloHitCreatorSettings.m_muonToMip,
                            float(10.));

    registerProcessorParameter("HCalMipThreshold",
                            "Threshold for creating calo hits in the HCal, units mip",
                            m_caloHitCreatorSettings.m_hCalMipThreshold,
                            float(0.));

    registerProcessorParameter("ECalToEMGeVCalibration",
                            "The calibration from deposited ECal energy to EM energy",
                            m_caloHitCreatorSettings.m_eCalToEMGeV,
                            float(1.));

    registerProcessorParameter("HCalToEMGeVCalibration",
                            "The calibration from deposited HCal energy to EM energy",
                            m_caloHitCreatorSettings.m_hCalToEMGeV,
                            float(1.));

    registerProcessorParameter("ECalToHadGeVCalibrationEndCap",
                            "The calibration from deposited ECal energy to hadronic energy",
                            m_caloHitCreatorSettings.m_eCalToHadGeVEndCap,
                            float(1.));

    registerProcessorParameter("ECalToHadGeVCalibrationBarrel",
                            "The calibration from deposited ECal energy to hadronic energy",
                            m_caloHitCreatorSettings.m_eCalToHadGeVBarrel,
                            float(1.));

    registerProcessorParameter("HCalToHadGeVCalibration",
                            "The calibration from deposited HCal energy to hadronic energy",
                            m_caloHitCreatorSettings.m_hCalToHadGeV,
                            float(1.));

    registerProcessorParameter("DigitalMuonHits",
                            "Treat muon hits as digital",
                            m_caloHitCreatorSettings.m_muonDigitalHits,
                            int(1));

    registerProcessorParameter("MuonHitEnergy",
                            "The energy for a digital muon calorimeter hit, units GeV",
                            m_caloHitCreatorSettings.m_muonHitEnergy,
                            float(0.5));

    registerProcessorParameter("MaxHCalHitHadronicEnergy",
                            "The maximum hadronic energy allowed for a single hcal hit",
                            m_caloHitCreatorSettings.m_maxHCalHitHadronicEnergy,
                            float(1.));

    registerProcessorParameter("NOuterSamplingLayers",
                            "Number of layers from edge for hit to be flagged as an outer layer hit",
                            m_caloHitCreatorSettings.m_nOuterSamplingLayers,
                            int(3));

    registerProcessorParameter("LayersFromEdgeMaxRearDistance",
                            "Maximum number of layers from candidate outer layer hit to rear of detector",
                            m_caloHitCreatorSettings.m_layersFromEdgeMaxRearDistance,
                            float(250.f));

    // B-field parameters
    registerProcessorParameter("MuonBarrelBField",
                            "The bfield in the muon barrel, units Tesla",
                            SimpleBFieldCalculator::m_muonBarrelBField,
                            float(-1.5f));

    registerProcessorParameter("MuonEndCapBField",
                            "The bfield in the muon endcap, units Tesla",
                            SimpleBFieldCalculator::m_muonEndCapBField,
                            float(0.01f));

    // Track relationship parameters
    registerProcessorParameter("ShouldFormTrackRelationships",
                            "Whether to form pandora track relationships using v0 and kink info",
                            m_trackCreatorSettings.m_shouldFormTrackRelationships,
                            int(1));

    // Initial track hit specifications
   registerProcessorParameter("MinTrackHits",
                            "Track quality cut: the minimum number of track hits",
                            m_trackCreatorSettings.m_minTrackHits,
                            int(5));

   registerProcessorParameter("MinFtdTrackHits",
                            "Track quality cut: the minimum number of ftd track hits for ftd only tracks",
                            m_trackCreatorSettings.m_minFtdTrackHits,
                            int(0));

   registerProcessorParameter("MaxTrackHits",
                            "Track quality cut: the maximum number of track hits",
                            m_trackCreatorSettings.m_maxTrackHits,
                            int(5000));

   registerProcessorParameter("UseOldTrackStateCalculation",
                            "Whether to calculate track states manually, rather than copy stored fitter values",
                            m_trackCreatorSettings.m_useOldTrackStateCalculation,
                            int(0));

    // Track PFO usage parameters
    registerProcessorParameter("D0TrackCut",
                            "Track d0 cut used to determine whether track can be used to form pfo",
                            m_trackCreatorSettings.m_d0TrackCut,
                            float(50.));

    registerProcessorParameter("Z0TrackCut",
                            "Track z0 cut used to determine whether track can be used to form pfo",
                            m_trackCreatorSettings.m_z0TrackCut,
                            float(50.));

    registerProcessorParameter("UseNonVertexTracks",
                            "Whether can form pfos from tracks that don't start at vertex",
                            m_trackCreatorSettings.m_usingNonVertexTracks,
                            int(1.));

    registerProcessorParameter("UseUnmatchedNonVertexTracks",
                            "Whether can form pfos from unmatched tracks that don't start at vertex",
                            m_trackCreatorSettings.m_usingUnmatchedNonVertexTracks,
                            int(0.));

    registerProcessorParameter("UseUnmatchedVertexTracks",
                            "Whether can form pfos from unmatched tracks that start at vertex",
                            m_trackCreatorSettings.m_usingUnmatchedVertexTracks,
                            int(1.));

    registerProcessorParameter("UnmatchedVertexTrackMaxEnergy",
                            "Maximum energy for unmatched vertex track",
                            m_trackCreatorSettings.m_unmatchedVertexTrackMaxEnergy,
                            float(5.));

    registerProcessorParameter("D0UnmatchedVertexTrackCut",
                            "d0 cut used to determine whether unmatched vertex track can form pfo",
                            m_trackCreatorSettings.m_d0UnmatchedVertexTrackCut,
                            float(5.));

    registerProcessorParameter("Z0UnmatchedVertexTrackCut",
                            "z0 cut used to determine whether unmatched vertex track can form pfo",
                            m_trackCreatorSettings.m_z0UnmatchedVertexTrackCut,
                            float(5.));

    registerProcessorParameter("ZCutForNonVertexTracks",
                            "Non vtx track z cut to determine whether track can be used to form pfo",
                            m_trackCreatorSettings.m_zCutForNonVertexTracks,
                            float(250.));

    // Track "reaches ecal" parameters
    registerProcessorParameter("ReachesECalNTpcHits",
                            "Minimum number of tpc hits to consider track as reaching ecal",
                            m_trackCreatorSettings.m_reachesECalNTpcHits,
                            int(11));

    registerProcessorParameter("ReachesECalNFtdHits",
                            "Minimum number of ftd hits to consider track as reaching ecal",
                            m_trackCreatorSettings.m_reachesECalNFtdHits,
                            int(4));

    registerProcessorParameter("ReachesECalTpcOuterDistance",
                            "Max distance from track to tpc r max to id whether track reaches ecal",
                            m_trackCreatorSettings.m_reachesECalTpcOuterDistance,
                            float(-100.));

    registerProcessorParameter("ReachesECalMinFtdLayer",
                            "Min FTD layer for track to be considered to have reached ecal",
                            m_trackCreatorSettings.m_reachesECalMinFtdLayer,
                            int(9));

    registerProcessorParameter("ReachesECalTpcZMaxDistance",
                            "Max distance from track to tpc z max to id whether track reaches ecal",
                            m_trackCreatorSettings.m_reachesECalTpcZMaxDistance,
                            float(-50.));

    registerProcessorParameter("ReachesECalFtdZMaxDistance",
                            "Max distance from track hit to ftd z position to identify ftd hits",
                            m_trackCreatorSettings.m_reachesECalFtdZMaxDistance,
                            float(1.));

    registerProcessorParameter("CurvatureToMomentumFactor",
                            "Constant relating track curvature in b field to momentum",
                            m_trackCreatorSettings.m_curvatureToMomentumFactor,
                            float(0.3 / 2000.));

    registerProcessorParameter("MinTrackECalDistanceFromIp",
                            "Sanity check on separation between ip and track projected ecal position",
                            m_trackCreatorSettings.m_minTrackECalDistanceFromIp,
                            float(100.));

    // Final track quality parameters
    registerProcessorParameter("MaxTrackSigmaPOverP",
                            "Cut on fractional track momentum error",
                            m_trackCreatorSettings.m_maxTrackSigmaPOverP,
                            float(0.15));

    registerProcessorParameter("MinMomentumForTrackHitChecks",
                            "Min track momentum required to perform final quality checks on number of hits",
                            m_trackCreatorSettings.m_minMomentumForTrackHitChecks,
                            float(1.));

    registerProcessorParameter("TpcMembraneMaxZ",
                            "Tpc membrane max z coordinate",
                            m_trackCreatorSettings.m_tpcMembraneMaxZ,
                            float(10.));

    registerProcessorParameter("MinTpcHitFractionOfExpected",
                            "Cut on fractional of expected number of TPC hits",
                            m_trackCreatorSettings.m_minTpcHitFractionOfExpected,
                            float(0.20));

    registerProcessorParameter("MinFtdHitsForTpcHitFraction",
                            "Cut on minimum number of FTD hits of TPC hit fraction to be applied",
                            m_trackCreatorSettings.m_minFtdHitsForTpcHitFraction,
                            int(2));

    registerProcessorParameter("MaxTpcInnerRDistance",
                            "Track cut on distance from tpc inner r to id whether track can form pfo",
                            m_trackCreatorSettings.m_maxTpcInnerRDistance,
                            float(50.));

    // Additional geometry parameters
    registerProcessorParameter("ECalEndCapInnerSymmetryOrder",
                            "ECal end cap inner symmetry order (missing from ILD gear files)",
                            m_geometryCreatorSettings.m_eCalEndCapInnerSymmetryOrder,
                            int(4));

    registerProcessorParameter("ECalEndCapInnerPhiCoordinate",
                            "ECal end cap inner phi coordinate (missing from ILD gear files)",
                            m_geometryCreatorSettings.m_eCalEndCapInnerPhiCoordinate,
                            float(0.));

    registerProcessorParameter("ECalEndCapOuterSymmetryOrder",
                            "ECal end cap outer symmetry order (missing from ILD gear files)",
                            m_geometryCreatorSettings.m_eCalEndCapOuterSymmetryOrder,
                            int(8));

    registerProcessorParameter("ECalEndCapOuterPhiCoordinate",
                            "ECal end cap outer phi coordinate (missing from ILD gear files)",
                            m_geometryCreatorSettings.m_eCalEndCapOuterPhiCoordinate,
                            float(0.));

    registerProcessorParameter("HCalEndCapInnerSymmetryOrder",
                            "HCal end cap inner symmetry order (missing from ILD gear files)",
                            m_geometryCreatorSettings.m_hCalEndCapInnerSymmetryOrder,
                            int(4));

    registerProcessorParameter("HCalEndCapInnerPhiCoordinate",
                            "HCal end cap inner phi coordinate (missing from ILD gear files)",
                            m_geometryCreatorSettings.m_hCalEndCapInnerPhiCoordinate,
                            float(0.));

    registerProcessorParameter("HCalEndCapOuterSymmetryOrder",
                            "HCal end cap outer symmetry order (missing from ILD gear files)",
                            m_geometryCreatorSettings.m_hCalEndCapOuterSymmetryOrder,
                            int(16));

    registerProcessorParameter("HCalEndCapOuterPhiCoordinate",
                            "HCal end cap outer phi coordinate (missing from ILD gear files)",
                            m_geometryCreatorSettings.m_hCalEndCapOuterPhiCoordinate,
                            float(0.));

    // Number of events to skip
    registerProcessorParameter("NEventsToSkip",
                            "Number of events to skip at start of reconstruction job",
                            m_settings.m_nEventsToSkip,
                            int(0));

    // For Strip Splitting method and also for hybrid ECAL
    registerProcessorParameter("StripSplittingOn",
                            "To use strip splitting algorithm, this should be true",
                            m_caloHitCreatorSettings.m_stripSplittingOn,
                            bool(false));

    // For Strip Splitting method and also for hybrid ECAL
    registerProcessorParameter("UseEcalScLayers",
                            "To use scintillator layers ~ hybrid ECAL, this should be true",
                            m_caloHitCreatorSettings.m_useEcalScLayers,
                            bool(false));

    // Parameters for hybrid ECAL
    // Energy to MIP for Si-layers and Sc-layers, respectively.
    //Si
    registerProcessorParameter("ECalSiToMipCalibration",
                            "The calibration from deposited Si-layer energy to mip",
                            m_caloHitCreatorSettings.m_eCalSiToMip,
                            float(1.));

    //Sc
    registerProcessorParameter("ECalScToMipCalibration",
                            "The calibration from deposited Sc-layer energy to mip",
                            m_caloHitCreatorSettings.m_eCalScToMip,
                            float(1.));

    // MipThreshold for Si-layers and Sc-layers, respectively.
    // Si
    registerProcessorParameter("ECalSiMipThreshold",
                            "Threshold for creating calo hits in the Si-layers of ECAL, units mip",
                            m_caloHitCreatorSettings.m_eCalSiMipThreshold,
                            float(0.));

    //Sc
    registerProcessorParameter("ECalScMipThreshold",
                            "Threshold for creating calo hits in the Sc-layers of ECAL, units mip",
                            m_caloHitCreatorSettings.m_eCalScMipThreshold,
                            float(0.));

    // EcalToEM for Si-layers and Sc-layers, respectively.
    //Si
    registerProcessorParameter("ECalSiToEMGeVCalibration",
                            "The calibration from deposited Si-layer energy to EM energy",
                            m_caloHitCreatorSettings.m_eCalSiToEMGeV,
                            float(1.));

    //Sc
    registerProcessorParameter("ECalScToEMGeVCalibration",
                            "The calibration from deposited Sc-layer energy to EM energy",
                            m_caloHitCreatorSettings.m_eCalScToEMGeV,
                            float(1.));

    // EcalToHad for Si-layers and Sc-layers of the endcaps, respectively.
    //Si
    registerProcessorParameter("ECalSiToHadGeVCalibrationEndCap",
                            "The calibration from deposited Si-layer energy on the enecaps to hadronic energy",
                            m_caloHitCreatorSettings.m_eCalSiToHadGeVEndCap,
                            float(1.));

    //Sc
    registerProcessorParameter("ECalScToHadGeVCalibrationEndCap",
                            "The calibration from deposited Sc-layer energy on the endcaps to hadronic energy",
                            m_caloHitCreatorSettings.m_eCalScToHadGeVEndCap,
                            float(1.));

    // EcalToHad for Si-layers and Sc-layers of the barrel, respectively.
    //Si
    registerProcessorParameter("ECalSiToHadGeVCalibrationBarrel",
                            "The calibration from deposited Si-layer energy on the barrel to hadronic energy",
                            m_caloHitCreatorSettings.m_eCalSiToHadGeVBarrel,
                            float(1.));

    //Sc
    registerProcessorParameter("ECalScToHadGeVCalibrationBarrel",
                            "The calibration from deposited Sc-layer energy to the barrel hadronic energy",
                            m_caloHitCreatorSettings.m_eCalScToHadGeVBarrel,
                            float(1.));
}

//------------------------------------------------------------------------------------------------------------------------------------------

void ArborPFAProcessor::FinaliseSteeringParameters()
{
    // ATTN: This function seems to be necessary for operations that cannot easily be performed at construction of the processor,
    // when the steering file is parsed e.g. the call to GEAR to get the inner bfield
    m_caloHitCreatorSettings.m_absorberRadiationLength = m_geometryCreatorSettings.m_absorberRadiationLength;
    m_caloHitCreatorSettings.m_absorberInteractionLength = m_geometryCreatorSettings.m_absorberInteractionLength;
    m_caloHitCreatorSettings.m_hCalEndCapInnerSymmetryOrder = m_geometryCreatorSettings.m_hCalEndCapInnerSymmetryOrder;
    m_caloHitCreatorSettings.m_hCalEndCapInnerPhiCoordinate = m_geometryCreatorSettings.m_hCalEndCapInnerPhiCoordinate;

    m_trackCreatorSettings.m_prongSplitVertexCollections = m_trackCreatorSettings.m_prongVertexCollections;
    m_trackCreatorSettings.m_prongSplitVertexCollections.insert(m_trackCreatorSettings.m_prongSplitVertexCollections.end(),
        m_trackCreatorSettings.m_splitVertexCollections.begin(), m_trackCreatorSettings.m_splitVertexCollections.end());

    SimpleBFieldCalculator::m_innerBField = marlin::Global::GEAR->getBField().at(gear::Vector3D(0., 0., 0.)).z();
}

//------------------------------------------------------------------------------------------------------------------------------------------

void ArborPFAProcessor::Reset()
{
    m_pCaloHitCreator->Reset();
    m_pTrackCreator->Reset();
}

//------------------------------------------------------------------------------------------------------------------------------------------

//void ArborPFAProcessor::SDHCALEnergyCorrectionFunction(const pandora::Cluster *const pCluster, float &energyCorrection)
//{
//	if(NULL == pCluster)
//		throw pandora::StatusCodeException(pandora::STATUS_CODE_INVALID_PARAMETER);
//
//	if(0 == pCluster->GetNCaloHits())
//	{
//		energyCorrection = 0.f;
//		return;
//	}
//
//	pandora::CaloHitList clusterCaloHitList;
//	pCluster->GetOrderedCaloHitList().GetCaloHitList(clusterCaloHitList);
//
//	unsigned int N(caloHitList.size());
//	unsigned int N1(0);
//	unsigned int N2(0);
//	unsigned int N3(0);
//
//	float alpha = m_sdhcalEnergyConstants.at(0) + m_sdhcalEnergyConstants.at(1)*N + m_sdhcalEnergyConstants.at(2) *N*N;
//	float beta  = m_sdhcalEnergyConstants.at(3) + m_sdhcalEnergyConstants.at(4)*N + m_sdhcalEnergyConstants.at(5) *N*N;
//	float gamma = m_sdhcalEnergyConstants.at(6) + m_sdhcalEnergyConstants.at(7)*N + m_sdhcalEnergyConstants.at(8) *N*N;
//
//	for(pandora::CaloHitList::const_iterator iter = caloHitList.begin() , endIter = caloHitList.end() ; endIter != iter ; ++iter)
//	{
//		const pandora::CaloHit *pCaloHit = *iter;
//
//		if( (pCaloHit->GetInputEnergy() - m_sdhcalEnergyFactors.at(0)) < std::numeric_limits<float>::epsilon() )
//			N1++;
//		else if((pCaloHit->GetInputEnergy() - m_sdhcalEnergyFactors.at(1)) < std::numeric_limits<float>::epsilon())
//			N2++;
//		else if((pCaloHit->GetInputEnergy() - m_sdhcalEnergyFactors.at(2)) < std::numeric_limits<float>::epsilon())
//			N3++;
//		else
//			throw pandora::StatusCodeException(pandora::STATUS_CODE_FAILURE);
//	}
//
//	energyCorrection = alpha*N1 + beta*N2 + gamma*N3;
//}

