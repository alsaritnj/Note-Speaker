#include <sapi.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <locale>
using namespace std;

unsigned cdBetweenWords = 150;
unsigned cdBetweenThreeWords = 1000;
unsigned cdBetweenSentence = 3000;


void dictateThreeSentence(const wstring& sentence, ISpVoice* pVoice)
{
    if (sentence.empty())
        return;
    vector<wstring> words;
    wstringstream tempWstrs;
    wstring tempWstr;
    tempWstrs << sentence;
    while (getline(tempWstrs, tempWstr, L' '))
    {
        words.emplace_back(tempWstr);
    }

    pVoice->Speak(sentence.c_str(), 0, 0);
    wcout << sentence << endl;

    size_t i = 0;
    while (true)
    {
        pVoice->Speak(words[i].c_str(), 0, 0);

        pVoice->Speak(words[i + 1].c_str(), 0, 0);

        pVoice->Speak(words[i + 2].c_str(), 0, 0);

        pVoice->Speak(words[i].c_str(), 0, 0);
        Sleep(cdBetweenWords);

        pVoice->Speak(words[i + 1].c_str(), 0, 0);
        Sleep(cdBetweenWords);

        pVoice->Speak(words[i + 2].c_str(), 0, 0);

        i += 3;
        if (i == words.size())
        {
            Sleep(cdBetweenSentence);
            return;
        }
        Sleep(cdBetweenThreeWords);
        if (words.size() - i < 3)
        {
            break;
        }
    }
    for (size_t j = i; j < words.size(); j++)
    {
        pVoice->Speak(words[j].c_str(), 0, 0);
        Sleep(cdBetweenWords);
    }
    Sleep(cdBetweenThreeWords);
    for (size_t j = i; j < words.size(); j++)
    {
        pVoice->Speak(words[j].c_str(), 0, 0);
        Sleep(cdBetweenWords);
    }
    Sleep(cdBetweenSentence - cdBetweenWords);
}

int main()
{
    // some magic here
    std::locale system("");
    std::locale::global(system);

    vector<wstring> sentences;
    wstringstream tempWstrs;
    wstring tempWstr;
    tempWstrs << L"Поэтому, для нормальной работы динамической памяти требуется периодическая регенерация памяти (подзаряд конденсаторов), что усложняет электрическую схему работы динамической памяти.Но, поскольку, основополагающим требованием для оперативной памяти является ее объем, то динамическая память оказалась предпочтительней, несмотря на то, что она работает медленнее и имеет сложную схему управляющего контроллера.Статическая память нашла свое применение в кэш-памяти которая, имеет невысокий объем, но очень высокое быстродействие.";

    while (getline(tempWstrs, tempWstr, L'.'))
    {
        sentences.emplace_back(tempWstr);
    }

    // initalising com
    // also magic here
    ISpVoice* pVoice = NULL;

    if (FAILED(CoInitialize(NULL)))
        return 1;

    HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&pVoice);

    if (SUCCEEDED(hr))
    {
        for (auto& el : sentences)
        {
            dictateThreeSentence(el, pVoice);
        }

        pVoice->Release();
    }
    CoUninitialize();
    return 0;
}
