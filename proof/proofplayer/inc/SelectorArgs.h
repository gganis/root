#ifndef SELECTOR_ARGS
#define SELECTOR_ARGS

#include <string>
#include <vector>

#include "TNamed.h"

class SelectorArgs : public TNamed {

private:
    Bool_t forceRecreation;
    Bool_t skipErrors;
    Bool_t reoptimize;
    Bool_t noTrees;
    Int_t numFiles;
    Int_t numWorkers;
    Int_t verbosity;
    Int_t newComp;
    Int_t maxOpenedFiles;
    std::string workingDir;
    std::string targetName;
    std::vector<std::string> fileNames;
public:
    SelectorArgs();
    SelectorArgs(Int_t workers, Int_t verbo, Int_t files,
            std::vector<std::string> names, std::string path,
            Bool_t force, Int_t newcomp, Int_t maxopenedfiles,
            Bool_t skiperr, Bool_t reopt, Bool_t notrees,
            std::string targetname);
    ~SelectorArgs();
    Int_t getNumFiles();
    Int_t getNumWorkers();
    Int_t getVerbosity();
    Int_t getNewComp();
    Int_t getMaxOpenedFiles();
    Bool_t getForce();
    Bool_t getReoptimize();
    Bool_t getNoTrees();
    Bool_t getSkipErrors();
    std::vector<std::string> getFileNames();
    const char* pwd();
    const char* getTargetName();

    ClassDef(SelectorArgs, 1)
};


#endif
