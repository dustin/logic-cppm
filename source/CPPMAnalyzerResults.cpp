#include "CPPMAnalyzerResults.h"
#include <AnalyzerHelpers.h>
#include "CPPMAnalyzer.h"
#include "CPPMAnalyzerSettings.h"
#include <iostream>
#include <fstream>

CPPMAnalyzerResults::CPPMAnalyzerResults(CPPMAnalyzer *analyzer, CPPMAnalyzerSettings *settings)
    :   AnalyzerResults(),
        mSettings(settings),
        mAnalyzer(analyzer)
{
}

CPPMAnalyzerResults::~CPPMAnalyzerResults()
{
}

void CPPMAnalyzerResults::GenerateBubbleText(U64 frame_index, Channel &channel, DisplayBase display_base)
{
    ClearResultStrings();
    Frame frame = GetFrame(frame_index);

    char channel_str[8];
    snprintf(channel_str, sizeof(channel_str), "%d", frame.mData2);
    char number_str[128];
    snprintf(number_str, sizeof(number_str), "%d", frame.mData1);
    AddResultString(channel_str, ":", number_str, " μS");
    AddResultString(channel_str, ":", number_str);
    AddResultString(number_str);
}

void CPPMAnalyzerResults::wideExport(const char *file)
{
    std::ofstream file_stream(file, std::ios::out);

    U64 num_frames = GetNumFrames();

    U64 trigger_sample = mAnalyzer->GetTriggerSample();
    U32 sample_rate = mAnalyzer->GetSampleRate();

    int nchans = 0;
    int prevchan = 0;
    for (U32 i = 0; i < num_frames; i++) {
        Frame frame = GetFrame(i);
        if (frame.mData2 < prevchan) {
            break;
        }
        nchans++;
        prevchan = frame.mData2;
    }

    file_stream << "Time [s]";
    for (int i = 0; i < nchans; i++) {
        file_stream << ",ch" << (i+1);
    }
    file_stream << ",error" << std::endl;

    for (U32 i = 0; i < num_frames; i+=nchans) {
        Frame frame = GetFrame(i);
        int flags = 0;

        char time_str[128];
        char number_str[128];
        AnalyzerHelpers::GetTimeString(frame.mStartingSampleInclusive, trigger_sample, sample_rate, time_str, 128);

        file_stream << time_str;

        for (int j = 0; j < nchans; j++) {
            if (i+j >= num_frames) {
                file_stream << ",";
                continue;
            }

            Frame frame = GetFrame(i+j);
            flags |= frame.mFlags;

            // Don't include bogus readings
            if (frame.mFlags != 0) {
                i++;
                j--;
                continue;
            }

            snprintf(number_str, sizeof(number_str), "%d", frame.mData1);
            file_stream << "," << number_str;

            if (UpdateExportProgressAndCheckForCancel(i+j, num_frames) == true) {
                file_stream.close();
                return;
            }
        }

        file_stream << (flags == 0 ? "," : ",error") << std::endl;
    }

    file_stream.close();
}

void CPPMAnalyzerResults::longExport(const char *file)
{
    std::ofstream file_stream(file, std::ios::out);

    U64 num_frames = GetNumFrames();

    U64 trigger_sample = mAnalyzer->GetTriggerSample();
    U32 sample_rate = mAnalyzer->GetSampleRate();

    file_stream << "Time [s],val,chan,error" << std::endl;

    for (U32 i = 0; i < num_frames; i++) {
        Frame frame = GetFrame(i);

        char time_str[128];
        char number_str[128];
        AnalyzerHelpers::GetTimeString(frame.mStartingSampleInclusive, trigger_sample, sample_rate, time_str, 128);

        file_stream << time_str << "," << frame.mData1 << "," << frame.mData2
                    << (frame.mFlags == 0 ? "," : ",error")
                    << std::endl;
        if (UpdateExportProgressAndCheckForCancel(i, num_frames) == true) {
            file_stream.close();
            return;
        }
    }

    file_stream.close();
}

void CPPMAnalyzerResults::GenerateExportFile(const char *file, DisplayBase display_base, U32 export_type_user_id)
{
    if (export_type_user_id == 0) {
        wideExport(file);
    } else {
        longExport(file);
    }
}

void CPPMAnalyzerResults::GenerateFrameTabularText(U64 frame_index, DisplayBase display_base)
{
    Frame frame = GetFrame(frame_index);
    ClearTabularText();

    if (frame.mType == 0) {
        return;
    }

    char number_str[128];
    snprintf(number_str, sizeof(number_str), "%d:%d", frame.mData2, frame.mData1);
    AddTabularText(number_str);
}

void CPPMAnalyzerResults::GeneratePacketTabularText(U64 packet_id, DisplayBase display_base)
{
    ClearResultStrings();
    AddResultString("not supported");
}

void CPPMAnalyzerResults::GenerateTransactionTabularText(U64 transaction_id, DisplayBase display_base)
{
    ClearResultStrings();
    AddResultString("not supported");
}
