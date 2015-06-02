#include "CPPMSimulationDataGenerator.h"
#include "CPPMAnalyzerSettings.h"

#include <AnalyzerHelpers.h>

CPPMSimulationDataGenerator::CPPMSimulationDataGenerator()
{
}

CPPMSimulationDataGenerator::~CPPMSimulationDataGenerator()
{
}

void CPPMSimulationDataGenerator::Initialize(U32 simulation_sample_rate,
                                            CPPMAnalyzerSettings *settings)
{
    mSimulationSampleRateHz = simulation_sample_rate;
    mSettings = settings;

    pulseLen = 1000;
    incr = 13;

    mCPPMSimulationData.SetChannel(mSettings->mInputChannel);
    mCPPMSimulationData.SetSampleRate(simulation_sample_rate);
    mCPPMSimulationData.SetInitialBitState(BIT_HIGH);

    mClockGenerator.Init(1000000, simulation_sample_rate);
}

U32 CPPMSimulationDataGenerator::GenerateSimulationData(U64 largest_sample_requested,
                                                       U32 sample_rate,
                                                       SimulationChannelDescriptor **simulation_channel)
{
    U64 adjusted_largest_sample_requested = AnalyzerHelpers::AdjustSimulationTargetSample(largest_sample_requested,
                                            sample_rate,
                                            mSimulationSampleRateHz);

    while (mCPPMSimulationData.GetCurrentSampleNumber() < adjusted_largest_sample_requested) {
        Pulse(pulseLen, 8);
        pulseLen += incr;
        if (pulseLen > 2000) {
            pulseLen -= incr;
            incr = -incr;
        } else if (pulseLen < 1000) {
            pulseLen += incr;
            incr = -incr;
        }
    }

    *simulation_channel = &mCPPMSimulationData;
    return 1;
}

void CPPMSimulationDataGenerator::Pulse(double duration, int channels)
{
    mCPPMSimulationData.TransitionIfNeeded(BIT_HIGH);
    mCPPMSimulationData.Advance(mClockGenerator.AdvanceByTimeS(.005));
    mCPPMSimulationData.TransitionIfNeeded(BIT_LOW);

    for (int i = 0; i < channels; i++) {
        mCPPMSimulationData.Advance(mClockGenerator.AdvanceByTimeS(.0003));
        mCPPMSimulationData.TransitionIfNeeded(BIT_HIGH);
        mCPPMSimulationData.Advance(mClockGenerator.AdvanceByTimeS((duration * 1E-6) - 0.0003));
        mCPPMSimulationData.TransitionIfNeeded(BIT_LOW);
    }
    mCPPMSimulationData.Advance(mClockGenerator.AdvanceByTimeS(.0003));
    mCPPMSimulationData.TransitionIfNeeded(BIT_HIGH);
}
