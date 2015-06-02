#ifndef CPPM_ANALYZER_SETTINGS
#define CPPM_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

class CPPMAnalyzerSettings : public AnalyzerSettings
{
public:
    CPPMAnalyzerSettings();
    virtual ~CPPMAnalyzerSettings();

    virtual bool SetSettingsFromInterfaces();
    void UpdateInterfacesFromSettings();
    virtual void LoadSettings(const char *settings);
    virtual const char *SaveSettings();


    Channel mInputChannel;
    U32 mSyncTime;

protected:
    std::auto_ptr< AnalyzerSettingInterfaceChannel >    mInputChannelInterface;
    std::auto_ptr< AnalyzerSettingInterfaceInteger >    mSyncTimeInterface;
};

#endif //CPPM_ANALYZER_SETTINGS
