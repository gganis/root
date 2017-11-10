/// \file
/// \ingroup tutorial_multicore
/// Fill histograms in parallel with automatic binning.
/// Illustrates use of power-of-two autobin algorithm
///
/// \macro_code
///
/// \author Gerardo Ganis
/// \date November 2017

// The number of workers
const UInt_t nWorkers = 8U;

// Reference boundaries
const Double_t xmiref = -1.;
const Double_t xmaref = 7.;


void mt304_1d(UInt_t nNumbers)
{

   // Histograms to be filled in parallel
   ROOT::TThreadedObject<TH1D> h1d("h1d", "1D test histogram", 64, 0., -1.);
   ROOT::TThreadedObject<TH1D> h1dr("h1dr", "1D test histogram w/ ref boundaries", 64, xmiref, xmaref);

   // We define our work item
   auto workItem = [&](UInt_t workerID) {
      // One generator, file and ntuple per worker
      TRandom3 workerRndm(workerID); // Change the seed

      auto wh1d = h1d.Get();
      wh1d->SetBit(TH1::kAutoBinPTwo);
      auto wh1dr = h1dr.Get();

      Double_t x;
      for (UInt_t i = 0; i < nNumbers; ++i) {
         x = workerRndm.Gaus(3.);
         wh1d->Fill(x);
         wh1dr->Fill(x);
      }
   };

   // Create the collection which will hold the threads, our "pool"
   std::vector<std::thread> workers;

   // Fill the "pool" with workers
   for (auto workerID : ROOT::TSeqI(nWorkers)) {
      workers.emplace_back(workItem, workerID);
   }

   // Now join them
   for (auto &&worker : workers)
      worker.join();

   // Merge
   auto fh1d = h1d.Merge();
   auto fh1dr = h1dr.Merge();

   // Make the canvas
   auto c = new TCanvas("c", "c", 800, 800);
   c->Divide(1, 2);

   gStyle->SetOptStat(111110);
   c->cd(1);
   fh1d->DrawCopy();
   c->cd(2);
   fh1dr->DrawCopy();

   c->Update();

}

void mt304_2d(UInt_t nNumbers)
{

   // Histograms to be filled in parallel
   ROOT::TThreadedObject<TH2D> h2d("h2d", "2D test histogram", 64, 0., -1., 64, 0., -1.);
   ROOT::TThreadedObject<TH2D> h2dr("h2dr", "2D test histogram w/ ref boundaries", 64, xmiref, xmaref, 64, xmiref, xmaref);

   // We define our work item
   auto workItem = [&](UInt_t workerID) {
      // One generator, file and ntuple per worker
      TRandom3 workerRndm(workerID); // Change the seed

      auto wh2d = h2d.Get();
      wh2d->SetBit(TH1::kAutoBinPTwo);
      auto wh2dr = h2dr.Get();

      Double_t x, y;
      for (UInt_t i = 0; i < nNumbers; ++i) {
         x = workerRndm.Gaus(3.);
         y = workerRndm.Gaus(3.);
         wh2d->Fill(x,y);
         wh2dr->Fill(x,y);
      }
   };

   // Create the collection which will hold the threads, our "pool"
   std::vector<std::thread> workers;

   // Fill the "pool" with workers
   for (auto workerID : ROOT::TSeqI(nWorkers)) {
      workers.emplace_back(workItem, workerID);
   }

   // Now join them
   for (auto &&worker : workers)
      worker.join();

   // Merge
   auto fh2d = h2d.Merge();
   auto fh2dr = h2dr.Merge();

   // Make the canvas
   TCanvas *c = new TCanvas("c", "c", 800, 800);
   c->Divide(1, 2);

   gStyle->SetOptStat(111110);
   c->cd(1);
   fh2d->DrawCopy("LEGO");
   c->cd(2);
   fh2dr->DrawCopy("LEGO");

   c->Update();

}

void mt304_3d(UInt_t nNumbers)
{

   // Histograms to be filled in parallel
   ROOT::TThreadedObject<TH3D> h3d("h3d", "3D test histogram", 64, 0., -1., 64, 0., -1., 64, 0., -1.);
   ROOT::TThreadedObject<TH3D> h3dr("h3dr", "3D test histogram w/ ref boundaries", 64, xmiref, xmaref, 64, xmiref, xmaref, 64, xmiref, xmaref);

   // We define our work item
   auto workItem = [&](UInt_t workerID) {
      // One generator, file and ntuple per worker
      TRandom3 workerRndm(workerID); // Change the seed

      auto wh3d = h3d.Get();
      wh3d->SetBit(TH1::kAutoBinPTwo);
      auto wh3dr = h3dr.Get();

      Double_t x, y, z;
      for (UInt_t i = 0; i < nNumbers; ++i) {
         x = workerRndm.Gaus(3.);
         y = workerRndm.Gaus(3.);
         z = workerRndm.Gaus(3.);
         wh3d->Fill(x,y,z);
         wh3dr->Fill(x,y,z);
      }
   };

   // Create the collection which will hold the threads, our "pool"
   std::vector<std::thread> workers;

   // Fill the "pool" with workers
   for (auto workerID : ROOT::TSeqI(nWorkers)) {
      workers.emplace_back(workItem, workerID);
   }

   // Now join them
   for (auto &&worker : workers)
      worker.join();

   // Merge
   auto fh3d = h3d.Merge();
   auto fh3dr = h3dr.Merge();

   // Make the canvas
   TCanvas *c = new TCanvas("c", "c", 800, 800);
   c->Divide(1, 2);

   gStyle->SetOptStat(111110);
   c->cd(1);
   fh3d->DrawCopy();
   c->cd(2);
   fh3dr->DrawCopy();

   c->Update();

}

Int_t mt304_fillHistos(UInt_t nNumbers = 1001, UInt_t opt = 1)
{

   // The first, fundamental operation to be performed in order to make ROOT
   // thread-aware.
   ROOT::EnableThreadSafety();

   switch(opt) {
      case 3:
         mt304_3d(nNumbers);
         break;
      case 2:
         mt304_2d(nNumbers);
         break;
      default:
         mt304_1d(nNumbers);
   }

   gROOTMutex = 0;

   return 0;
}
