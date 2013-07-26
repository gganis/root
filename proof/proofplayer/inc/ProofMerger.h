#ifndef ProofMerger_h
#define ProofMerger_h

#include "TSelector.h"
#include "SelectorArgs.h"
#include "TFileMerger.h"
#include "TSystem.h"
#include "Riostream.h"
#include "TProofServ.h"
#include "TParameter.h"
#include <string>
#include <vector>
#include <stdlib.h>

// Selector for merging files in parallel
class ProofMerger : public TSelector {
    private:
        Int_t GetCurrentWorkerID();
        std::string GetCurrentWorkerStr();
        void DeleteFiles(TFileMerger *fileMerger);
        Bool_t MergeFiles(std::vector<std::string> files, std::string outputFile, Bool_t remove);

    public:
        SelectorArgs *args;
        std::vector<std::string> currentFiles;

        ProofMerger();
        virtual ~ProofMerger();
        virtual Int_t   Version() const { return 2; }
        virtual void    Begin(TTree *tree);
        virtual void    SlaveBegin(TTree *tree);
        virtual Bool_t  Process(Long64_t entry);
        virtual void    SetOption(const char *option) { fOption = option; }
        virtual void    SetObject(TObject *obj) { fObject = obj; }
        virtual void    SetInputList(TList *input) { fInput = input; }
        virtual TList  *GetOutputList() const { return fOutput; }
        virtual void    SlaveTerminate();
        virtual void    Terminate();

        ClassDef(ProofMerger,0);
};
#endif
