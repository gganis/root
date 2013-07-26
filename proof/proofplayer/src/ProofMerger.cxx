#define ProofMerger_cxx

#include "ProofMerger.h"

#define WORKER_FILE_NAME    "/worker"

// constructor for the merger
ProofMerger::ProofMerger()
{
}

// destructor for the merger
ProofMerger::~ProofMerger()
{
}

// begin method acquires the input arguments
void ProofMerger::Begin(TTree * /*tree*/)
{
    if (fInput -> FindObject("args")) {
        args = dynamic_cast<SelectorArgs*>(fInput -> FindObject("args"));
    }
}

// get ID of current worker
Int_t ProofMerger::GetCurrentWorkerID()
{
    std::string ord = gProofServ -> GetOrdinal();
    ord.erase(0, 2);
    Int_t id = atoi(ord.c_str());
    return id;
}

// get string ID of current worker
std::string ProofMerger::GetCurrentWorkerStr()
{
    std::string ord = gProofServ -> GetOrdinal();
    ord.erase(0, 2);
    return ord;
}

// each worker acquires the input arguments and selects
// the files to merge
void ProofMerger::SlaveBegin(TTree * /*tree*/)
{
    if (fInput -> FindObject("args")) {
        args = dynamic_cast<SelectorArgs*>(fInput -> FindObject("args"));
        Int_t id = GetCurrentWorkerID();
        Int_t numWorkers = args -> getNumWorkers();
        Int_t numFiles = args -> getNumFiles();
        std::vector<std::string> files = args -> getFileNames();
        Int_t numWorkerFiles = numFiles / numWorkers;
        for (Int_t i = id * numWorkerFiles; i < (id + 1) * numWorkerFiles; i++)
            currentFiles.push_back(files[i]);
        if (id == numWorkers - 1)
            for (Int_t i = (id + 1) * numWorkerFiles; i < numFiles; i++)
                currentFiles.push_back(files[i]);
    }
}

// method to remove files from a merger
void ProofMerger::DeleteFiles(TFileMerger *fileMerger)
{
    // Remove the files
    TList *fileList = fileMerger -> GetMergeList();
    if (fileList) {
        TIter next(fileList);
        TObjString *url = 0;
        while((url = (TObjString*)next())) {
            gSystem -> Unlink(url -> GetString());
        }
    }
    // Reset the merger
    fileMerger->Reset();
}

// method for merging a set of input files (same as in hadd)
Bool_t ProofMerger::MergeFiles(std::vector<std::string> files, std::string outputFile, Bool_t remove)
{
    TFileMerger merger(kFALSE, kFALSE);
    merger.SetMsgPrefix("hadd");
    merger.SetPrintLevel(args -> getVerbosity() - 1);

    Bool_t force = args -> getForce();
    Bool_t reoptimize = args -> getReoptimize();
    Bool_t skipErrors = args -> getSkipErrors();
    Bool_t noTrees = args -> getNoTrees();
    Int_t newcomp = args -> getNewComp();
    Int_t maxopenedfiles = args -> getMaxOpenedFiles();

    if (maxopenedfiles > 0) {
        merger.SetMaxOpenedFiles(maxopenedfiles);
    }

    if (!merger.OutputFile(outputFile.c_str(),force,newcomp) ) {
        std::cerr << "hadd error opening target file (does " << outputFile.c_str() << " exist?)." << std::endl;
        std::cerr << "Pass \"-f\" argument to force re-creation of output file." << std::endl;
        exit(1);
    }

    for (Int_t i = 0; i < (Int_t)files.size(); i++) {
        const char *fileName = files[i].c_str();
        if(!merger.AddFile(fileName)) {

            if ( skipErrors ) {
                std::cerr << "hadd skipping file with error: " << fileName << std::endl;
            } else {
                std::cerr << "hadd exiting due to error in " << fileName << std::endl;
                return kFALSE;
            }

            return kFALSE;
        }
    }

    if (reoptimize) {
        merger.SetFastMethod(kFALSE);
    } else {
        if (merger.HasCompressionChange()) {
            // Don't warn if the user any request re-optimization.
            std::cout <<"hadd Sources and Target have different compression levels"<< std::endl;
            std::cout <<"hadd merging will be slower"<< std::endl;
        }
    }

    merger.SetNotrees(noTrees);
    Bool_t status = merger.Merge();

    // remove input files
    if (remove == kTRUE)
        DeleteFiles(&merger);

    return status;
}

// worker process method merges selected files
Bool_t ProofMerger::Process(Long64_t)
{
    std::string workerFile(args -> pwd());
    workerFile.append(WORKER_FILE_NAME);
    workerFile.append(GetCurrentWorkerStr());
    workerFile.append(".root");
    return MergeFiles(currentFiles, workerFile, kFALSE);
}

// nothing to do in worker terminate
void ProofMerger::SlaveTerminate()
{
}

// client merges worker files
void ProofMerger::Terminate()
{
    if (!args) {
        std::cerr << "Parameters not found" << std::endl;
        return;
    }

    std::vector<std::string> workerFiles;
    for (Int_t i = 0; i < args -> getNumWorkers(); i++) {
        TString *workerFile = new TString();
        workerFile -> Form("%s%s%d.root", args -> pwd(), WORKER_FILE_NAME, i);
        workerFiles.push_back(workerFile -> Data());
    }
    std::string outputFile(args -> pwd());
    outputFile.append("/");
    outputFile.append(args -> getTargetName());
    Bool_t status = MergeFiles(workerFiles, outputFile, kTRUE);
    if (status == kFALSE)
        std::cerr << "Final merger failure" << std::endl;
}


