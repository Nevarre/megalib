/* 
 * SimRewriter.cxx
 *
 *
 * Copyright (C) by Andreas Zoglauer.
 * All rights reserved.
 *
 *
 * This code implementation is the intellectual property of
 * Andreas Zoglauer.
 *
 * By copying, distributing or modifying the Program (or any work
 * based on the Program) you indicate your acceptance of this statement,
 * and all its terms.
 *
 */

// Standard
#include <iostream>
#include <string>
#include <sstream>
#include <csignal>
#include <map>
using namespace std;

// ROOT
#include <TApplication.h>
#include <TStyle.h>
#include <TH1.h>
#include <TCanvas.h>

// MEGAlib
#include "MGlobal.h"
#include "MStreams.h"
#include "MString.h"
#include "MDGeometryQuest.h"
#include "MDDetector.h"
#include "MFileEventsSim.h"
#include "MDVolumeSequence.h"
#include "MSimEvent.h"
#include "MSimHT.h"

/******************************************************************************/

class SimRewriter
{
public:
  /// Default constructor
  SimRewriter();
  /// Default destructor
  ~SimRewriter();
  
  /// Parse the command line
  bool ParseCommandLine(int argc, char** argv);
  /// Analyze whatever needs to be analyzed...
  bool Analyze();
  /// Analyze the event, return true if it has to be writen to file
  bool AnalyzeEvent(MSimEvent* Event);
  /// Interrupt the analysis
  void Interrupt() { m_Interrupt = true; }

private:
  /// True, if the analysis needs to be interrupted
  bool m_Interrupt;

  /// Simulation file name
  MString m_FileName;
  /// Output file name
  MString m_OutputFileName;
  /// Geometry file name
  MString m_GeometryFileName;
};

/******************************************************************************/


/******************************************************************************
 * Default constructor
 */
SimRewriter::SimRewriter() : m_Interrupt(false)
{
  // Intentionally left blank
}


/******************************************************************************
 * Default destructor
 */
SimRewriter::~SimRewriter()
{
  // Intentionally left blank
}


/******************************************************************************
 * Parse the command line
 */
bool SimRewriter::ParseCommandLine(int argc, char** argv)
{
  ostringstream Usage;
  Usage<<endl;
  Usage<<"  Usage: SimRewriter <options>"<<endl;
  Usage<<"    General options:"<<endl;
  Usage<<"         -f:   simulation file name"<<endl;
  Usage<<"         -o:   output simulation file name"<<endl;
  Usage<<"         -g:   geometry file name"<<endl;
  Usage<<"         -h:   print this help"<<endl;
  Usage<<endl;

  string Option;

  // Check for help
  for (int i = 1; i < argc; i++) {
    Option = argv[i];
    if (Option == "-h" || Option == "--help" || Option == "?" || Option == "-?") {
      cout<<Usage.str()<<endl;
      return false;
    }
  }

  // Now parse the command line options:
  for (int i = 1; i < argc; i++) {
    Option = argv[i];

    // First check if each option has sufficient arguments:
    // Single argument
    if (Option == "-f" || Option == "-o") {
      if (!((argc > i+1) && argv[i+1][0] != '-')){
        cout<<"Error: Option "<<argv[i][1]<<" needs a second argument!"<<endl;
        cout<<Usage.str()<<endl;
        return false;
      }
    } 
    // Multiple arguments_
    //else if (Option == "-??") {
    //  if (!((argc > i+2) && argv[i+1][0] != '-' && argv[i+2][0] != '-')){
    //    cout<<"Error: Option "<<argv[i][1]<<" needs two arguments!"<<endl;
    //    cout<<Usage.str()<<endl;
    //    return false;
    //  }
    //}

    // Then fulfill the options:
    if (Option == "-f") {
      m_FileName = argv[++i];
      cout<<"Accepting file name: "<<m_FileName<<endl;
    } else if (Option == "-o") {
      m_OutputFileName = argv[++i];
      cout<<"Accepting output file name: "<<m_OutputFileName<<endl;
    } else if (Option == "-g") {
      m_GeometryFileName = argv[++i];
      cout<<"Accepting file name: "<<m_GeometryFileName<<endl;
    } else {
      cout<<"Error: Unknown option \""<<Option<<"\"!"<<endl;
      cout<<Usage.str()<<endl;
      return false;
    }
  }

  if (m_FileName == "") {
    cout<<"Error: Need a simulation file name!"<<endl;
    cout<<Usage.str()<<endl;
    return false;
  }

  if (m_GeometryFileName == "") {
    cout<<"Error: Need a geometry file name!"<<endl;
    cout<<Usage.str()<<endl;
    return false;
  }

  if (m_FileName.EndsWith(".sim") == true) {
    if (m_OutputFileName == "") {
      m_OutputFileName = m_FileName;
      m_OutputFileName.Replace(m_FileName.Length()-4, 4, ".mod.sim");
      cout<<"Accepting output file name: "<<m_OutputFileName<<endl;
    }    
  } else if (m_FileName.EndsWith(".sim.gz") == true) {
    if (m_OutputFileName == "") {
      m_OutputFileName = m_FileName;
      m_OutputFileName.Replace(m_FileName.Length()-4, 4, ".mod.sim.gz");
      cout<<"Accepting output file name: "<<m_OutputFileName<<endl;
    }    
  } else {
    cout<<"Error: Need a simulation file name, not a "<<m_FileName<<" file "<<endl;
    cout<<Usage.str()<<endl;
    return false;
  }

  return true;
}


/******************************************************************************
 * Do whatever analysis is necessary
 */
bool SimRewriter::Analyze()
{
  // Load geometry:
  MDGeometryQuest* Geometry = new MDGeometryQuest();
  if (Geometry->ScanSetupFile(m_GeometryFileName) == true) {
    cout<<"Geometry "<<Geometry->GetName()<<" loaded!"<<endl;
    Geometry->ActivateNoising(false);
    Geometry->SetGlobalFailureRate(0.0);
  } else {
    cout<<"Unable to load geometry "<<Geometry->GetName()<<" - Aborting!"<<endl;
    return false;
  }  

  MFileEventsSim* Reader = new MFileEventsSim(Geometry);
  if (Reader->Open(m_FileName) == false) {
    cout<<"Unable to open sim file "<<m_FileName<<" - Aborting!"<<endl; 
    return false;
  }
  Reader->ShowProgress();
  
  // cout<<"Opened file "<<SiReader.GetFileName()<<" created with MEGAlib version: "<<SiReader.GetMEGAlibVersion()<<endl;
  // cout<<"Triggered events: "<<SiReader.GetNEvents(false)<<" --- Observation time: "<<SiReader.GetObservationTime()<<" sec  --  simulated events: "<<SiReader.GetSimulatedEvents()<<endl;

  // Open output file:
  MFileEventsSim* Writer = new MFileEventsSim(Geometry);
  if (Writer->Open(m_OutputFileName, MFile::c_Write) == false) {
    cout<<"Unable to open output file!"<<endl;
    return false;
  }
   
  Writer->SetGeometryFileName(m_GeometryFileName);
  Writer->SetVersion(300);
  Writer->WriteHeader();

  ofstream bin;
  bin.open("_.bin", ios::binary);

  MSimEvent* Event = 0;
  while ((Event = Reader->GetNextEvent(false)) != 0) {
    if (m_Interrupt == true) return false;
    if (AnalyzeEvent(Event) == true) {
      //sync word
      bin.put()

      // Write ID 
      bin.write(reinterpret_cast<<const char *>(&Event ->GetSimulationEventID(), sizeof(long));

      // Write mother ID (long)
      bin.write(reinterpret_cast<<const char *>(&Event ->GetMotherParticleID(), sizeof(long));
      // Mother position (3xfloat)
      bin.write(reinterpret_cast<<const char *>(&Event ->GetMotherDirection(), sizeof(double));
      // detector ID
      bin.write(reinterpret_cast<<const char *>(&Event ->GetDetectorID(), sizeof(long));
      // get time
      bin.write(reinterpret_cast<<const char *>(&Event ->GetTime(), sizeof(double));
      // get position
      bin.write(reinterpret_cast<<const char *>(&Event ->GetPositionInDetector(), sizeof(double));
      // original particle ID
      bin.write(reinterpret_cast<<const char *>(&Event ->GetOriginID(), sizeof(double));
      // get original particle direction
      bin.write(reinterpret_cast<<const char *>(&Event ->GetOrigin_____(), sizeof(double));
      // get original particle polarization
      bin.write(reinterpret_cast<<const char *>(&Event ->GetOrigin_____(), sizeof(double));
      // new kinetic energy of original particle
      bin.write(reinterpret_cast<<const char *>(&Event ->GetOrigin_____(), sizeof(double));
      // ID of new particle
      bin.write(reinterpret_cast<<const char *>(&Event ->GetOrigin_____(), sizeof(double));
      // Direction of new particle
      bin.write(reinterpret_cast<<const char *>(&Event ->GetOrigin_____(), sizeof(double));
      // polarization of new particle
      bin.write(reinterpret_cast<<const char *>(&Event ->GetOrigin_____(), sizeof(double));
      // KE of new particle
      bin.write(reinterpret_cast<<const char *>(&Event ->GetOrigin_____(), sizeof(double));

    }
    delete Event;
  }

  cout<<"Observation time: "<<Reader->GetObservationTime()<<" sec  --  simulated events: "<<Reader->GetSimulatedEvents()<<endl;

  Reader->Close();
  delete Reader;
  
  Writer->CloseEventList();
  Writer->Close();
  delete Writer;
  
  delete Geometry;

  return true;
}


/******************************************************************************
 * Analyze the event, return true if it has to be writen to file
 */
bool SimRewriter::AnalyzeEvent(MSimEvent* Event)
{
  // Add your code here
  // Return true if the event should be written to file

  // Example:
  // if (Event.GetVeto() == true) return false;

  return true;
}


/******************************************************************************/

SimRewriter* g_Prg = 0;
int g_NInterrupts = 2;

/******************************************************************************/


/******************************************************************************
 * Called when an interrupt signal is flagged
 * All catched signals lead to a well defined exit of the program
 */
void CatchSignal(int a)
{
  cout<<"Catched signal Ctrl-C:"<<endl;
  
  --g_NInterrupts;
  if (g_NInterrupts <= 0) {
    cout<<"Aborting..."<<endl;
    abort();
  } else {
    cout<<"Trying to cancel the analysis..."<<endl;
    if (g_Prg != 0) {
      g_Prg->Interrupt();
    }
    cout<<"If you hit "<<g_NInterrupts<<" more times, then I will abort immediately!"<<endl;
  }
}


/******************************************************************************
 * Main program
 */
int main(int argc, char** argv)
{
  // Set a default error handler and catch some signals...
  signal(SIGINT, CatchSignal);

  // Initialize global MEGAlib variables, especially mgui, etc.
  MGlobal::Initialize();

  TApplication SimRewriterApp("SimRewriterApp", 0, 0);

  g_Prg = new SimRewriter();

  if (g_Prg->ParseCommandLine(argc, argv) == false) {
    cerr<<"Error during parsing of command line!"<<endl;
    return -1;
  } 
  if (g_Prg->Analyze() == false) {
    cerr<<"Error during analysis!"<<endl;
    return -2;
  } 

  //SimRewriterApp.Run();

  cout<<"Program exited normally!"<<endl;

  return 0;
}

/*
 * Cosima: the end...
 ******************************************************************************/
