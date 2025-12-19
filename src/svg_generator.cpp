#include "fb_types.h"
#include <fstream>
#include <algorithm>
#include <string>

int calculateBlockHeight(const FuncBlock* fbType, const Config& config) {

    int leftPorts = fbType->eventInputs.size() + fbType->inputVars.size();
    int rightPorts = fbType->eventOutputs.size() + fbType->outputVars.size();

    int maxPorts = std::max(leftPorts, maxPorts);

    int calculatedHeight = config.margin * 2 + maxPorts * config.portSpacing;


    return calculatedHeight;
}

std::string generateSVG(const FuncBlock* fbType, const Config& config) {
    
    int blockHeight = calculateBlockHeight(fbType, config);
    std::string svg = "";
    
    svg += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    svg += "<svg xmlns=\"http://www.w3.org/2000/svg\" ";
    svg += "width=\"" + std::to_string(config.width) + "\" ";
    svg += "height=\"" + std::to_string(blockHeight + 100) + "\">\n";
    
    svg += "  <rect x=\"0\" y=\"0\" ";
    svg += "width=\"" + std::to_string(config.width) + "\" ";
    svg += "height=\"" + std::to_string(blockHeight) + "\" ";
    svg += "rx=\"8\" ry=\"8\" ";
    svg += "fill=\"" + config.blockColor + "\" ";
    svg += "stroke=\"" + config.borderColor + "\" ";
    svg += "stroke-width=\"2\"/>\n";
    
    int currentY = config.margin + config.portSpacing;
    
    for (int i = 0; i < fbType->eventInputs.size(); i++) {
        svg += "  <line x1=\"-25\" y1=\"" + std::to_string(currentY) + "\" ";
        svg += "x2=\"0\" y2=\"" + std::to_string(currentY) + "\" ";
        svg += "stroke=\"" + config.borderColor + "\" stroke-width=\"1\"/>\n";
        
        svg += "  <circle cx=\"0\" cy=\"" + std::to_string(currentY) + "\" ";
        svg += "r=\"" + std::to_string(config.portRadius) + "\" ";
        svg += "fill=\"" + config.eventPortColor + "\" ";
        svg += "stroke=\"" + config.borderColor + "\" stroke-width=\"1\"/>\n";
        
        svg += "  <text x=\"15\" y=\"" + std::to_string(currentY) + "\" ";
        svg += "text-anchor=\"start\" font-size=\"" + std::to_string(config.textSize) + "\">";
        svg += fbType->eventInputs[i].name + "</text>\n";
        
        svg += "  <text x=\"-30\" y=\"" + std::to_string(currentY) + "\" ";
        svg += "text-anchor=\"end\" font-size=\"" + std::to_string(config.textSize - 2) + "\">EVENT</text>\n";
        
        if (!fbType->eventInputs[i].comment.empty()) {
            svg += "  <text x=\"-100\" y=\"" + std::to_string(currentY) + "\" ";
            svg += "text-anchor=\"end\" font-size=\"" + std::to_string(config.textSize - 2) + "\">";
            svg += fbType->eventInputs[i].comment + "</text>\n";
        }
        
        currentY += config.portSpacing;
    }
    
    for (int i = 0; i < fbType->inputVars.size(); i++) {
        svg += "  <line x1=\"-25\" y1=\"" + std::to_string(currentY) + "\" ";
        svg += "x2=\"0\" y2=\"" + std::to_string(currentY) + "\" ";
        svg += "stroke=\"" + config.borderColor + "\" stroke-width=\"1\"/>\n";
        
        svg += "  <circle cx=\"0\" cy=\"" + std::to_string(currentY) + "\" ";
        svg += "r=\"" + std::to_string(config.portRadius) + "\" ";
        svg += "fill=\"" + config.dataPortColor + "\" ";
        svg += "stroke=\"" + config.borderColor + "\" stroke-width=\"1\"/>\n";
        
        svg += "  <text x=\"15\" y=\"" + std::to_string(currentY) + "\" ";
        svg += "text-anchor=\"start\" font-size=\"" + std::to_string(config.textSize) + "\">";
        svg += fbType->inputVars[i].name + "</text>\n";
        
        svg += "  <text x=\"-30\" y=\"" + std::to_string(currentY) + "\" ";
        svg += "text-anchor=\"end\" font-size=\"" + std::to_string(config.textSize - 2) + "\">";
        svg += fbType->inputVars[i].type + "</text>\n";
        
        currentY += config.portSpacing;
    }
    
    currentY = config.margin + config.portSpacing;
    
    for (int i = 0; i < fbType->eventOutputs.size(); i++) {
        svg += "  <line x1=\"" + std::to_string(config.width) + "\" y1=\"" + std::to_string(currentY) + "\" ";
        svg += "x2=\"" + std::to_string(config.width + 25) + "\" y2=\"" + std::to_string(currentY) + "\" ";
        svg += "stroke=\"" + config.borderColor + "\" stroke-width=\"1\"/>\n";
        
        svg += "  <circle cx=\"" + std::to_string(config.width) + "\" cy=\"" + std::to_string(currentY) + "\" ";
        svg += "r=\"" + std::to_string(config.portRadius) + "\" ";
        svg += "fill=\"" + config.eventPortColor + "\" ";
        svg += "stroke=\"" + config.borderColor + "\" stroke-width=\"1\"/>\n";
        
        svg += "  <text x=\"" + std::to_string(config.width - 15) + "\" y=\"" + std::to_string(currentY) + "\" ";
        svg += "text-anchor=\"end\" font-size=\"" + std::to_string(config.textSize) + "\">";
        svg += fbType->eventOutputs[i].name + "</text>\n";
        
        svg += "  <text x=\"" + std::to_string(config.width + 30) + "\" y=\"" + std::to_string(currentY) + "\" ";
        svg += "text-anchor=\"start\" font-size=\"" + std::to_string(config.textSize - 2) + "\">EVENT</text>\n";
        
        currentY += config.portSpacing;
    }
    
    for (int i = 0; i < fbType->outputVars.size(); i++) {
        svg += "  <line x1=\"" + std::to_string(config.width) + "\" y1=\"" + std::to_string(currentY) + "\" ";
        svg += "x2=\"" + std::to_string(config.width + 25) + "\" y2=\"" + std::to_string(currentY) + "\" ";
        svg += "stroke=\"" + config.borderColor + "\" stroke-width=\"1\"/>\n";
        
        svg += "  <circle cx=\"" + std::to_string(config.width) + "\" cy=\"" + std::to_string(currentY) + "\" ";
        svg += "r=\"" + std::to_string(config.portRadius) + "\" ";
        svg += "fill=\"" + config.dataPortColor + "\" ";
        svg += "stroke=\"" + config.borderColor + "\" stroke-width=\"1\"/>\n";
        
        svg += "  <text x=\"" + std::to_string(config.width - 15) + "\" y=\"" + std::to_string(currentY) + "\" ";
        svg += "text-anchor=\"end\" font-size=\"" + std::to_string(config.textSize) + "\">";
        svg += fbType->outputVars[i].name + "</text>\n";
        
        svg += "  <text x=\"" + std::to_string(config.width + 30) + "\" y=\"" + std::to_string(currentY) + "\" ";
        svg += "text-anchor=\"start\" font-size=\"" + std::to_string(config.textSize - 2) + "\">";
        svg += fbType->outputVars[i].type + "</text>\n";
        
        currentY += config.portSpacing;
    }
    
    std::string displayName = fbType->name;
    
    svg += "  <text x=\"" + std::to_string(config.width / 2) + "\" y=\"" + std::to_string(blockHeight / 2) + "\" ";
    svg += "text-anchor=\"middle\" font-size=\"" + std::to_string(config.textSize + 2) + "\" ";
    svg += "font-weight=\"bold\">" + displayName + "</text>\n";
    
    svg += "</svg>\n";
    
    return svg;
}

bool saveSVGToFile(const std::string& filename, const std::string& svgContent) {
    
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        return false;
    }
    
    file << svgContent;
    file.close();
    
    return true;
}