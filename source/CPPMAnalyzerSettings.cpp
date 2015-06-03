#include "CPPMAnalyzerSettings.h"
#include <AnalyzerHelpers.h>


CPPMAnalyzerSettings::CPPMAnalyzerSettings()
    :   mInputChannel(UNDEFINED_CHANNEL),
        mSyncTime(2500),
        mSyncDir(0),
        mMaxChan(32),
        mMinChange(3)
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

    mMinChangeInterface.reset(new AnalyzerSettingInterfaceInteger());
    mMinChangeInterface->SetTitleAndTooltip("Min Change(μS)",
                                           "The minimum amount of value change before recording a frame.");
    mMinChangeInterface->SetMax(10000);
    mMinChangeInterface->SetMin(0);
    mMinChangeInterface->SetInteger(mMinChange);

    mSyncDirInterface.reset(new AnalyzerSettingInterfaceNumberList());
    mSyncDirInterface->SetTitleAndTooltip("Sync Level",
                                          "Whether the sync is expected to be high or low");
    mSyncDirInterface->AddNumber(0, "Auto", "Automatically determine sync level");
    mSyncDirInterface->AddNumber(1, "High", "Sync signal is high");
    mSyncDirInterface->AddNumber(2, "Low", "Sync signal is low");
    mSyncDirInterface->SetNumber(mSyncDir);

    mMaxChanInterface.reset(new AnalyzerSettingInterfaceInteger());
    mMaxChanInterface->SetTitleAndTooltip("Maximum Channels",
                                           "The maximum channel number in a frame.");
    mMaxChanInterface->SetMax(10000000);
    mMaxChanInterface->SetMin(1);
    mMaxChanInterface->SetInteger(mMaxChan);


    AddInterface(mInputChannelInterface.get());
    AddInterface(mSyncTimeInterface.get());
    AddInterface(mSyncDirInterface.get());
    AddInterface(mMaxChanInterface.get());
    AddInterface(mMinChangeInterface.get());

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
    mSyncDir = mSyncDirInterface->GetNumber();
    mMaxChan = mMaxChanInterface->GetInteger();
    mMinChange = mMinChangeInterface->GetInteger();

    ClearChannels();
    AddChannel(mInputChannel, "CPPM Analyzer", true);

    return true;
}

void CPPMAnalyzerSettings::UpdateInterfacesFromSettings()
{
    mInputChannelInterface->SetChannel(mInputChannel);
    mSyncTimeInterface->SetInteger(mSyncTime);
    mSyncDirInterface->SetNumber(mSyncDir);
    mMaxChanInterface->SetInteger(mMaxChan);
    mMinChangeInterface->SetInteger(mMinChange);
}

void CPPMAnalyzerSettings::LoadSettings(const char *settings)
{
    SimpleArchive text_archive;
    text_archive.SetString(settings);

    text_archive >> mInputChannel;
    text_archive >> mSyncTime;
    text_archive >> mSyncDir;
    text_archive >> mMaxChan;
    text_archive >> mMinChange;

    ClearChannels();
    AddChannel(mInputChannel, "Simple CPPM Analyzer", true);

    UpdateInterfacesFromSettings();
}

const char *CPPMAnalyzerSettings::SaveSettings()
{
    SimpleArchive text_archive;

    text_archive << mInputChannel;
    text_archive << mSyncTime;
    text_archive << mSyncDir;
    text_archive << mMaxChan;
    text_archive << mMinChange;

    return SetReturnString(text_archive.GetString());
}
