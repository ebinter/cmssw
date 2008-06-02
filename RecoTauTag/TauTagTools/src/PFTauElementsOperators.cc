#include "RecoTauTag/TauTagTools/interface/PFTauElementsOperators.h"

 PFTauElementsOperators::PFTauElementsOperators(PFTau& thePFTau): TauElementsOperators(thePFTau),AreaMetric_recoElements_maxabsEta_(2.5){
   PFJetRef_=thePFTau.pfTauTagInfoRef()->pfjetRef();
   PFCands_=thePFTau.pfTauTagInfoRef()->PFCands();
   PFChargedHadrCands_=thePFTau.pfTauTagInfoRef()->PFChargedHadrCands();
   PFNeutrHadrCands_=thePFTau.pfTauTagInfoRef()->PFNeutrHadrCands();
   PFGammaCands_=thePFTau.pfTauTagInfoRef()->PFGammaCands();
   IsolPFCands_=thePFTau.isolationPFCands();
   IsolPFChargedHadrCands_=thePFTau.isolationPFChargedHadrCands();
   IsolPFNeutrHadrCands_=thePFTau.isolationPFNeutrHadrCands();
   IsolPFGammaCands_=thePFTau.isolationPFGammaCands();
   Tracks_=thePFTau.pfTauTagInfoRef()->Tracks();
}
void PFTauElementsOperators::setAreaMetricrecoElementsmaxabsEta( double x) {AreaMetric_recoElements_maxabsEta_=x;}

PFCandidateRefVector PFTauElementsOperators::PFCandsInCone(const PFCandidateRefVector thePFCands,const math::XYZVector myVector,const string conemetric,const double conesize,const double minPt)const{     
  PFCandidateRefVector theFilteredPFCands;
  for (PFCandidateRefVector::const_iterator iPFCand=thePFCands.begin();iPFCand!=thePFCands.end();++iPFCand) {
    if ((**iPFCand).pt()>minPt)theFilteredPFCands.push_back(*iPFCand);
  }  
  PFCandidateRefVector theFilteredPFCandsInCone;
  if (conemetric=="DR"){
    theFilteredPFCandsInCone=PFCandsinCone_DRmetric_(myVector,metricDR_,conesize,theFilteredPFCands);
  }else if(conemetric=="angle"){
    theFilteredPFCandsInCone=PFCandsinCone_Anglemetric_(myVector,metricAngle_,conesize,theFilteredPFCands);
  }else if(conemetric=="area"){
    int errorFlag=0;
    FixedAreaIsolationCone theFixedAreaCone;
    theFixedAreaCone.setAcceptanceLimit(AreaMetric_recoElements_maxabsEta_);
    double coneangle=theFixedAreaCone(myVector.theta(),myVector.phi(),0,conesize,errorFlag); 
    if (errorFlag!=0)return theFilteredPFCandsInCone;   
    theFilteredPFCandsInCone=PFCandsinCone_Anglemetric_(myVector,metricAngle_,coneangle,theFilteredPFCands);
  }else return PFCandidateRefVector();
  return theFilteredPFCandsInCone;
}
PFCandidateRefVector PFTauElementsOperators::PFCandsInCone(const math::XYZVector myVector,const string conemetric,const double conesize,const double minPt)const{     
  PFCandidateRefVector theFilteredPFCandsInCone=PFCandsInCone(PFCands_,myVector,conemetric,conesize,minPt);
  return theFilteredPFCandsInCone;
}
PFCandidateRefVector PFTauElementsOperators::PFChargedHadrCandsInCone(const math::XYZVector myVector,const string conemetric,const double conesize,const double minPt)const{     
  PFCandidateRefVector theFilteredPFCandsInCone=PFCandsInCone(PFChargedHadrCands_,myVector,conemetric,conesize,minPt);
  return theFilteredPFCandsInCone;
}
PFCandidateRefVector PFTauElementsOperators::PFChargedHadrCandsInCone(const math::XYZVector myVector,const string conemetric,const double conesize,const double minPt,const double PFChargedHadrCand_tracktorefpoint_maxDZ,const double refpoint_Z)const{     
  PFCandidateRefVector filteredPFChargedHadrCands;
  for(PFCandidateRefVector::const_iterator iPFCand=PFChargedHadrCands_.begin();iPFCand!=PFChargedHadrCands_.end();iPFCand++){
    TrackRef PFChargedHadrCand_track;
    if ((**iPFCand).block()->elements().size()!=0){
      for (OwnVector<PFBlockElement>::const_iterator iPFBlock=(**iPFCand).block()->elements().begin();iPFBlock!=(**iPFCand).block()->elements().end();iPFBlock++){
	if ((*iPFBlock).type()==PFBlockElement::TRACK && ROOT::Math::VectorUtil::DeltaR((**iPFCand).momentum(),(*iPFBlock).trackRef()->momentum())<0.001){
	  PFChargedHadrCand_track=(*iPFBlock).trackRef();
	}
      }
    }else continue;
    if (!PFChargedHadrCand_track)continue;
    if (fabs((*PFChargedHadrCand_track).dz()-refpoint_Z)<=PFChargedHadrCand_tracktorefpoint_maxDZ) filteredPFChargedHadrCands.push_back(*iPFCand);
  }
  PFCandidateRefVector theFilteredPFCandsInCone=PFCandsInCone(filteredPFChargedHadrCands,myVector,conemetric,conesize,minPt);
  return theFilteredPFCandsInCone;
}
PFCandidateRefVector PFTauElementsOperators::PFNeutrHadrCandsInCone(const math::XYZVector myVector,const string conemetric,const double conesize,const double minPt)const{     
  PFCandidateRefVector theFilteredPFCandsInCone=PFCandsInCone(PFNeutrHadrCands_,myVector,conemetric,conesize,minPt);
  return theFilteredPFCandsInCone;
}
 PFCandidateRefVector PFTauElementsOperators::PFGammaCandsInCone(const math::XYZVector myVector,const string conemetric,const double conesize,const double minPt)const{     
  PFCandidateRefVector theFilteredPFCandsInCone=PFCandsInCone(PFGammaCands_,myVector,conemetric,conesize,minPt);
  return theFilteredPFCandsInCone;
}
 PFCandidateRefVector PFTauElementsOperators::PFCandsInAnnulus(const PFCandidateRefVector thePFCands,const math::XYZVector myVector,const string innercone_metric,const double innercone_size,const string outercone_metric,const double outercone_size,const double minPt)const{     
  PFCandidateRefVector theFilteredPFCands;
  for (PFCandidateRefVector::const_iterator iPFCand=thePFCands.begin();iPFCand!=thePFCands.end();++iPFCand) {
    if ((**iPFCand).pt()>minPt)theFilteredPFCands.push_back(*iPFCand);
  }  
  PFCandidateRefVector theFilteredPFCandsInAnnulus;
  if (outercone_metric=="DR"){
    if (innercone_metric=="DR"){
      theFilteredPFCandsInAnnulus=PFCandsinAnnulus_innerDRouterDRmetrics_(myVector,metricDR_,innercone_size,metricDR_,outercone_size,theFilteredPFCands);
    }else if(innercone_metric=="angle"){
      theFilteredPFCandsInAnnulus=PFCandsinAnnulus_innerAngleouterDRmetrics_(myVector,metricAngle_,innercone_size,metricDR_,outercone_size,theFilteredPFCands);
    }else if(innercone_metric=="area"){
      int errorFlag=0;
      FixedAreaIsolationCone theFixedAreaSignalCone;
      theFixedAreaSignalCone.setAcceptanceLimit(AreaMetric_recoElements_maxabsEta_);
      double innercone_angle=theFixedAreaSignalCone(myVector.theta(),myVector.phi(),0,innercone_size,errorFlag);
      if (errorFlag!=0)return theFilteredPFCandsInAnnulus;
      theFilteredPFCandsInAnnulus=PFCandsinAnnulus_innerAngleouterDRmetrics_(myVector,metricAngle_,innercone_angle,metricDR_,outercone_size,theFilteredPFCands);
    }else return PFCandidateRefVector();
  }else if(outercone_metric=="angle"){
    if (innercone_metric=="DR"){
      theFilteredPFCandsInAnnulus=PFCandsinAnnulus_innerDRouterAnglemetrics_(myVector,metricDR_,innercone_size,metricAngle_,outercone_size,theFilteredPFCands);
    }else if(innercone_metric=="angle"){
      theFilteredPFCandsInAnnulus=PFCandsinAnnulus_innerAngleouterAnglemetrics_(myVector,metricAngle_,innercone_size,metricAngle_,outercone_size,theFilteredPFCands);
    }else if(innercone_metric=="area"){
      int errorFlag=0;
      FixedAreaIsolationCone theFixedAreaSignalCone;
      theFixedAreaSignalCone.setAcceptanceLimit(AreaMetric_recoElements_maxabsEta_);
      double innercone_angle=theFixedAreaSignalCone(myVector.theta(),myVector.phi(),0,innercone_size,errorFlag);
      if (errorFlag!=0)return theFilteredPFCandsInAnnulus;
      theFilteredPFCandsInAnnulus=PFCandsinAnnulus_innerAngleouterAnglemetrics_(myVector,metricAngle_,innercone_angle,metricAngle_,outercone_size,theFilteredPFCands);
    }else return PFCandidateRefVector();
  }else if(outercone_metric=="area"){
    int errorFlag=0;
    FixedAreaIsolationCone theFixedAreaSignalCone;
    theFixedAreaSignalCone.setAcceptanceLimit(AreaMetric_recoElements_maxabsEta_);
    if (innercone_metric=="DR"){
      // not implemented yet
    }else if(innercone_metric=="angle"){
      double outercone_angle=theFixedAreaSignalCone(myVector.theta(),myVector.phi(),innercone_size,outercone_size,errorFlag);    
      if (errorFlag!=0)return theFilteredPFCandsInAnnulus;
      theFilteredPFCandsInAnnulus=PFCandsinAnnulus_innerAngleouterAnglemetrics_(myVector,metricAngle_,innercone_size,metricAngle_,outercone_angle,theFilteredPFCands);
    }else if(innercone_metric=="area"){
      double innercone_angle=theFixedAreaSignalCone(myVector.theta(),myVector.phi(),0,innercone_size,errorFlag);    
      if (errorFlag!=0)return theFilteredPFCandsInAnnulus;
      double outercone_angle=theFixedAreaSignalCone(myVector.theta(),myVector.phi(),innercone_angle,outercone_size,errorFlag);
      if (errorFlag!=0)return theFilteredPFCandsInAnnulus;
      theFilteredPFCandsInAnnulus=PFCandsinAnnulus_innerAngleouterAnglemetrics_(myVector,metricAngle_,innercone_angle,metricAngle_,outercone_angle,theFilteredPFCands);
    }else return PFCandidateRefVector();
  }
  return theFilteredPFCandsInAnnulus;
}
 PFCandidateRefVector PFTauElementsOperators::PFCandsInAnnulus(const math::XYZVector myVector,const string innercone_metric,const double innercone_size,const string outercone_metric,const double outercone_size,const double minPt)const{     
  PFCandidateRefVector theFilteredPFCandsInAnnulus=PFCandsInAnnulus(PFCands_,myVector,innercone_metric,innercone_size,outercone_metric,outercone_size,minPt);
  return theFilteredPFCandsInAnnulus;
}
 PFCandidateRefVector PFTauElementsOperators::PFChargedHadrCandsInAnnulus(const math::XYZVector myVector,const string innercone_metric,const double innercone_size,const string outercone_metric,const double outercone_size,const double minPt)const{     
  PFCandidateRefVector theFilteredPFCandsInAnnulus=PFCandsInAnnulus(PFChargedHadrCands_,myVector,innercone_metric,innercone_size,outercone_metric,outercone_size,minPt);
  return theFilteredPFCandsInAnnulus;
}
PFCandidateRefVector PFTauElementsOperators::PFChargedHadrCandsInAnnulus(const math::XYZVector myVector,const string innercone_metric,const double innercone_size,const string outercone_metric,const double outercone_size,const double minPt,const double PFChargedHadrCand_tracktorefpoint_maxDZ,const double refpoint_Z)const{     
  PFCandidateRefVector filteredPFChargedHadrCands;
  for(PFCandidateRefVector::const_iterator iPFCand=PFChargedHadrCands_.begin();iPFCand!=PFChargedHadrCands_.end();iPFCand++){
    TrackRef PFChargedHadrCand_track;
    if ((**iPFCand).block()->elements().size()!=0){
      for (OwnVector<PFBlockElement>::const_iterator iPFBlock=(**iPFCand).block()->elements().begin();iPFBlock!=(**iPFCand).block()->elements().end();iPFBlock++){
	if ((*iPFBlock).type()==PFBlockElement::TRACK && ROOT::Math::VectorUtil::DeltaR((**iPFCand).momentum(),(*iPFBlock).trackRef()->momentum())<0.001){
	  PFChargedHadrCand_track=(*iPFBlock).trackRef();
	}
      }
    }else continue;
    if (!PFChargedHadrCand_track)continue;
    if (fabs((*PFChargedHadrCand_track).dz()-refpoint_Z)<=PFChargedHadrCand_tracktorefpoint_maxDZ) filteredPFChargedHadrCands.push_back(*iPFCand);
  }
  PFCandidateRefVector theFilteredPFCandsInAnnulus=PFCandsInAnnulus(filteredPFChargedHadrCands,myVector,innercone_metric,innercone_size,outercone_metric,outercone_size,minPt);
  return theFilteredPFCandsInAnnulus;
}
 PFCandidateRefVector PFTauElementsOperators::PFNeutrHadrCandsInAnnulus(const math::XYZVector myVector,const string innercone_metric,const double innercone_size,const string outercone_metric,const double outercone_size,const double minPt)const{     
  PFCandidateRefVector theFilteredPFCandsInAnnulus=PFCandsInAnnulus(PFNeutrHadrCands_,myVector,innercone_metric,innercone_size,outercone_metric,outercone_size,minPt);
  return theFilteredPFCandsInAnnulus;
}
 PFCandidateRefVector PFTauElementsOperators::PFGammaCandsInAnnulus(const math::XYZVector myVector,const string innercone_metric,const double innercone_size,const string outercone_metric,const double outercone_size,const double minPt)const{     
  PFCandidateRefVector theFilteredPFCandsInAnnulus=PFCandsInAnnulus(PFGammaCands_,myVector,innercone_metric,innercone_size,outercone_metric,outercone_size,minPt);
  return theFilteredPFCandsInAnnulus;
}
PFCandidateRef PFTauElementsOperators::leadPFCand(const string matchingcone_metric,const double matchingcone_size,const double minPt)const{
  PFCandidateRef myleadPFCand;
  if (!PFJetRef_) return myleadPFCand;
  math::XYZVector PFJet_XYZVector=(*PFJetRef_).momentum();
  const PFCandidateRefVector theFilteredPFCandsInCone=PFCandsInCone(PFJet_XYZVector,matchingcone_metric,matchingcone_size,minPt);
  double pt_cut=minPt;
  if (theFilteredPFCandsInCone.size()>0.){
    for(PFCandidateRefVector::const_iterator iPFCand =theFilteredPFCandsInCone.begin();iPFCand!=theFilteredPFCandsInCone.end();iPFCand++){
      if((*iPFCand)->pt()>pt_cut) {
	myleadPFCand=*iPFCand;
	pt_cut=(**iPFCand).pt();
      }
    }
  }
  return myleadPFCand;
}
PFCandidateRef PFTauElementsOperators::leadPFCand(const math::XYZVector myVector,const string matchingcone_metric,const double matchingcone_size,const double minPt)const{
  PFCandidateRef myleadPFCand;
  const PFCandidateRefVector theFilteredPFCandsInCone=PFCandsInCone(myVector,matchingcone_metric,matchingcone_size,minPt);
  double pt_cut=minPt;
  if (theFilteredPFCandsInCone.size()>0){
    for(PFCandidateRefVector::const_iterator iPFCand=theFilteredPFCandsInCone.begin();iPFCand!=theFilteredPFCandsInCone.end();iPFCand++){
      if((*iPFCand)->pt()>pt_cut) {
	myleadPFCand=*iPFCand;
	pt_cut=(**iPFCand).pt();
      }
    }
  }
  return myleadPFCand;
}
PFCandidateRef PFTauElementsOperators::leadPFChargedHadrCand(const string matchingcone_metric,const double matchingcone_size,const double minPt)const{
  PFCandidateRef myleadPFCand;
  if (!PFJetRef_) return myleadPFCand;
  math::XYZVector PFJet_XYZVector=(*PFJetRef_).momentum();
  const PFCandidateRefVector theFilteredPFCandsInCone=PFChargedHadrCandsInCone(PFJet_XYZVector,matchingcone_metric,matchingcone_size,minPt);
  double pt_cut=minPt;
  if (theFilteredPFCandsInCone.size()>0.){
    for(PFCandidateRefVector::const_iterator iPFCand =theFilteredPFCandsInCone.begin();iPFCand!=theFilteredPFCandsInCone.end();iPFCand++){
      if((*iPFCand)->pt()>pt_cut) {
	myleadPFCand=*iPFCand;
	pt_cut=(**iPFCand).pt();
      }
    }
  }
  return myleadPFCand;
}
PFCandidateRef PFTauElementsOperators::leadPFChargedHadrCand(const math::XYZVector myVector,const string matchingcone_metric,const double matchingcone_size,const double minPt)const{
  PFCandidateRef myleadPFCand;
  const PFCandidateRefVector theFilteredPFCandsInCone=PFChargedHadrCandsInCone(myVector,matchingcone_metric,matchingcone_size,minPt);
  double pt_cut=minPt;
  if (theFilteredPFCandsInCone.size()>0){
    for(PFCandidateRefVector::const_iterator iPFCand=theFilteredPFCandsInCone.begin();iPFCand!=theFilteredPFCandsInCone.end();iPFCand++){
      if((*iPFCand)->pt()>pt_cut) {
	myleadPFCand=*iPFCand;
	pt_cut=(**iPFCand).pt();
      }
    }
  }
  return myleadPFCand;
}
PFCandidateRef PFTauElementsOperators::leadPFNeutrHadrCand(const string matchingcone_metric,const double matchingcone_size,const double minPt)const{
  PFCandidateRef myleadPFCand;
  if (!PFJetRef_) return myleadPFCand;
  math::XYZVector PFJet_XYZVector=(*PFJetRef_).momentum();
  const PFCandidateRefVector theFilteredPFCandsInCone=PFNeutrHadrCandsInCone(PFJet_XYZVector,matchingcone_metric,matchingcone_size,minPt);
  double pt_cut=minPt;
  if (theFilteredPFCandsInCone.size()>0.){
    for(PFCandidateRefVector::const_iterator iPFCand =theFilteredPFCandsInCone.begin();iPFCand!=theFilteredPFCandsInCone.end();iPFCand++){
      if((*iPFCand)->pt()>pt_cut) {
	myleadPFCand=*iPFCand;
	pt_cut=(**iPFCand).pt();
      }
    }
  }
  return myleadPFCand;
}
PFCandidateRef PFTauElementsOperators::leadPFNeutrHadrCand(const math::XYZVector myVector,const string matchingcone_metric,const double matchingcone_size,const double minPt)const{
  PFCandidateRef myleadPFCand;
  const PFCandidateRefVector theFilteredPFCandsInCone=PFNeutrHadrCandsInCone(myVector,matchingcone_metric,matchingcone_size,minPt);
  double pt_cut=minPt;
  if (theFilteredPFCandsInCone.size()>0){
    for(PFCandidateRefVector::const_iterator iPFCand=theFilteredPFCandsInCone.begin();iPFCand!=theFilteredPFCandsInCone.end();iPFCand++){
      if((*iPFCand)->pt()>pt_cut) {
	myleadPFCand=*iPFCand;
	pt_cut=(**iPFCand).pt();
      }
    }
  }
  return myleadPFCand;
}
PFCandidateRef PFTauElementsOperators::leadPFGammaCand(const string matchingcone_metric,const double matchingcone_size,const double minPt)const{
  PFCandidateRef myleadPFCand;
  if (!PFJetRef_) return myleadPFCand;
  math::XYZVector PFJet_XYZVector=(*PFJetRef_).momentum();
  const PFCandidateRefVector theFilteredPFCandsInCone=PFGammaCandsInCone(PFJet_XYZVector,matchingcone_metric,matchingcone_size,minPt);
  double pt_cut=minPt;
  if (theFilteredPFCandsInCone.size()>0.){
    for(PFCandidateRefVector::const_iterator iPFCand =theFilteredPFCandsInCone.begin();iPFCand!=theFilteredPFCandsInCone.end();iPFCand++){
      if((*iPFCand)->pt()>pt_cut) {
	myleadPFCand=*iPFCand;
	pt_cut=(**iPFCand).pt();
      }
    }
  }
  return myleadPFCand;
}
PFCandidateRef PFTauElementsOperators::leadPFGammaCand(const math::XYZVector myVector,const string matchingcone_metric,const double matchingcone_size,const double minPt)const{
  PFCandidateRef myleadPFCand;
  const PFCandidateRefVector theFilteredPFCandsInCone=PFGammaCandsInCone(myVector,matchingcone_metric,matchingcone_size,minPt);
  double pt_cut=minPt;
  if (theFilteredPFCandsInCone.size()>0){
    for(PFCandidateRefVector::const_iterator iPFCand=theFilteredPFCandsInCone.begin();iPFCand!=theFilteredPFCandsInCone.end();iPFCand++){
      if((*iPFCand)->pt()>pt_cut) {
	myleadPFCand=*iPFCand;
	pt_cut=(**iPFCand).pt();
      }
    }
  }
  return myleadPFCand;
}
// ***
double PFTauElementsOperators::discriminatorByIsolPFCandsN(int IsolPFCands_maxN){
  double myDiscriminator=0.;
  if ((int)IsolPFCands_.size()<=IsolPFCands_maxN) myDiscriminator=1;
  return myDiscriminator;
}
double PFTauElementsOperators::discriminatorByIsolPFCandsN(string matchingcone_metric,double matchingcone_size,string signalcone_metric,double signalcone_size,string isolcone_metric,double isolcone_size,bool useOnlyChargedHadrforleadPFCand,double minPt_leadPFCand,double minPt_PFCand,int IsolPFCands_maxN){
  double myDiscriminator=0.;
  PFCandidateRef myleadPFCand;
  if (useOnlyChargedHadrforleadPFCand) myleadPFCand=leadPFChargedHadrCand(matchingcone_metric,matchingcone_size,minPt_leadPFCand);  
  else myleadPFCand=leadPFCand(matchingcone_metric,matchingcone_size,minPt_leadPFCand);  
  if(!myleadPFCand)return myDiscriminator;
  //if(signalcone_size>=isolcone_size) return 1.;
  math::XYZVector leadPFCand_XYZVector=(*myleadPFCand).momentum() ;
  const PFCandidateRefVector isolPFCands=PFCandsInAnnulus(leadPFCand_XYZVector,signalcone_metric,signalcone_size,isolcone_metric,isolcone_size,minPt_PFCand);   
  if ((int)isolPFCands.size()<=IsolPFCands_maxN) myDiscriminator=1;
  return myDiscriminator;
}
double PFTauElementsOperators::discriminatorByIsolPFCandsN(math::XYZVector myVector,string matchingcone_metric,double matchingcone_size,string signalcone_metric,double signalcone_size,string isolcone_metric,double isolcone_size,bool useOnlyChargedHadrforleadPFCand,double minPt_leadPFCand,double minPt_PFCand,int IsolPFCands_maxN){
  double myDiscriminator=0;
  PFCandidateRef myleadPFCand;
  if (useOnlyChargedHadrforleadPFCand) myleadPFCand=leadPFChargedHadrCand(myVector,matchingcone_metric,matchingcone_size,minPt_leadPFCand);  
  else myleadPFCand=leadPFCand(myVector,matchingcone_metric,matchingcone_size,minPt_leadPFCand); 
  if(!myleadPFCand)return myDiscriminator;
  //if(signalcone_size>=isolcone_size) return 1.;
  math::XYZVector leadPFCand_XYZVector=(*myleadPFCand).momentum() ;
  const PFCandidateRefVector isolPFCands=PFCandsInAnnulus(leadPFCand_XYZVector,signalcone_metric,signalcone_size,isolcone_metric,isolcone_size,minPt_PFCand);  
  if ((int)isolPFCands.size()<=IsolPFCands_maxN) myDiscriminator=1;
  return myDiscriminator;
}
double PFTauElementsOperators::discriminatorByIsolPFChargedHadrCandsN(int IsolPFChargedHadrCands_maxN){
  double myDiscriminator=0.;
  if ((int)IsolPFChargedHadrCands_.size()<=IsolPFChargedHadrCands_maxN) myDiscriminator=1;
  return myDiscriminator;
}
double PFTauElementsOperators::discriminatorByIsolPFChargedHadrCandsN(string matchingcone_metric,double matchingcone_size,string signalcone_metric,double signalcone_size,string isolcone_metric,double isolcone_size,bool useOnlyChargedHadrforleadPFCand,double minPt_leadPFCand,double minPt_PFCand,int IsolPFChargedHadrCands_maxN){
  double myDiscriminator=0.;
  PFCandidateRef myleadPFCand;
  if (useOnlyChargedHadrforleadPFCand) myleadPFCand=leadPFChargedHadrCand(matchingcone_metric,matchingcone_size,minPt_leadPFCand);  
  else myleadPFCand=leadPFCand(matchingcone_metric,matchingcone_size,minPt_leadPFCand);  
  if(!myleadPFCand)return myDiscriminator;
  //if(signalcone_size>=isolcone_size) return 1.;
  math::XYZVector leadPFCand_XYZVector=(*myleadPFCand).momentum() ;
  PFCandidateRefVector isolPFChargedHadrCands=PFChargedHadrCandsInAnnulus(leadPFCand_XYZVector,signalcone_metric,signalcone_size,isolcone_metric,isolcone_size,minPt_PFCand);   
  if ((int)isolPFChargedHadrCands.size()<=IsolPFChargedHadrCands_maxN) myDiscriminator=1;
  return myDiscriminator;
}
double PFTauElementsOperators::discriminatorByIsolPFChargedHadrCandsN(math::XYZVector myVector,string matchingcone_metric,double matchingcone_size,string signalcone_metric,double signalcone_size,string isolcone_metric,double isolcone_size,bool useOnlyChargedHadrforleadPFCand,double minPt_leadPFCand,double minPt_PFCand,int IsolPFChargedHadrCands_maxN){
  double myDiscriminator=0;
  PFCandidateRef myleadPFCand;
  if (useOnlyChargedHadrforleadPFCand) myleadPFCand=leadPFChargedHadrCand(myVector,matchingcone_metric,matchingcone_size,minPt_leadPFCand);  
  else myleadPFCand=leadPFCand(myVector,matchingcone_metric,matchingcone_size,minPt_leadPFCand); 
  if(!myleadPFCand)return myDiscriminator;
  //if(signalcone_size>=isolcone_size) return 1.;
  math::XYZVector leadPFCand_XYZVector=(*myleadPFCand).momentum() ;
   PFCandidateRefVector isolPFChargedHadrCands=PFChargedHadrCandsInAnnulus(leadPFCand_XYZVector,signalcone_metric,signalcone_size,isolcone_metric,isolcone_size,minPt_PFCand);  
  if ((int)isolPFChargedHadrCands.size()<=IsolPFChargedHadrCands_maxN) myDiscriminator=1;
  return myDiscriminator;
}
double PFTauElementsOperators::discriminatorByIsolPFNeutrHadrCandsN(int IsolPFNeutrHadrCands_maxN){
  double myDiscriminator=0.;
  if ((int)IsolPFNeutrHadrCands_.size()<=IsolPFNeutrHadrCands_maxN) myDiscriminator=1;
  return myDiscriminator;
}
double PFTauElementsOperators::discriminatorByIsolPFNeutrHadrCandsN(string matchingcone_metric,double matchingcone_size,string signalcone_metric,double signalcone_size,string isolcone_metric,double isolcone_size,bool useOnlyChargedHadrforleadPFCand,double minPt_leadPFCand,double minPt_PFCand,int IsolPFNeutrHadrCands_maxN){
  double myDiscriminator=0.;
  PFCandidateRef myleadPFCand;
  if (useOnlyChargedHadrforleadPFCand) myleadPFCand=leadPFChargedHadrCand(matchingcone_metric,matchingcone_size,minPt_leadPFCand);  
  else myleadPFCand=leadPFCand(matchingcone_metric,matchingcone_size,minPt_leadPFCand);  
  if(!myleadPFCand)return myDiscriminator;
  //if(signalcone_size>=isolcone_size) return 1.;
  math::XYZVector leadPFCand_XYZVector=(*myleadPFCand).momentum() ;
   PFCandidateRefVector isolPFNeutrHadrCands=PFNeutrHadrCandsInAnnulus(leadPFCand_XYZVector,signalcone_metric,signalcone_size,isolcone_metric,isolcone_size,minPt_PFCand);   
  if ((int)isolPFNeutrHadrCands.size()<=IsolPFNeutrHadrCands_maxN) myDiscriminator=1;
  return myDiscriminator;
}
double PFTauElementsOperators::discriminatorByIsolPFNeutrHadrCandsN(math::XYZVector myVector,string matchingcone_metric,double matchingcone_size,string signalcone_metric,double signalcone_size,string isolcone_metric,double isolcone_size,bool useOnlyChargedHadrforleadPFCand,double minPt_leadPFCand,double minPt_PFCand,int IsolPFNeutrHadrCands_maxN){
  double myDiscriminator=0;
  PFCandidateRef myleadPFCand;
  if (useOnlyChargedHadrforleadPFCand) myleadPFCand=leadPFChargedHadrCand(myVector,matchingcone_metric,matchingcone_size,minPt_leadPFCand);  
  else myleadPFCand=leadPFCand(myVector,matchingcone_metric,matchingcone_size,minPt_leadPFCand); 
  if(!myleadPFCand)return myDiscriminator;
  //if(signalcone_size>=isolcone_size) return 1.;
  math::XYZVector leadPFCand_XYZVector=(*myleadPFCand).momentum() ;
   PFCandidateRefVector isolPFNeutrHadrCands=PFNeutrHadrCandsInAnnulus(leadPFCand_XYZVector,signalcone_metric,signalcone_size,isolcone_metric,isolcone_size,minPt_PFCand);  
  if ((int)isolPFNeutrHadrCands.size()<=IsolPFNeutrHadrCands_maxN) myDiscriminator=1;
  return myDiscriminator;
}
double PFTauElementsOperators::discriminatorByIsolPFGammaCandsN(int IsolPFGammaCands_maxN){
  double myDiscriminator=0.;
  if ((int)IsolPFGammaCands_.size()<=IsolPFGammaCands_maxN) myDiscriminator=1;
  return myDiscriminator;
}
double PFTauElementsOperators::discriminatorByIsolPFGammaCandsN(string matchingcone_metric,double matchingcone_size,string signalcone_metric,double signalcone_size,string isolcone_metric,double isolcone_size,bool useOnlyChargedHadrforleadPFCand,double minPt_leadPFCand,double minPt_PFCand,int IsolPFGammaCands_maxN){
  double myDiscriminator=0.;
  PFCandidateRef myleadPFCand;
  if (useOnlyChargedHadrforleadPFCand) myleadPFCand=leadPFChargedHadrCand(matchingcone_metric,matchingcone_size,minPt_leadPFCand);  
  else myleadPFCand=leadPFCand(matchingcone_metric,matchingcone_size,minPt_leadPFCand);  
  if(!myleadPFCand)return myDiscriminator;
  //if(signalcone_size>=isolcone_size) return 1.;
  math::XYZVector leadPFCand_XYZVector=(*myleadPFCand).momentum() ;
   PFCandidateRefVector isolPFGammaCands=PFGammaCandsInAnnulus(leadPFCand_XYZVector,signalcone_metric,signalcone_size,isolcone_metric,isolcone_size,minPt_PFCand);   
  if ((int)isolPFGammaCands.size()<=IsolPFGammaCands_maxN) myDiscriminator=1;
  return myDiscriminator;
}
double PFTauElementsOperators::discriminatorByIsolPFGammaCandsN(math::XYZVector myVector,string matchingcone_metric,double matchingcone_size,string signalcone_metric,double signalcone_size,string isolcone_metric,double isolcone_size,bool useOnlyChargedHadrforleadPFCand,double minPt_leadPFCand,double minPt_PFCand,int IsolPFGammaCands_maxN){
  double myDiscriminator=0;
  PFCandidateRef myleadPFCand;
  if (useOnlyChargedHadrforleadPFCand) myleadPFCand=leadPFChargedHadrCand(myVector,matchingcone_metric,matchingcone_size,minPt_leadPFCand);  
  else myleadPFCand=leadPFCand(myVector,matchingcone_metric,matchingcone_size,minPt_leadPFCand); 
  if(!myleadPFCand)return myDiscriminator;
  //if(signalcone_size>=isolcone_size) return 1.;
  math::XYZVector leadPFCand_XYZVector=(*myleadPFCand).momentum() ;
   PFCandidateRefVector isolPFGammaCands=PFGammaCandsInAnnulus(leadPFCand_XYZVector,signalcone_metric,signalcone_size,isolcone_metric,isolcone_size,minPt_PFCand);  
  if ((int)isolPFGammaCands.size()<=IsolPFGammaCands_maxN) myDiscriminator=1;
  return myDiscriminator;
}
double PFTauElementsOperators::discriminatorByIsolPFCandsEtSum(double IsolPFCands_maxEtSum){
  double myIsolPFCandsEtSum=0.;
  double myDiscriminator=0.;
  for(PFCandidateRefVector::const_iterator iPFCand=IsolPFCands_.begin();iPFCand!=IsolPFCands_.end();iPFCand++) myIsolPFCandsEtSum+=(**iPFCand).et();
  if (myIsolPFCandsEtSum<=IsolPFCands_maxEtSum) myDiscriminator=1;
  return myDiscriminator;
}
double PFTauElementsOperators::discriminatorByIsolPFCandsEtSum(string matchingcone_metric,double matchingcone_size,string signalcone_metric,double signalcone_size,string isolcone_metric,double isolcone_size,bool useOnlyChargedHadrforleadPFCand,double minPt_leadPFCand,double minPt_PFCand,double IsolPFCands_maxEtSum){
  double myIsolPFCandsEtSum=0.;
  double myDiscriminator=0.;
  PFCandidateRef myleadPFCand;
  if (useOnlyChargedHadrforleadPFCand) myleadPFCand=leadPFChargedHadrCand(matchingcone_metric,matchingcone_size,minPt_leadPFCand);  
  else myleadPFCand=leadPFCand(matchingcone_metric,matchingcone_size,minPt_leadPFCand);  
  if(!myleadPFCand)return myDiscriminator;
  //if(signalcone_size>=isolcone_size) return 1.;
  math::XYZVector leadPFCand_XYZVector=(*myleadPFCand).momentum() ;
   PFCandidateRefVector isolPFCands=PFCandsInAnnulus(leadPFCand_XYZVector,signalcone_metric,signalcone_size,isolcone_metric,isolcone_size,minPt_PFCand);   
  for(PFCandidateRefVector::const_iterator iPFCand=isolPFCands.begin();iPFCand!=isolPFCands.end();iPFCand++) myIsolPFCandsEtSum+=(**iPFCand).et();
  if (myIsolPFCandsEtSum<=IsolPFCands_maxEtSum) myDiscriminator=1;
  return myDiscriminator;
}
double PFTauElementsOperators::discriminatorByIsolPFCandsEtSum(math::XYZVector myVector,string matchingcone_metric,double matchingcone_size,string signalcone_metric,double signalcone_size,string isolcone_metric,double isolcone_size,bool useOnlyChargedHadrforleadPFCand,double minPt_leadPFCand,double minPt_PFCand,double IsolPFCands_maxEtSum){
  double myIsolPFCandsEtSum=0.;
  double myDiscriminator=0;
  PFCandidateRef myleadPFCand;
  if (useOnlyChargedHadrforleadPFCand) myleadPFCand=leadPFChargedHadrCand(myVector,matchingcone_metric,matchingcone_size,minPt_leadPFCand);  
  else myleadPFCand=leadPFCand(myVector,matchingcone_metric,matchingcone_size,minPt_leadPFCand); 
  if(!myleadPFCand)return myDiscriminator;
  //if(signalcone_size>=isolcone_size) return 1.;
  math::XYZVector leadPFCand_XYZVector=(*myleadPFCand).momentum() ;
   PFCandidateRefVector isolPFCands=PFCandsInAnnulus(leadPFCand_XYZVector,signalcone_metric,signalcone_size,isolcone_metric,isolcone_size,minPt_PFCand);  
  for(PFCandidateRefVector::const_iterator iPFCand=isolPFCands.begin();iPFCand!=isolPFCands.end();iPFCand++) myIsolPFCandsEtSum+=(**iPFCand).et();
  if (myIsolPFCandsEtSum<=IsolPFCands_maxEtSum) myDiscriminator=1;
  return myDiscriminator;
}
double PFTauElementsOperators::discriminatorByIsolPFChargedHadrCandsEtSum(double IsolPFChargedHadrCands_maxEtSum){
  double myIsolPFCandsEtSum=0.;
  double myDiscriminator=0.;
  for(PFCandidateRefVector::const_iterator iPFCand=IsolPFChargedHadrCands_.begin();iPFCand!=IsolPFChargedHadrCands_.end();iPFCand++) myIsolPFCandsEtSum+=(**iPFCand).et();
  if (myIsolPFCandsEtSum<=IsolPFChargedHadrCands_maxEtSum) myDiscriminator=1;
  return myDiscriminator;
}
double PFTauElementsOperators::discriminatorByIsolPFChargedHadrCandsEtSum(string matchingcone_metric,double matchingcone_size,string signalcone_metric,double signalcone_size,string isolcone_metric,double isolcone_size,bool useOnlyChargedHadrforleadPFCand,double minPt_leadPFCand,double minPt_PFCand,double IsolPFChargedHadrCands_maxEtSum){
  double myIsolPFCandsEtSum=0.;
  double myDiscriminator=0.;
  PFCandidateRef myleadPFCand;
  if (useOnlyChargedHadrforleadPFCand) myleadPFCand=leadPFChargedHadrCand(matchingcone_metric,matchingcone_size,minPt_leadPFCand);  
  else myleadPFCand=leadPFCand(matchingcone_metric,matchingcone_size,minPt_leadPFCand);  
  if(!myleadPFCand)return myDiscriminator;
  //if(signalcone_size>=isolcone_size) return 1.;
  math::XYZVector leadPFCand_XYZVector=(*myleadPFCand).momentum() ;
   PFCandidateRefVector isolPFChargedHadrCands=PFChargedHadrCandsInAnnulus(leadPFCand_XYZVector,signalcone_metric,signalcone_size,isolcone_metric,isolcone_size,minPt_PFCand);   
  for(PFCandidateRefVector::const_iterator iPFCand=isolPFChargedHadrCands.begin();iPFCand!=isolPFChargedHadrCands.end();iPFCand++) myIsolPFCandsEtSum+=(**iPFCand).et();
  if (myIsolPFCandsEtSum<=IsolPFChargedHadrCands_maxEtSum) myDiscriminator=1;
  return myDiscriminator;
}
double PFTauElementsOperators::discriminatorByIsolPFChargedHadrCandsEtSum(math::XYZVector myVector,string matchingcone_metric,double matchingcone_size,string signalcone_metric,double signalcone_size,string isolcone_metric,double isolcone_size,bool useOnlyChargedHadrforleadPFCand,double minPt_leadPFCand,double minPt_PFCand,double IsolPFChargedHadrCands_maxEtSum){
  double myIsolPFCandsEtSum=0.;
  double myDiscriminator=0;
  PFCandidateRef myleadPFCand;
  if (useOnlyChargedHadrforleadPFCand) myleadPFCand=leadPFChargedHadrCand(myVector,matchingcone_metric,matchingcone_size,minPt_leadPFCand);  
  else myleadPFCand=leadPFCand(myVector,matchingcone_metric,matchingcone_size,minPt_leadPFCand); 
  if(!myleadPFCand)return myDiscriminator;
  //if(signalcone_size>=isolcone_size) return 1.;
  math::XYZVector leadPFCand_XYZVector=(*myleadPFCand).momentum() ;
   PFCandidateRefVector isolPFChargedHadrCands=PFChargedHadrCandsInAnnulus(leadPFCand_XYZVector,signalcone_metric,signalcone_size,isolcone_metric,isolcone_size,minPt_PFCand);  
  for(PFCandidateRefVector::const_iterator iPFCand=isolPFChargedHadrCands.begin();iPFCand!=isolPFChargedHadrCands.end();iPFCand++) myIsolPFCandsEtSum+=(**iPFCand).et();
  if (myIsolPFCandsEtSum<=IsolPFChargedHadrCands_maxEtSum) myDiscriminator=1;
  return myDiscriminator;
}
double PFTauElementsOperators::discriminatorByIsolPFNeutrHadrCandsEtSum(double IsolPFNeutrHadrCands_maxEtSum){
  double myIsolPFCandsEtSum=0.;
  double myDiscriminator=0.;
  for(PFCandidateRefVector::const_iterator iPFCand=IsolPFNeutrHadrCands_.begin();iPFCand!=IsolPFNeutrHadrCands_.end();iPFCand++) myIsolPFCandsEtSum+=(**iPFCand).et();
  if (myIsolPFCandsEtSum<=IsolPFNeutrHadrCands_maxEtSum) myDiscriminator=1;
  return myDiscriminator;
}
double PFTauElementsOperators::discriminatorByIsolPFNeutrHadrCandsEtSum(string matchingcone_metric,double matchingcone_size,string signalcone_metric,double signalcone_size,string isolcone_metric,double isolcone_size,bool useOnlyChargedHadrforleadPFCand,double minPt_leadPFCand,double minPt_PFCand,double IsolPFNeutrHadrCands_maxEtSum){
  double myIsolPFCandsEtSum=0.;
  double myDiscriminator=0.;
  PFCandidateRef myleadPFCand;
  if (useOnlyChargedHadrforleadPFCand) myleadPFCand=leadPFChargedHadrCand(matchingcone_metric,matchingcone_size,minPt_leadPFCand);  
  else myleadPFCand=leadPFCand(matchingcone_metric,matchingcone_size,minPt_leadPFCand);  
  if(!myleadPFCand)return myDiscriminator;
  //if(signalcone_size>=isolcone_size) return 1.;
  math::XYZVector leadPFCand_XYZVector=(*myleadPFCand).momentum() ;
   PFCandidateRefVector isolPFNeutrHadrCands=PFNeutrHadrCandsInAnnulus(leadPFCand_XYZVector,signalcone_metric,signalcone_size,isolcone_metric,isolcone_size,minPt_PFCand);   
  for(PFCandidateRefVector::const_iterator iPFCand=isolPFNeutrHadrCands.begin();iPFCand!=isolPFNeutrHadrCands.end();iPFCand++) myIsolPFCandsEtSum+=(**iPFCand).et();
  if (myIsolPFCandsEtSum<=IsolPFNeutrHadrCands_maxEtSum) myDiscriminator=1;
  return myDiscriminator;
}
double PFTauElementsOperators::discriminatorByIsolPFNeutrHadrCandsEtSum(math::XYZVector myVector,string matchingcone_metric,double matchingcone_size,string signalcone_metric,double signalcone_size,string isolcone_metric,double isolcone_size,bool useOnlyChargedHadrforleadPFCand,double minPt_leadPFCand,double minPt_PFCand,double IsolPFNeutrHadrCands_maxEtSum){
  double myIsolPFCandsEtSum=0.;
  double myDiscriminator=0;
  PFCandidateRef myleadPFCand;
  if (useOnlyChargedHadrforleadPFCand) myleadPFCand=leadPFChargedHadrCand(myVector,matchingcone_metric,matchingcone_size,minPt_leadPFCand);  
  else myleadPFCand=leadPFCand(myVector,matchingcone_metric,matchingcone_size,minPt_leadPFCand); 
  if(!myleadPFCand)return myDiscriminator;
  //if(signalcone_size>=isolcone_size) return 1.;
  math::XYZVector leadPFCand_XYZVector=(*myleadPFCand).momentum() ;
   PFCandidateRefVector isolPFNeutrHadrCands=PFNeutrHadrCandsInAnnulus(leadPFCand_XYZVector,signalcone_metric,signalcone_size,isolcone_metric,isolcone_size,minPt_PFCand);  
  for(PFCandidateRefVector::const_iterator iPFCand=isolPFNeutrHadrCands.begin();iPFCand!=isolPFNeutrHadrCands.end();iPFCand++) myIsolPFCandsEtSum+=(**iPFCand).et();
  if (myIsolPFCandsEtSum<=IsolPFNeutrHadrCands_maxEtSum) myDiscriminator=1;
  return myDiscriminator;
}
double PFTauElementsOperators::discriminatorByIsolPFGammaCandsEtSum(double IsolPFGammaCands_maxEtSum){
  double myIsolPFCandsEtSum=0.;
  double myDiscriminator=0.;
  for(PFCandidateRefVector::const_iterator iPFCand=IsolPFGammaCands_.begin();iPFCand!=IsolPFGammaCands_.end();iPFCand++) myIsolPFCandsEtSum+=(**iPFCand).et();
  if (myIsolPFCandsEtSum<=IsolPFGammaCands_maxEtSum) myDiscriminator=1;
  return myDiscriminator;
}
double PFTauElementsOperators::discriminatorByIsolPFGammaCandsEtSum(string matchingcone_metric,double matchingcone_size,string signalcone_metric,double signalcone_size,string isolcone_metric,double isolcone_size,bool useOnlyChargedHadrforleadPFCand,double minPt_leadPFCand,double minPt_PFCand,double IsolPFGammaCands_maxEtSum){
  double myIsolPFCandsEtSum=0.;
  double myDiscriminator=0.;
  PFCandidateRef myleadPFCand;
  if (useOnlyChargedHadrforleadPFCand) myleadPFCand=leadPFChargedHadrCand(matchingcone_metric,matchingcone_size,minPt_leadPFCand);  
  else myleadPFCand=leadPFCand(matchingcone_metric,matchingcone_size,minPt_leadPFCand);  
  if(!myleadPFCand)return myDiscriminator;
  if(signalcone_size>=isolcone_size) return 1.;
  math::XYZVector leadPFCand_XYZVector=(*myleadPFCand).momentum() ;
   PFCandidateRefVector isolPFGammaCands=PFGammaCandsInAnnulus(leadPFCand_XYZVector,signalcone_metric,signalcone_size,isolcone_metric,isolcone_size,minPt_PFCand);   
  for(PFCandidateRefVector::const_iterator iPFCand=isolPFGammaCands.begin();iPFCand!=isolPFGammaCands.end();iPFCand++) myIsolPFCandsEtSum+=(**iPFCand).et();
  if (myIsolPFCandsEtSum<=IsolPFGammaCands_maxEtSum) myDiscriminator=1;
  return myDiscriminator;
}
double PFTauElementsOperators::discriminatorByIsolPFGammaCandsEtSum(math::XYZVector myVector,string matchingcone_metric,double matchingcone_size,string signalcone_metric,double signalcone_size,string isolcone_metric,double isolcone_size,bool useOnlyChargedHadrforleadPFCand,double minPt_leadPFCand,double minPt_PFCand,double IsolPFGammaCands_maxEtSum){
  double myIsolPFCandsEtSum=0.;
  double myDiscriminator=0;
  PFCandidateRef myleadPFCand;
  if (useOnlyChargedHadrforleadPFCand) myleadPFCand=leadPFChargedHadrCand(myVector,matchingcone_metric,matchingcone_size,minPt_leadPFCand);  
  else myleadPFCand=leadPFCand(myVector,matchingcone_metric,matchingcone_size,minPt_leadPFCand); 
  if(!myleadPFCand)return myDiscriminator;
  //if(signalcone_size>=isolcone_size) return 1.;
  math::XYZVector leadPFCand_XYZVector=(*myleadPFCand).momentum() ;
   PFCandidateRefVector isolPFGammaCands=PFGammaCandsInAnnulus(leadPFCand_XYZVector,signalcone_metric,signalcone_size,isolcone_metric,isolcone_size,minPt_PFCand);  
  for(PFCandidateRefVector::const_iterator iPFCand=isolPFGammaCands.begin();iPFCand!=isolPFGammaCands.end();iPFCand++) myIsolPFCandsEtSum+=(**iPFCand).et();
  if (myIsolPFCandsEtSum<=IsolPFGammaCands_maxEtSum) myDiscriminator=1;
  return myDiscriminator;
}
