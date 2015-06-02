#include "CPPMAnalyzer.h"
#include "CPPMAnalyzerSettings.h"
#include <AnalyzerChannelData.h>

CPPMAnalyzer::CPPMAnalyzer()
    :   Analyzer2(),
        mSettings(new CPPMAnalyzerSettings()),
        mSimulationInitilized(false)
{
    SetAnalyzerSettings(mSettings.get());
}

CPPMAnalyzer::~CPPMAnalyzer()
{
    KillThread();
}

void CPPMAnalyzer::SetupResults()
{
    mResults.reset(new CPPMAnalyzerResults(this, mSettings.get()));
    SetAnalyzerResults(mResults.get());
    mResults->AddChannelBubblesWillAppearOn(mSettings->mInputChannel);
}

void CPPMAnalyzer::WorkerThread()
{
    mSampleRateHz = GetSampleRate();

    mCPPM = GetAnalyzerChannelData(mSettings->mInputChannel);

    // Wait for a clean start
    while (SamplesToUs(mCPPM->GetSampleOfNextEdge() - mCPPM->GetSampleNumber()) < 4000) {
        mCPPM->AdvanceToNextEdge();
    }
    mCPPM->AdvanceToNextEdge(); // Should now be low
    mCPPM->AdvanceToNextEdge(); // Should now be and high to start sizing first channel

    //let's put a dot exactly where we sample this bit:
    mResults->AddMarker(mCPPM->GetSampleNumber(), AnalyzerResults::Dot, mSettings->mInputChannel);

    U8 channel = 0;
    for (;;) {
        U64 start = mCPPM->GetSampleNumber(); // Falling Edge

        mCPPM->AdvanceToNextEdge();

        U64 end = mCPPM->GetSampleNumber();

        U64 width = SamplesToUs(end - start);

        if (width >= 4000 && mCPPM->GetBitState() == BIT_LOW) {
            channel = 0;
            mCPPM->AdvanceToNextEdge(); // Falling Edge
            mResults->AddMarker(mCPPM->GetSampleNumber(), AnalyzerResults::Dot, mSettings->mInputChannel);

            continue;
        }

        channel++;

        //we have a byte to save.
        Frame frame;
        frame.mData1 = width;
        frame.mData2 = channel;
        frame.mFlags = 0;
        frame.mStartingSampleInclusive = start;
        frame.mEndingSampleInclusive = mCPPM->GetSampleNumber();

        mResults->AddFrame(frame);
        mResults->CommitResults();
        ReportProgress(frame.mEndingSampleInclusive);

        mCPPM->AdvanceToNextEdge(); // Rising edge
    }
}

bool CPPMAnalyzer::NeedsRerun()
{
    return false;
}

U32 CPPMAnalyzer::GenerateSimulationData(U64 minimum_sample_index, U32 device_sample_rate, SimulationChannelDescriptor **simulation_channels)
{
    if (mSimulationInitilized == false) {
        mSimulationDataGenerator.Initialize(GetSimulationSampleRate(), mSettings.get());
        mSimulationInitilized = true;
    }

    return mSimulationDataGenerator.GenerateSimulationData(minimum_sample_index, device_sample_rate, simulation_channels);
}

U32 CPPMAnalyzer::GetMinimumSampleRateHz()
{
    return 1000000;
}

const char *CPPMAnalyzer::GetAnalyzerName() const
{
    return "CPPM";
}

const char *GetAnalyzerName()
{
    return "CPPM";
}

Analyzer *CreateAnalyzer()
{
    return new CPPMAnalyzer();
}

void DestroyAnalyzer(Analyzer *analyzer)
{
    delete analyzer;
}

U64 CPPMAnalyzer::SamplesToUs(U64 samples)
{
    return (samples * 1000000) / mSampleRateHz;
}
