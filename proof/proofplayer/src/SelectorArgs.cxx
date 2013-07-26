#include "SelectorArgs.h"

// empty constructor
SelectorArgs::SelectorArgs() : TNamed("args", "") {}

// constructor with paramteres (the arguments for the selector)
SelectorArgs::SelectorArgs(Int_t workers, Int_t verbo, Int_t files,
        std::vector<std::string> names, std::string path,
        Bool_t force, Int_t newcomp, Int_t maxopenedfiles,
        Bool_t skiperr, Bool_t reopt, Bool_t notrees,
        std::string targetname) : TNamed("args", "") {
    numWorkers = workers;
    numFiles = files;
    verbosity = verbo;
    workingDir = path;
    forceRecreation = force;
    newComp = newcomp;
    maxOpenedFiles = maxopenedfiles;
    skipErrors = skiperr;
    reoptimize = reopt;
    noTrees = notrees;
    targetName = targetname;
    for (Int_t i = 0; i < numFiles; i++) {
        char *c = new char[names[i].length() + 1];
        memcpy(c, names[i].c_str(), names[i].length() + 1);
        fileNames.push_back(c);
    }
}

// empty desturctor
SelectorArgs::~SelectorArgs() {}

// returns number of files
Int_t SelectorArgs::getNumFiles() { return numFiles; }

// returns number of workers
Int_t SelectorArgs::getNumWorkers() { return numWorkers; }

// returns verbosity parameter
Int_t SelectorArgs::getVerbosity() { return verbosity; }

// returns new comp parameter
Int_t SelectorArgs::getNewComp() { return newComp; }

// returns max opened files parameter
Int_t SelectorArgs::getMaxOpenedFiles() { return maxOpenedFiles; }

// returns force recreation parameter
Bool_t SelectorArgs::getForce() { return forceRecreation; }

// returns reoptimization parameter
Bool_t SelectorArgs::getReoptimize() { return reoptimize; }

// returns no trees parameter
Bool_t SelectorArgs::getNoTrees() { return noTrees; }

// returns skip errors parameter
Bool_t SelectorArgs::getSkipErrors() { return skipErrors; }

// returns vector containing the names of the files
std::vector<std::string> SelectorArgs::getFileNames() { return fileNames; }

// returns path to working directory
const char* SelectorArgs::pwd() { return workingDir.c_str(); }

// returns output file name
const char* SelectorArgs::getTargetName() { return targetName.c_str(); }
