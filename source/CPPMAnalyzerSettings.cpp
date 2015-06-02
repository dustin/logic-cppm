#include "CPPMAnalyzerSettings.h"
#include <AnalyzerHelpers.h>


CPPMAnalyzerSettings::CPPMAnalyzerSettings()
    :   mInputChannel(UNDEFINED_CHANNEL)
{
    mInputChannelInterface.reset(new AnalyzerSettingInterfaceChannel());
    mInputChannelInterface->SetTitleAndTooltip("CPPM", "Simple CPPM Analyzer");
    mInputChannelInterface->SetChannel(mInputChannel);

    AddInterface(mInputChannelInterface.get());

    AddExportOption(0, "Export as csv file");
    AddExportExtension(0, "csv", "csv");

    ClearChannels();
    AddChannel(mInputChannel, "CPPM", false);
}

CPPMAnalyzerSettings::~CPPMAnalyzerSettings()
{
}

bool CPPMAnalyzerSettings::SetSettingsFromInterfaces()
{
    mInputChannel = mInputChannelInterface->GetChannel();

    ClearChannels();
    AddChannel(mInputChannel, "CPPM Analyzer", true);

    return true;
}

void CPPMAnalyzerSettings::UpdateInterfacesFromSettings()
{
    mInputChannelInterface->SetChannel(mInputChannel);
}

void CPPMAnalyzerSettings::LoadSettings(const char *settings)
{
    SimpleArchive text_archive;
    text_archive.SetString(settings);

    text_archive >> mInputChannel;

    ClearChannels();
    AddChannel(mInputChannel, "Simple CPPM Analyzer", true);

    UpdateInterfacesFromSettings();
}

const char *CPPMAnalyzerSettings::SaveSettings()
{
    SimpleArchive text_archive;

    text_archive << mInputChannel;

    return SetReturnString(text_archive.GetString());
}
