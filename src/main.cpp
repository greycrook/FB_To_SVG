#include "fb_types.h"
#include <iostream>

FuncBlock* parseFuncBlock(const std::string& filename);
void freeFBType(FuncBlock* fbType);
std::string generateSVG(const FuncBlock* fbType, const Config& config);
bool saveSVGToFile(const std::string& filename, const std::string& svgContent);

int main(int argc, char* argv[]) {

    std::string inputFile = argv[1];
    std::string outputFile;

    if (argc > 2) {
        outputFile = argv[2];
    } else {
        outputFile = "output.svg";
    }
    
    std::cout << "Parsing... " << std::endl;
    
    FuncBlock* fbType = parseFuncBlock(inputFile);
    
    if (!fbType) {
        std::cerr << "Error while parsing XML file " << std::endl;
        return 1;
    }
    
    std::cout << "Parsing was done : " << fbType->name << std::endl;
    
    Config config;
    
    std::cout << "Generating... " << std::endl;
    std::string svgContent = generateSVG(fbType, config);
    
    if (svgContent.empty()) {
        std::cerr << "Error while generating SVG" << std::endl;
        delete fbType;
        return 1;
    }
    
    std::cout << "Saving... " << outputFile << std::endl;
    
    if (saveSVGToFile(outputFile, svgContent)) {
        std::cout << "Your SVG was done : " << outputFile << std::endl;
    } else {
        std::cerr << "Error while saving SVG " << std::endl;
        delete fbType;
        return 1;
    }
    
    delete fbType;
    
    return 0;
}