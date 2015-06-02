#ifndef CPPM_ANALYZER_H
#define CPPM_ANALYZER_H

#include <Analyzer.h>
#include "CPPMAnalyzerResults.h"
#include "CPPMSimulationDataGenerator.h"

class CPPMAnalyzerSettings;
class ANALYZER_EXPORT CPPMAnalyzer : public Analyzer2
{
public:
    CPPMAnalyzer();
    virtual ~CPPMAnalyzer();
    virtual void WorkerThread();
    virtual void SetupResults();

    virtual U32 GenerateSimulationData(U64 newest_sample_requested,
                                       U32 sample_rate,
                                       SimulationChannelDescriptor **simulation_channels);
    virtual U32 GetMinimumSampleRateHz();

    virtual const char *GetAnalyzerName() const;
    virtual bool NeedsRerun();

protected: //vars
    std::auto_ptr< CPPMAnalyzerSettings > mSettings;
    std::auto_ptr< CPPMAnalyzerResults > mResults;
    AnalyzerChannelData *mCPPM;

    CPPMSimulationDataGenerator mSimulationDataGenerator;
    bool mSimulationInitilized;

    U64 SamplesToUs(U64 samples);

    //CPPM analysis vars:
    U32 mSampleRateHz;
};

extern "C" ANALYZER_EXPORT const char *__cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer *__cdecl CreateAnalyzer();
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer(Analyzer *analyzer);

#endif //CPPM_ANALYZER_H
