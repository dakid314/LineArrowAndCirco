#include "../include/cmdline.h"
#include "SDK/Dataloader/SDK_Dataloader.hpp"
#include "SDK/Drawer/SDK_Drawer.hpp"
#include <algorithm>
#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <vector>
bool fileExists(const std::string& filename)
{
    std::ifstream file(filename);
    return file.good();
}
namespace {



std::map<std::string, std::string> initFolder(const std::string& tmp_dirpath = "./", const std::string& data_dirpath = "./")
{
    namespace fs = std::experimental::filesystem;
    std::map<std::string, std::string> ph_of_dir;

    ph_of_dir["data"] = data_dirpath + "/data/";
    ph_of_dir["setting"] = data_dirpath + "/setting/";
    ph_of_dir["out"] = data_dirpath + "/out/";
    ph_of_dir["tmp"] = tmp_dirpath + "/tmp/";
    for (auto& i : ph_of_dir)
    {
        if (!fileExists(i.second)) {
            fs::create_directories(i.second);
        }
    }

    return ph_of_dir;
}
}

decltype(initFolder()) all_path;

int main(int argc, char** argv)
{
    std::cout << "\033[1;33m _     _             ___      _" << std::endl
              << "| |   (_)_ __   ___ ( _ )    / \\   _ __ _ __ _____      __" << std::endl
              << "| |   | | '_ \\ / _ \\/ _ \\/\\ / _ \\ | '__| '__/ _ \\ \\ /\\ / /" << std::endl
              << "| |___| | | | |  __/ (_>  </ ___ \\| |  | | | (_) \\ V  V /" << std::endl
              << "|_____|_|_| |_|\\___|\\___/\\/_/   \\_\\_|  |_|  \\___/ \\_/\\_/" << std::endl
              << "\033[0m" << std::endl;

    std::cout << "\033[1;33m     ____               _" << std::endl
              << "    |  _ \\             | |" << std::endl
              << "    | |_) |_   _       | |  _ __ ___ " << std::endl
              << "    |  _ <| | | |  _   | | | '_ ` _ \\ " << std::endl
              << "    | |_) | |_| | | |__| | | | | | | |" << std::endl
              << "    |____/ \\__, |  \\____/  |_| |_| |_|" << std::endl
              << "            __/ |" << std::endl
              << "           |___/" << std::endl
              << "\033[0m" << std::endl;
    cmdline::parser cmdconfig;
    cmdconfig.set_program_name("LineAndArrow");
    cmdconfig.add<std::string>("input", 'i', "Path to Input File", true);
    cmdconfig.add<std::string>("output", 'o', "Name of Output.", true);
    cmdconfig.add<double>("width", 'w', "Width Of the Arrow", false, 20.0);
    cmdconfig.add<double>("fontsize", 'f', "Size Of the Font", false, SDK_Draw::annotate_font_size);
    cmdconfig.add("stdout", 's', "Print SVG in STDOUT.");

    cmdconfig.add<std::string>("datapath", '\000', "Work Path", false, "./");
    cmdconfig.add<std::string>("tmppath", '\000', "Tmp Path", false, "./");
    cmdconfig.parse_check(argc, argv);
    SDK_Draw::annotate_font_size = cmdconfig.get<double>("fontsize");
    all_path = initFolder(cmdconfig.get<std::string>("tmppath"), cmdconfig.get<std::string>("datapath"));

    cr::cairo_surface_t* surfacepng = cr::cairo_image_surface_create(cr::CAIRO_FORMAT_ARGB32, SDK_Draw::PageW, SDK_Draw::PageH);

    std::string filemane = std::string(cmdconfig.get<std::string>("output"));
    cr::cairo_surface_t* surfacepdf = cr::cairo_pdf_surface_create((filemane + ".pdf").c_str(), SDK_Draw::PageW, SDK_Draw::PageH);
    cr::cairo_surface_t* circo_surfacepdf = cr::cairo_pdf_surface_create((filemane + "_circo.pdf").c_str(), SDK_Draw::circo_PageW, SDK_Draw::circo_PageH);
    cr::cairo_surface_t* surfacesvg = cr::cairo_svg_surface_create((filemane + ".svg").c_str(), SDK_Draw::PageW, SDK_Draw::PageH);

    SDK_Draw::make_line(surfacepng, SDK_Core::NormalColor::GRy);
    SDK_Draw::make_line(surfacepdf, SDK_Core::NormalColor::GRy);
    SDK_Draw::make_line(surfacesvg, SDK_Core::NormalColor::GRy);
    SDK_Draw::make_circo(circo_surfacepdf);

    std::string inputPath = cmdconfig.get<std::string>("input");
    
    
    if (!fileExists(inputPath)) {
        std::cout << "\033[1;32m" << inputPath << " Not exist. \033[0m" << std::endl;
        throw std::logic_error(inputPath + " Not exist.");
    }
    std::vector<SDK_Dataloader::fdata> data = SDK_Dataloader::readdata(cmdconfig.get<std::string>("input"));
    auto a = SDK_Dataloader::normalize_data(data);

    for (const auto& element : a) {
        SDK_Draw::arrow(surfacepng, element.startpoint, element.endpoint, element.color, SDK_Core::SIZE2D({ SDK_Draw::PageW, SDK_Draw::PageH }), cmdconfig.get<double>("width"));
        SDK_Draw::annotate(surfacepng, (element.startpoint + element.endpoint) / 2, SDK_Core::SIZE2D({ SDK_Draw::PageW, SDK_Draw::PageH }), element.name.c_str(), cmdconfig.get<double>("width"), SDK_Draw::UpandDown(element.startpoint, element.endpoint));
        SDK_Draw::arrow(surfacepdf, element.startpoint, element.endpoint, element.color, SDK_Core::SIZE2D({ SDK_Draw::PageW, SDK_Draw::PageH }), cmdconfig.get<double>("width"));
        SDK_Draw::annotate(surfacepdf, (element.startpoint + element.endpoint) / 2, SDK_Core::SIZE2D({ SDK_Draw::PageW, SDK_Draw::PageH }), element.name.c_str(), cmdconfig.get<double>("width"), SDK_Draw::UpandDown(element.startpoint, element.endpoint));
        SDK_Draw::arrow(surfacesvg, element.startpoint, element.endpoint, element.color, SDK_Core::SIZE2D({ SDK_Draw::PageW, SDK_Draw::PageH }), cmdconfig.get<double>("width"));
        SDK_Draw::annotate(surfacesvg, (element.startpoint + element.endpoint) / 2, SDK_Core::SIZE2D({ SDK_Draw::PageW, SDK_Draw::PageH }), element.name.c_str(), cmdconfig.get<double>("width"), SDK_Draw::UpandDown(element.startpoint, element.endpoint));
        SDK_Draw::circo_arrow(circo_surfacepdf, element.startpoint, element.endpoint, element.color);
        SDK_Draw::circo_annotate(circo_surfacepdf, (element.startpoint + element.endpoint) / 2, element.name.c_str());
    }

    cr::cairo_surface_write_to_png(surfacepng, (filemane + ".png").c_str());
    cr::cairo_surface_destroy(surfacepng);
    cr::cairo_surface_destroy(surfacepdf);
    cr::cairo_surface_destroy(surfacesvg);
    cr::cairo_surface_destroy(circo_surfacepdf);

    return 0;
}
