#ifndef CPPM_SIMULATION_DATA_GENERATOR
#define CPPM_SIMULATION_DATA_GENERATOR

#include <vector>
#include <random>

#include <AnalyzerHelpers.h>

#include <SimulationChannelDescriptor.h>
class CPPMAnalyzerSettings;

class CPPMSimulationDataGenerator
{
public:
    CPPMSimulationDataGenerator();
    ~CPPMSimulationDataGenerator();

    void Initialize(U32 simulation_sample_rate, CPPMAnalyzerSettings *settings);
    U32 GenerateSimulationData(U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor **simulation_channel);

protected:
    CPPMAnalyzerSettings *mSettings;
    U32 mSimulationSampleRateHz;

protected:
    SimulationChannelDescriptor mCPPMSimulationData;
    ClockGenerator mClockGenerator;

    void Pulse();
    double GenDelta();

    std::mt19937 rgen;
    std::exponential_distribution<> rdist;
    std::vector<double> pulses;

};
#endif //CPPM_SIMULATION_DATA_GENERATOR
