#include "CPPMAnalyzerSettings.h"
#include <AnalyzerHelpers.h>


CPPMAnalyzerSettings::CPPMAnalyzerSettings()
    :   mInputChannel(UNDEFINED_CHANNEL),
        mSyncTime(2500)
{
    mInputChannelInterface.reset(new AnalyzerSettingInterfaceChannel());
    mInputChannelInterface->SetTitleAndTooltip("CPPM", "Simple CPPM Analyzer");
    mInputChannelInterface->SetChannel(mInputChannel);

    mSyncTimeInterface.reset(new AnalyzerSettingInterfaceInteger());
    mSyncTimeInterface->SetTitleAndTooltip("PPM Sync Time Minimum (μS)",
                                           "Specify the minimum number of microseconds for a PPM sync.");
    mSyncTimeInterface->SetMax(10000000);
    mSyncTimeInterface->SetMin(100);
    mSyncTimeInterface->SetInteger(mSyncTime);

    AddInterface(mInputChannelInterface.get());
    AddInterface(mSyncTimeInterface.get());

    AddExportOption(0, "Export as csv file");
    AddExportExtension(0, "csv file", "csv");

    AddExportOption(1, "Export as long form csv file");
    AddExportExtension(1, "csv file", "csv");

    ClearChannels();
    AddChannel(mInputChannel, "CPPM", false);
}

CPPMAnalyzerSettings::~CPPMAnalyzerSettings()
{
}

bool CPPMAnalyzerSettings::SetSettingsFromInterfaces()
{
    mInputChannel = mInputChannelInterface->GetChannel();
    mSyncTime = mSyncTimeInterface->GetInteger();

    ClearChannels();
    AddChannel(mInputChannel, "CPPM Analyzer", true);

    return true;
}

void CPPMAnalyzerSettings::UpdateInterfacesFromSettings()
{
    mInputChannelInterface->SetChannel(mInputChannel);
    mSyncTimeInterface->SetInteger(mSyncTime);
}

void CPPMAnalyzerSettings::LoadSettings(const char *settings)
{
    SimpleArchive text_archive;
    text_archive.SetString(settings);

    text_archive >> mInputChannel;
    text_archive >> mSyncTime;

    ClearChannels();
    AddChannel(mInputChannel, "Simple CPPM Analyzer", true);

    UpdateInterfacesFromSettings();
}

const char *CPPMAnalyzerSettings::SaveSettings()
{
    SimpleArchive text_archive;

    text_archive << mInputChannel;
    text_archive << mSyncTime;

    return SetReturnString(text_archive.GetString());
}
