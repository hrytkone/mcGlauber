void FinalPlot() {
    gROOT->ProcessLine(".L src/Nucleon.cxx+");
    gROOT->ProcessLine(".x Plot.C");
}
