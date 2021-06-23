
#include "GeneratorInterface/Core/interface/GeneratorFilter.h"
#include "GeneratorInterface/ExternalDecays/interface/ExternalDecayDriver.h"

#include "GeneratorInterface/Pythia8Interface/interface/Py8GunBase.h"

namespace gen {

class Py8EGunV2 : public Py8GunBase {

   public:

      Py8EGunV2( edm::ParameterSet const& );
      ~Py8EGunV2() {}

      bool generatePartonsAndHadronize() override;
      const char* classname() const override;

   private:

      // PtGun particle(s) characteristics
      double  fMinEta;
      double  fMaxEta;
      double  fMinE ;
      double  fMaxE ;
      double  fMinMass ;
      double  fMaxMass ;
      double  fMinCTau ;
      double  fMaxCTau ;
      bool    fAddAntiParticle;

};

// implementation
//
Py8EGunV2::Py8EGunV2( edm::ParameterSet const& ps )
   : Py8GunBase(ps)
{

   // ParameterSet defpset ;
   edm::ParameterSet pgun_params =
      ps.getParameter<edm::ParameterSet>("PGunParameters"); // , defpset ) ;
   fMinEta     = pgun_params.getParameter<double>("MinEta"); // ,-2.2);
   fMaxEta     = pgun_params.getParameter<double>("MaxEta"); // , 2.2);
   fMinE       = pgun_params.getParameter<double>("MinE"); // ,  0.);
   fMaxE       = pgun_params.getParameter<double>("MaxE"); // ,  0.);
   fMinMass    = pgun_params.getParameter<double>("MinMass"); // ,  0.);
   fMaxMass    = pgun_params.getParameter<double>("MaxMass"); // ,  0.);
   fMinCTau    = pgun_params.getParameter<double>("MinCTau"); // ,  0.);
   fMaxCTau    = pgun_params.getParameter<double>("MaxCTau"); // ,  0.);
   fAddAntiParticle = pgun_params.getParameter<bool>("AddAntiParticle"); //, false) ;

}

bool Py8EGunV2::generatePartonsAndHadronize()
{

   fMasterGen->event.reset();

   // Define lifetime [mm/c]
   // All particle guns in event get the same proper lifetime ctau
   //double tauTmp = (12. - 2.e-5)*randomEngine().flat() + 2.e-5; // mm/c
   double tauTmp = (fMaxCTau-fMinCTau)*randomEngine().flat() + fMinCTau; // flat lifetime

   for ( size_t i=0; i<fPartIDs.size(); i++ )
   {

      int particleID = fPartIDs[i]; // this is PDG - need to convert to Py8 ???

      // Define energy [GeV]
      double ee = fMaxE; // fixed E
      /*
      //double ee = (fMaxE-fMinE) * randomEngine().flat() + fMinE; // flat E
      */

      // Define energy [GeV]
      double mass = (fMaxMass-fMinMass) * randomEngine().flat() + fMinMass; // flat mass
      /*
      //double mass = fMaxMass; // fixed mass
      //double mass = log(1. + (randomEngine().flat())*(exp(fMaxMass)-1.) ); // exp growth in mass with upper bound
      //double mass = (fMasterGen->particleData).m0( particleID ); // use SM default
      */

      // Calculate angles
      double phi = (fMaxPhi-fMinPhi) * randomEngine().flat() + fMinPhi;
      double eta = (fMaxEta-fMinEta) * randomEngine().flat() + fMinEta;
      double the = 2.*atan(exp(-eta));

      // Calculate momenta
      double pp = sqrt( ee*ee - mass*mass );
      double px = pp * sin(the) * cos(phi);
      double py = pp * sin(the) * sin(phi);
      double pz = pp * cos(the);

      if ( !((fMasterGen->particleData).isParticle( particleID )) )
      {
         particleID = std::fabs(particleID) ;
      }

      if( 1<= fabs(particleID) && fabs(particleID) <= 6) // quarks
        (fMasterGen->event).append( particleID, 23, 101, 0, px, py, pz, ee, mass );
      else if (fabs(particleID) == 21)                   // gluons
        (fMasterGen->event).append( 21, 23, 101, 102, px, py, pz, ee, mass );
      else                                               // other
        (fMasterGen->event).append( particleID, 1, 0, 0, px, py, pz, ee, mass );

      // Set proper lifetime of this particle
      // NOTE: distinction b/w nominal proper lifetime (tau0) parameter
      // and actual particle's proper lifetime (tau) depends on implementation here.
      int eventSize = (fMasterGen->event).size()-1;
      // -log(flat) = exponential distribution.
      // See: https://stats.stackexchange.com/questions/234544/from-uniform-distribution-to-exponential-distribution-and-vice-versa
      //double tauTmp = -(fMasterGen->event)[eventSize].tau0() * log(randomEngine().flat());
      //double tauTmp = (fMasterGen->event)[eventSize].tau0();
      //double tauTmp = (110. - 2.e-5)*randomEngine().flat() + 2.e-5;
      (fMasterGen->event)[eventSize].tau( tauTmp );
      std::cout << "tau0:" << tauTmp << std::endl;

      // Here also need to add anti-particle (if any)
      // otherwise just add a 2nd particle of the same type
      // (for example, gamma)
      if ( fAddAntiParticle )
      {

        if( 1 <= fabs(particleID) && fabs(particleID) <= 6){ // quarks
          (fMasterGen->event).append( -particleID, 23, 0, 101, -px, -py, -pz, ee, mass );
        }
        else if (fabs(particleID) == 21){ // gluons
          (fMasterGen->event).append( 21, 23, 102, 101, -px, -py, -pz, ee, mass );
        }
        else if ( (fMasterGen->particleData).isParticle( -particleID ) ){
          (fMasterGen->event).append( -particleID, 1, 0, 0, -px, -py, -pz, ee, mass );
        }
        else {
          (fMasterGen->event).append( particleID, 1, 0, 0, -px, -py, -pz, ee, mass );
        }

        // Set proper lifetime of this anti-particle
        eventSize = (fMasterGen->event).size()-1;
        // -log(flat) = exponential distribution
        //tauTmp = -(fMasterGen->event)[eventSize].tau0() * log(randomEngine().flat());
        //tauTmp = (fMasterGen->event)[eventSize].tau0();
        (fMasterGen->event)[eventSize].tau( tauTmp );

      } // antiparticle

   } // fPartIDs

   if ( !fMasterGen->next() ) return false;

   event().reset(new HepMC::GenEvent);
   return toHepMC.fill_next_event( fMasterGen->event, event().get() );

} // generatePartonsAndHadronize()

const char* Py8EGunV2::classname() const
{
   return "Py8EGunV2";
}

typedef edm::GeneratorFilter<gen::Py8EGunV2, gen::ExternalDecayDriver> Pythia8EGunV2;

} // end namespace

using gen::Pythia8EGunV2;
DEFINE_FWK_MODULE(Pythia8EGunV2);
