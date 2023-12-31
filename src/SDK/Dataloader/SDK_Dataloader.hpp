#ifndef SDK_Dataloader_hpp
#define SDK_Dataloader_hpp
#include "../Core/SDK_Core.hpp"
#include <vector>
namespace SDK_Dataloader {
struct fdata {
    std::string name;
    unsigned long long startpoint;
    unsigned long long endpoint;
    SDK_Core::RGB color;
    fdata() = default;
    fdata(const fdata& theFdata)
    {
        this->name = theFdata.name;
        this->startpoint = theFdata.startpoint;
        this->endpoint = theFdata.endpoint;
        this->color = theFdata.color;
    }
    fdata(const std::string& name, const unsigned long long startpoint, const unsigned long long endpoint, const SDK_Core::RGB& color)
    {
        this->name = name;
        this->startpoint = startpoint;
        this->endpoint = endpoint;
        this->color = color;
    }
    operator std::string();
    ~fdata() = default;
};

struct pdata {
    std::string name;
    double startpoint;
    double endpoint;
    SDK_Core::RGB color;
    pdata() = default;
    pdata(const pdata& theFdata)
    {
        this->name = theFdata.name;
        this->startpoint = theFdata.startpoint;
        this->endpoint = theFdata.endpoint;
        this->color = theFdata.color;
    }
    pdata(const std::string& name, const double startpoint, const double endpoint, const SDK_Core::RGB& color)
    {
        this->name = name;
        this->startpoint = startpoint;
        this->endpoint = endpoint;
        this->color = color;
    }
    operator std::string();
    ~pdata() = default;
};
std::vector<fdata> readdata(const std::string& pathway);
std::vector<pdata> normalize_data(const std::vector<fdata>& thefdata);
fdata Reline(const std::string& text);
SDK_Core::RGB changeCharToRGB(const char* text);
}

#endif