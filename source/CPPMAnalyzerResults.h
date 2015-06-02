#ifndef CPPM_ANALYZER_RESULTS
#define CPPM_ANALYZER_RESULTS

#include <AnalyzerResults.h>

class CPPMAnalyzer;
class CPPMAnalyzerSettings;

class CPPMAnalyzerResults : public AnalyzerResults
{
public:
    CPPMAnalyzerResults(CPPMAnalyzer *analyzer, CPPMAnalyzerSettings *settings);
    virtual ~CPPMAnalyzerResults();

    virtual void GenerateBubbleText(U64 frame_index, Channel &channel, DisplayBase display_base);
    virtual void GenerateExportFile(const char *file, DisplayBase display_base, U32 export_type_user_id);

    virtual void GenerateFrameTabularText(U64 frame_index, DisplayBase display_base);
    virtual void GeneratePacketTabularText(U64 packet_id, DisplayBase display_base);
    virtual void GenerateTransactionTabularText(U64 transaction_id, DisplayBase display_base);

protected: //functions

    void wideExport(const char *file);
    void longExport(const char *file);

protected:  //vars
    CPPMAnalyzerSettings *mSettings;
    CPPMAnalyzer *mAnalyzer;
};

#endif //CPPM_ANALYZER_RESULTS
