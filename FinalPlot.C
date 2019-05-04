void FinalPlot() {
    gROOT->ProcessLine(".L JNucleon.cxx+");
    gROOT->ProcessLine(".x Plot.C");
}
