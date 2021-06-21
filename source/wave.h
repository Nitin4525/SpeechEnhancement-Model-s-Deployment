//
// Created by Nitin on 2021/6/3.
//

#ifndef WAVETEST_WAVE_H
#define WAVETEST_WAVE_H

#include <string>
#include <vector>
#include <algorithm>

#ifdef __unix
#include <cstring>
#define fopen_s(pFile,filename,mode) ((*(pFile))=fopen((filename),  (mode)))==NULL
#endif

using namespace std;

struct WaveDate{
    int32_t size;
    vector<float> samples;
};

struct WaveHeader{
    /*头部信息*/
    char chunkId[4];
    int32_t chunkSize;
    char format[4];
    char subChunk1Id[4];
    int32_t subChunk1Size;
    uint16_t audioFormat;
    uint16_t numChannels;
    int32_t sampleRate;
    int32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
    char subChunk2Id[4];
    int32_t subChunk2Size;
};


pair<WaveHeader, WaveDate> getData(const string& filePath);
bool saveData(pair<WaveHeader, WaveDate> wave, const string& savePath);
vector<float> filter(const vector<float>& b, const vector<float>& a, const vector<float>& x);
vector<vector<float>> getFrames(const vector<float>& x, int winSize, int hopSize);
vector<float> framesReconstruct(const vector<vector<float>>& frames, int overlapSize, int Size);
#endif //WAVETEST_WAVE_H
