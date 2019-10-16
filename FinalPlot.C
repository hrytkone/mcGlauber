void FinalPlot() {
    gROOT->ProcessLine(".L Nucleon.cxx+");
    gROOT->ProcessLine(".x Plot.C");
}
