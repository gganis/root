#include "RConfig.h"
#include <string>
#include <iostream>
#include <fstream>
#include "TFile.h"
#include "THashList.h"
#include "TKey.h"
#include "TObjString.h"
#include "Riostream.h"
#include "TClass.h"
#include "TSystem.h"
#include "TProof.h"
#include <stdlib.h>
#include "TFileMerger.h"
#include "TROOT.h"
#include "TInterpreter.h"
#include "SelectorArgs.h"

// parameters
static Bool_t force = kFALSE;
static Bool_t skip_errors = kFALSE;
static Bool_t reoptimize = kFALSE;
static Bool_t noTrees = kFALSE;
static Bool_t parallel = kFALSE;
static Bool_t fromFile = kFALSE;
static Int_t maxopenedfiles = 0;
static Int_t verbosity = 99;
static Int_t outputPlace = 0;
static Int_t ffirst = 2;
static Int_t newcomp = 1;
static Int_t numWorkers = -1;
static Int_t numFiles = 0;
static const char *targetname = 0;
static std::vector<std::string> inputFiles;

// check if help argument was passed
void checkHelp(int argc, char **argv)
{
    if ( argc < 3 || "-h" == string(argv[1]) || "--help" == string(argv[1]) ) {
        std::cout << "Usage: " << argv[0] << "[-p[2-inf]] [-f[0-9]] [-k] [-T] [-O] [-n maxopenedfiles] [-v verbosity] targetfile source1 [source2 source3 ...]" << std::endl;
        std::cout << "This program will add histograms from a list of root files and write them" << std::endl;
        std::cout << "to a target root file. The target file is newly created and must not " << std::endl;
        std::cout << "exist, or if -f (\"force\") is given, must not be one of the source files." << std::endl;
        std::cout << "Supply at least two source files for this to make sense... ;-)" << std::endl;
        std::cout << "If the option -p is used, hadd will use a selector with multiple workers to merge files in parallel. The -p option can be followed by the number of workers (for example -p3 for 3 workers). If the number of workers is not given, the selector will choose a default number of workers" << std::endl;
        std::cout << "If the option -k is used, hadd will not exit on corrupt or non-existant input files but skip the offending files instead." << std::endl;
        std::cout << "If the option -T is used, Trees are not merged" <<std::endl;
        std::cout << "If the option -O is used, when merging TTree, the basket size is re-optimized" <<std::endl;
        std::cout << "If the option -v is used, explicitly set the verbosity level; 0 request no output, 99 is the default" <<std::endl;
        std::cout << "If the option -n is used, hadd will open at most 'maxopenedfiles' at once, use 0 to request to use the system maximum." << std::endl;
        std::cout << "When -the -f option is specified, one can also specify the compression" <<std::endl;
        std::cout << "level of the target file. By default the compression level is 1, but" <<std::endl;
        std::cout << "if \"-f0\" is specified, the target file will not be compressed." <<std::endl;
        std::cout << "if \"-f6\" is specified, the compression level 6 will be used." <<std::endl;
        std::cout << "if Target and source files have different compression levels"<<std::endl;
        std::cout << " a slower method is used"<<std::endl;
        exit(1);
    }
}


// check input arguments
void checkArgs(int argc, char **argv)
{
    for( int a = 1; a < argc; ++a ) {
        if (strcmp(argv[a],"-T") == 0) {
            noTrees = kTRUE;
            ++ffirst;
        } else if (strcmp(argv[a],"-f") == 0) {
            force = kTRUE;
            ++ffirst;
        } else if (strcmp(argv[a],"-k") == 0) {
            skip_errors = kTRUE;
            ++ffirst;
        } else if (strcmp(argv[a],"-O") == 0) {
            reoptimize = kTRUE;
            ++ffirst;
        } else if (strncmp(argv[a], "-p", 2) == 0) {
            parallel = kTRUE;
            ++ffirst;
            if (strlen(argv[a]) > 2)
                numWorkers = atoi(argv[a] + 2);
        } else if (strcmp(argv[a],"-n") == 0) {
            if (a + 1 >= argc) {
                std::cerr << "Error: no maximum number of opened was provided after -n.\n";
            } else {
                Long_t request = strtol(argv[a+1], 0, 10);
                if (request < kMaxLong && request >= 0) {
                    maxopenedfiles = (Int_t)request;
                    ++a;
                    ++ffirst;
                } else {
                    std::cerr << "Error: could not parse the max number of opened file passed after -n: " << argv[a+1] << ". We will use the system maximum.\n";
                }
            }
            ++ffirst;
        } else if (strcmp(argv[a],"-v") == 0) {
            if (a+1 >= argc) {
                std::cerr << "Error: no verbosity level was provided after -v.\n";
            } else {
                Long_t request = strtol(argv[a+1], 0, 10);
                if (request < kMaxLong && request >= 0) {
                    verbosity = (Int_t)request;
                    ++a;
                    ++ffirst;
                } else {
                    std::cerr << "Error: could not parse the verbosity level passed after -v: " << argv[a+1] << ". We will use the default value (99).\n";
                }
            }
            ++ffirst;
        } else if ( argv[a][0] == '-' ) {
            char ft[4];
            for( int j=0; j<=9; ++j ) {
                snprintf(ft,4,"-f%d",j);
                if (!strcmp(argv[a],ft)) {
                    force = kTRUE;
                    newcomp = j;
                    ++ffirst;
                    break;
                }
            }
            if (!force) {
                // Bad argument
                std::cerr << "Error: option " << argv[a] << " is not a supported option.\n";
                ++ffirst;
            }
        } else if (!outputPlace) {
            outputPlace = a;
        }
    }
}

// load Cintex if it exists
void loadCintex()
{
    gSystem->Load("libTreePlayer");
    TClass::GetClass("ROOT::Cintex::Cintex"); // autoload Cintex if it exists
    if (gInterpreter->IsLoaded("libCintex")) {
        gROOT->ProcessLine("ROOT::Cintex::Cintex::Enable();");
    }
}

// set name of output file
void setTargetName(char **argv)
{
    if (outputPlace) {
        targetname = argv[outputPlace];
    } else {
        targetname = argv[ffirst-1];
    }

    if (verbosity > 1) {
        std::cout << "hadd Target file: " << targetname << std::endl;
    }
}

// read files names from file
void readFromFile(char *fileName)
{
    std::ifstream file;
    file.open(fileName, ios::in);
    if (file.is_open()) {
        while(file.good()) {
            std::string line;
            std::getline(file, line);
            if (line.length() > 0)
                inputFiles.push_back(line);
        }
        file.close();
    }
    else {
        std::cerr << "hadd could not open indirect file " << fileName << std::endl;
        exit(1);
    }
}

// set names of input files
void setInputFiles(int argc, char**argv)
{
    for (Int_t i = ffirst; i < argc; i++) {
        if (argv[i] && argv[i][0] == '@')
            readFromFile(argv[i] + 1);
        else
            inputFiles.push_back(argv[i]);
    }
    numFiles = inputFiles.size();
}

// merge files in parallel using PROOF TSelector
void parallelMerge()
{
    TProof *p = NULL;
    if (numWorkers > 0) {
        TString *workers = new TString();
        workers -> Form("workers=%d", numWorkers);
        p = TProof :: Open(workers -> Data());
    }
    else
        p = TProof :: Open("lite://");

    if (!p || !p -> IsValid()) {
        std::cerr << "Could not start Proof" << std::endl;
        exit(1);
    }

    std::vector<std::string> names;
    std::string currentPath(gSystem -> WorkingDirectory());

    for (Int_t i = 0; i < (Int_t)inputFiles.size(); i++) {
        if (gSystem -> IsAbsoluteFileName(inputFiles[i].c_str()))
            names.push_back(inputFiles[i]);
        else {
            std::string fileName(gSystem -> ConcatFileName(currentPath.c_str(), inputFiles[i].c_str()));
            names.push_back(fileName);
        }
    }

    SelectorArgs *args = new SelectorArgs(p -> GetParallel(), verbosity,
                            names.size(), names, currentPath, force, newcomp,
                            maxopenedfiles, skip_errors, reoptimize, noTrees,
                            targetname);

    p -> SetParameter("PROOF_PacketizerFixedNum", (Int_t)1);
    p -> AddInput(args);
    p -> Process("ProofMerger", p -> GetParallel());
}

// merge files sequentially
void sequentialMerge()
{
    TFileMerger merger(kFALSE,kFALSE);
    merger.SetMsgPrefix("hadd");
    merger.SetPrintLevel(verbosity - 1);
    if (maxopenedfiles > 0) {
        merger.SetMaxOpenedFiles(maxopenedfiles);
    }
    if (!merger.OutputFile(targetname,force,newcomp) ) {
        std::cerr << "hadd error opening target file (does " << targetname << " exist?)." << std::endl;
        std::cerr << "Pass \"-f\" argument to force re-creation of output file." << std::endl;
        exit(1);
    }


    for (Int_t i = 0; i < (Int_t)inputFiles.size(); i++ ) {
        if(!merger.AddFile(inputFiles[i].c_str())) {
            if (skip_errors) {
                std::cerr << "hadd skipping file with error: " << inputFiles[i].c_str() << std::endl;
            } else {
                std::cerr << "hadd exiting due to error in " << inputFiles[i].c_str() << std::endl;
                exit(1);
            }
        }
    }

    if (reoptimize) {
        merger.SetFastMethod(kFALSE);
    } else {
        if (merger.HasCompressionChange()) {
            // Don't warn if the user any request re-optimization.
            std::cout << "hadd Sources and Target have different compression levels" << std::endl;
            std::cout << "hadd merging will be slower" << std::endl;
        }
    }
    merger.SetNotrees(noTrees);
    Bool_t status = merger.Merge();

    if (status) {
        if (verbosity == 1) {
            std::cout << "hadd merged " << merger.GetMergeList()->GetEntries() << " input files in " << targetname << ".\n";
        }
    } else {
        if (verbosity == 1) {
            std::cout << "hadd failure during the merge of " << merger.GetMergeList()->GetEntries() << " input files in " << targetname << ".\n";
        }
        exit(1);
    }
}

// selects the appropriate merge function
void mergeFiles()
{
    if (parallel == kTRUE && numWorkers < numFiles)
        parallelMerge();
    else
        sequentialMerge();
}

// main function
int main(int argc, char **argv)
{
    checkHelp(argc, argv);     // check for help argument
    checkArgs(argc, argv);     // check all arguments
    loadCintex();              // load Cintex if it exists
    setTargetName(argv);       // set output file name
    setInputFiles(argc, argv); // set input files names for merging
    mergeFiles();              // choose merge method and merge
    return 0;
}
