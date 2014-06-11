#include "SimG4Core/Application/interface/RunManagerMTWorker.h"
#include "SimG4Core/Application/interface/G4SimEvent.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/Handle.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"

#include "G4Event.hh"

RunManagerMTWorker::RunManagerMTWorker(const edm::ParameterSet& iConfig):
  m_generator(iConfig.getParameter<edm::ParameterSet>("Generator")),
  m_InTag(iConfig.getParameter<edm::ParameterSet>("Generator").getParameter<std::string>("HepMCProductLabel"))
{}

RunManagerMTWorker::~RunManagerMTWorker() {}

void RunManagerMTWorker::produce(const edm::Event& inpevt, const edm::EventSetup& es) {
  m_currentEvent.reset(generateEvent(inpevt));
}

G4Event * RunManagerMTWorker::generateEvent(const edm::Event& inpevt) {
  m_currentEvent.reset();
  m_simEvent.reset();

  G4Event * evt = new G4Event(inpevt.id().event());
  edm::Handle<edm::HepMCProduct> HepMCEvt;

  inpevt.getByLabel(m_InTag, HepMCEvt);

  m_generator.setGenEvent(HepMCEvt->GetEvent());

  // STUFF MISSING

  return evt;
}

