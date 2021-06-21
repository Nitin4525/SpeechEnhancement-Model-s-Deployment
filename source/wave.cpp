//
// Created by Nitin on 2021/6/3.
//
#include <iostream>
#include "wave.h"


pair<WaveHeader, WaveDate> getData(const string& filePath)
{
    pair<WaveHeader, WaveDate> wave;

    FILE *fp;
    fopen_s(&fp, filePath.c_str(), "rb");
    if( fp == nullptr )
    {
        cout << "Can't Open  " << filePath << "... :o" << endl;
        exit(0);
    }

    /* get HEAD info */
    fread(&wave.first, sizeof(wave.first), 1, fp);
    if (strncmp(wave.first.chunkId, "RIFF", 4) || strncmp(wave.first.format, "WAVE", 4) || strncmp(wave.first.subChunk1Id, "fmt", 3) || strncmp(wave.first.subChunk2Id, "data", 4))
    {
        fclose(fp);
        cerr << "Unsupported file type.\n";
        exit(0);
    }
    else
    {
        /* cal size of sample points */
        wave.second.size = wave.first.subChunk2Size / (wave.first.byteRate / wave.first.sampleRate);

        /* get wave data */
        int16_t *data = (int16_t *) malloc(sizeof(int16_t) * wave.first.subChunk2Size);
        fread(data, sizeof(int16_t), wave.first.subChunk2Size, fp);
        fclose(fp);
        for(int i=0; i < wave.first.subChunk2Size; i++)
        {
            wave.second.samples.push_back(data[i] * 1.0 / 32768);   // make data to [-1, 1]
        }
        wave.second.samples.resize(wave.second.size);
        return wave;
    }
}

bool saveData(pair<WaveHeader, WaveDate> wave, const string& savePath)
{
    FILE *fp;
    fopen_s(&fp, savePath.c_str(), "wb+");
    if( fp == nullptr )
    {
        cout << "Can't Open  " << savePath << "... :o" << endl;
        exit(0);
    }

    /* save HEAD info */
    fwrite(&wave.first, sizeof(wave.first), 1, fp);

    /* save wave data */
    vector<int16_t> data ;
    for(float sample : wave.second.samples)
    {
        data.push_back(int16_t(sample * 32768));
    }
    fwrite(&data[0], sizeof(int16_t)*data.size(), 1, fp);
    fclose(fp);
    return true;
}

vector<float> filter(const vector<float>& b, const vector<float>& a, const vector<float>& x)
{
    /* a[0]*y[n] = b[0]*x[n] + b[1]*x[n-1] + ... + b[M]*x[n-M] - a[1]*y[n-1] - ... - a[N]*y[n-N] */
    /* assert a[0] = 1 */
    int M = b.size();
    int N = a.size();
    int n = x.size();

    if (a[0] != 1)
    {
        cerr << "a[0] must be 1";
        exit(0);
    }

    vector<float> y(n, 0);

    for(int i=0; i<n; i++)
    {
        for(int j=0; j<M&&(i-j+1); j++)
        {
            y[i] += b[j]*x[i-j];
        }
        for(int k=1; k<N&&(i-k+1); k++)
        {
            y[i] -= a[k]*y[i-k];
        }
    }
    return y;
}

vector<vector<float>> getFrames(const vector<float>& x, int winSize, int hopSize)
{
    int padSize = int(x.size()) + hopSize - ((int(x.size()) - winSize) % hopSize);
    vector<float> x_pad(x.begin(), x.end());
    x_pad.resize(padSize, 0);

    vector<vector<float>> frames;
    for (int i=0; i< int(x_pad.size())-winSize+1; i += hopSize)
    {
        vector<float> frame(x_pad.begin()+i, x_pad.begin()+i+winSize);
        frames.push_back(frame);
    }
    return frames;
}

vector<float> framesReconstruct(const vector<vector<float>>& frames, int overlapSize, int Size)
{
    vector<float> frame(frames[0].begin(), frames[0].end());

    for(int i=1; i<frames.size(); i++)
    {
        int frameSize = int(frame.size());
        frame.insert(frame.end(), frames[i].begin()+overlapSize, frames[i].end());
        for(int j=0; j<overlapSize; j++)
        {
            float  weight = j * 1.0 / overlapSize;
            frame[frameSize-overlapSize+j] = frame[frameSize-overlapSize+j]*(1.0-weight) + frames[i][j]*weight;
        }
    }
    frame.resize(Size);

    return frame;
}
