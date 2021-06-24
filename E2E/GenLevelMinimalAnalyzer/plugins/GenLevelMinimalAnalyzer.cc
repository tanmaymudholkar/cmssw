// -*- C++ -*-
//
// Package:    E2E/GenLevelMinimalAnalyzer
// Class:      GenLevelMinimalAnalyzer
//
/**\class GenLevelMinimalAnalyzer GenLevelMinimalAnalyzer.cc E2E/GenLevelMinimalAnalyzer/plugins/GenLevelMinimalAnalyzer.cc

   Description: [one line class summary]

   Implementation:
   [Notes on implementation]
*/
//
// Original Author:  Tanmay Mudholkar
//         Created:  Thu, 24 Jun 2021 02:16:11 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"


#include "TH1.h"
#include "TH1D.h"

//
// class declaration
//

class GenLevelMinimalAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
public:
  explicit GenLevelMinimalAnalyzer(const edm::ParameterSet&);
  ~GenLevelMinimalAnalyzer();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


private:
  virtual void beginJob() override;
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;

  // ----------member data ---------------------------
  edm::EDGetTokenT<reco::GenParticleCollection> genParticlesT_;  //used to select what tracks to read from configuration file
  TH1D * h_deltaR;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
GenLevelMinimalAnalyzer::GenLevelMinimalAnalyzer(const edm::ParameterSet& iConfig)
  : genParticlesT_(consumes<reco::GenParticleCollection>(iConfig.getUntrackedParameter<edm::InputTag>("genParticlesSrc"))) {

  //now do what ever initialization is needed
  usesResource("TFileService");
  edm::Service<TFileService> fs;
  h_deltaR = fs->make<TH1D>("deltaR" , "deltaR" , 100 , 0. , 0.1 );
}


GenLevelMinimalAnalyzer::~GenLevelMinimalAnalyzer()
{
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ method called for each event  ------------
void
GenLevelMinimalAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // using namespace edm;

  // Handle<TrackCollection> tracks;
  edm::Handle<reco::GenParticleCollection> genParticles;
  iEvent.getByToken(genParticlesT_, genParticles);

  for (unsigned int genParticlesCounter = 0; genParticlesCounter < genParticles->size(); ++genParticlesCounter) {
    reco::GenParticleRef gen_particle(genParticles, genParticlesCounter);
    // std::cout << "Found particle with pdgId: " << gen_particle->pdgId() << std::endl;
    if ( std::abs(gen_particle->pdgId()) != 111 ) continue;
    assert(gen_particle->numberOfDaughters() == 2);
    double deltaR_daughters = reco::deltaR(gen_particle->daughter(0)->eta(), gen_particle->daughter(0)->phi(), gen_particle->daughter(1)->eta(), gen_particle->daughter(1)->phi());
    // std::cout << "deltaR between daughters: " << deltaR_daughters << std::endl;
    h_deltaR->Fill(deltaR_daughters);
  }

// #ifdef THIS_IS_AN_EVENT_EXAMPLE
//   Handle<ExampleData> pIn;
//   iEvent.getByLabel("example",pIn);
// #endif

// #ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
//   ESHandle<SetupData> pSetup;
//   iSetup.get<SetupRecord>().get(pSetup);
// #endif
}


// ------------ method called once each job just before starting event loop  ------------
void
GenLevelMinimalAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
GenLevelMinimalAnalyzer::endJob()
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
GenLevelMinimalAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  // edm::ParameterSetDescription desc;
  // desc.setUnknown();
  // descriptions.addDefault(desc);

  //Specify that only 'tracks' is allowed
  //To use, remove the default given above and uncomment below
  edm::ParameterSetDescription desc;
  desc.addUntracked<edm::InputTag>("genParticlesSrc");
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(GenLevelMinimalAnalyzer);
