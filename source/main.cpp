//
// Created by Nitin on 2021/6/5.
//
#include <torch/script.h>
#include <iostream>

#include <ctime>

#include "wave.h"

int main() {
    clock_t startTime;
    startTime = clock();

    /* load models */
    string modelPath = R"(D:\CLionProjects\speech-enhancement\SEGAN.pt)";
    torch::jit::script::Module module;
    try
    {
        module = torch::jit::load(modelPath);
    }
    catch (const c10::Error& e)
    {
        std::cerr << "error loading the model\n";
        return -1;
    }
    cout << "The run time of loading model is: " <<(float)(clock() - startTime) / CLOCKS_PER_SEC << "s" << endl;
    startTime = clock();

    /* load audio file */
    string wavPath = R"(D:\DEMAND\test\noise\p232_001.wav)";
    pair<WaveHeader, WaveDate> wave = getData(wavPath);
    cout << "The run time of loading audio is: " <<(float)(clock() - startTime) / CLOCKS_PER_SEC << "s" << endl;
    startTime = clock();

    /* audio process */
    vector<float> b{1.0, -0.95};    // pre-emphasis factors
    vector<float> a{1.0};
    vector<vector<float>> frames = getFrames(wave.second.samples, 16384, 8192);     // separate frame
    cout << "The run time of getting frames is: " <<(float)(clock() - startTime) / CLOCKS_PER_SEC << "s" << endl;
    startTime = clock();
    vector<vector<float>>::iterator it;
    torch::Tensor z = torch::rand({1, 1024, 8});    // latent variable
    for(it=frames.begin(); it != frames.end();it++)
    {
        *it = filter(b, a, (*it));      // pre-emphasis
        torch::Tensor frame = torch::tensor((*it));
        frame = torch::unsqueeze(frame, 0);
        frame = torch::unsqueeze(frame, 0);
        std::vector<torch::jit::IValue> inputs = {frame};
        inputs.push_back(z);
        at::Tensor output = module.forward(inputs).toTensor();
        output = torch::squeeze(output, 0);
        output = torch::squeeze(output, 0);
        auto output_a = output.accessor<float, 1>();
        memcpy(&(*it)[0], output_a.data(), 16384*sizeof(float));    // tensor -> vector
        *it = filter(a, b, (*it));
    }
    cout << "The run time of model forward is: " <<(float)(clock() - startTime) / CLOCKS_PER_SEC << "s" << endl;
    startTime = clock();
    wave.second.samples = framesReconstruct(frames, 8192, wave.second.size);    // reconstruct frames
    cout << "The run time of reconstructing frames is: " <<(float)(clock() - startTime) / CLOCKS_PER_SEC << "s" << endl;
    startTime = clock();

    /* save file */
    string savePath = R"(D:\DEMAND\test\cpptest\p232_001.wav)";
    saveData(wave, savePath);
    cout << "The run time of saving result is: " <<(float)(clock() - startTime) / CLOCKS_PER_SEC << "s" << endl;

    return 0;
}
