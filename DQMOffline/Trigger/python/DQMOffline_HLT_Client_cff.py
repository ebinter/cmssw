import FWCore.ParameterSet.Config as cms

#from DQM.HLTEvF.HLTEventInfoClient_cfi import *

#from DQMOffline.Trigger.GeneralHLTOfflineClient_cff import *
from DQMOffline.Trigger.EgHLTOfflineClient_cfi import *
from DQMOffline.Trigger.MuonPostProcessor_cff import *
#from DQMOffline.Trigger.BPAGPostProcessor_cff import *
from DQMOffline.Trigger.JetMETHLTOfflineClient_cfi import *
#from DQMOffline.Trigger.TnPEfficiencyPostProcessor_cff import *
from DQMOffline.Trigger.HLTTauPostProcessor_cfi import *
from DQMOffline.Trigger.DQMOffline_HLT_Cert_cff import *
from DQMOffline.Trigger.HLTInclusiveVBFClient_cfi import *
from DQMOffline.Trigger.FSQHLTOfflineClient_cfi import  *
from DQMOffline.Trigger.HILowLumiHLTOfflineClient_cfi import  *

from DQMOffline.Trigger.TrackingMonitoring_Client_cff import *
from DQMOffline.Trigger.TrackingMonitoringPA_Client_cff import *

from DQMOffline.Trigger.ExoticaMonitoring_Client_cff import *
<<<<<<< HEAD
from DQMOffline.Trigger.SusyMonitoring_Client_cff import *
from DQMOffline.Trigger.B2GMonitoring_Client_cff import *
from DQMOffline.Trigger.BPhysicsMonitoring_Client_cff import *
from DQMOffline.Trigger.HiggsMonitoring_Client_cff import *
from DQMOffline.Trigger.StandardModelMonitoring_Client_cff import *
from DQMOffline.Trigger.TopMonitoring_Client_cff import *

from DQMOffline.Trigger.BTaggingMonitoring_Client_cff import *
=======
from DQMOffline.Trigger.BPHMonitoring_Client_cff import *
>>>>>>> mydev1

hltOfflineDQMClient = cms.Sequence(
#    hltGeneralSeqClient *
    egHLTOffDQMClient *
    hltMuonPostVal *
    jetMETHLTOfflineClient *
    fsqClient *
    HiJetClient * 
    #tagAndProbeEfficiencyPostProcessor *
    HLTTauPostSeq *
    dqmOfflineHLTCert *
    hltInclusiveVBFClient *
    exoticaClient *
<<<<<<< HEAD
    susyClient *
    b2gClient *
    higgsClient *
    smpClient *
    topClient *
    btaggingClient
=======
    bphClient
>>>>>>> mydev1
    )

