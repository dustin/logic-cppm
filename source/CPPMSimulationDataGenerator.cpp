#include <algorithm>
#include <vector>
#include <random>

#include "CPPMSimulationDataGenerator.h"
#include "CPPMAnalyzerSettings.h"

#include <AnalyzerHelpers.h>

CPPMSimulationDataGenerator::CPPMSimulationDataGenerator() : rgen(),
                                                             rdist(.5),
                                                             pulses(8, 1500)
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

    mCPPMSimulationData.SetChannel(mSettings->mInputChannel);
    mCPPMSimulationData.SetSampleRate(simulation_sample_rate);
    mCPPMSimulationData.SetInitialBitState(mSettings->mSyncDir == 2 ? BIT_LOW : BIT_HIGH);

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
        Pulse();
    }

    *simulation_channel = &mCPPMSimulationData;
    return 1;
}

void CPPMSimulationDataGenerator::Pulse()
{
    std::transform(pulses.begin(), pulses.end(), pulses.begin(), [&](double x) {
            double by = 10*rdist(rgen)*(rgen()&1?1:-1);
            return (x+by > 2000 || x+by < 1000) ? x-by : x+by;
        });

    mCPPMSimulationData.Advance(mClockGenerator.AdvanceByTimeS(.005));
    mCPPMSimulationData.Transition();

    std::for_each(pulses.begin(), pulses.end(), [&](double pulseLen) {
            mCPPMSimulationData.Advance(mClockGenerator.AdvanceByTimeS(.0003));
            mCPPMSimulationData.Transition();
            mCPPMSimulationData.Advance(mClockGenerator.AdvanceByTimeS((pulseLen * 1E-6) - 0.0003));
            mCPPMSimulationData.Transition();
        });
    mCPPMSimulationData.Advance(mClockGenerator.AdvanceByTimeS(.0003));
    mCPPMSimulationData.Transition();
}
