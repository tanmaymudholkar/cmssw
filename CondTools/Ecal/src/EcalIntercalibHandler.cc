#include "CondTools/Ecal/interface/EcalIntercalibHandler.h"
#include "FWCore/ParameterSet/interface/ParameterSetfwd.h"
#include "CondFormats/EcalObjects/interface/EcalIntercalibConstants.h"
#include "CondTools/Ecal/interface/EcalFloatCondObjectContainerXMLTranslator.h"

#include<iostream>

popcon::EcalIntercalibHandler::EcalIntercalibHandler(const edm::ParameterSet & ps)
  :    m_name(ps.getUntrackedParameter<std::string>("name","EcalIntercalibHandler")) {

  std::cout << "EcalIntercalib Source handler constructor\n" << std::endl;
  m_firstRun = static_cast<unsigned int>(atoi( ps.getParameter<std::string>("firstRun").c_str()));
  m_file_highfield = ps.getParameter<std::string>("InputFile");
}

popcon::EcalIntercalibHandler::~EcalIntercalibHandler() {}

void popcon::EcalIntercalibHandler::getNewObjects() {

  std::cout << "------- Ecal - > getNewObjects\n";

  std::ostringstream ss; 
  ss<<"ECAL ";

  unsigned int irun =  m_firstRun;
  
  std::cout << "Generating popcon record for run " << irun << "..." << std::flush;
  std::string file_= m_file_highfield;
  std::cout << "going to open file "<<file_ << std::flush;

  EcalCondHeader   header;
  EcalIntercalibConstants * payload = new EcalIntercalibConstants;
  EcalFloatCondObjectContainerXMLTranslator::readXML(file_,header,*payload);
  Time_t snc= (Time_t) irun ;
	    
  popcon::PopConSourceHandler<EcalIntercalibConstants>::m_to_transfer.push_back(
    std::make_pair(payload,snc));
	    
  ss << "Run=" << irun << "_Magnet_changed_"<<std::endl; 
  m_userTextLog = ss.str()+";";
	    
  std::cout << "Ecal - > end of getNewObjects -----------\n";
}
