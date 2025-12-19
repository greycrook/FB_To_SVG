#include "fb_types.h"

#include <fstream>
#include <algorithm>
#include <string>

int calculateBlockHeight(const FuncBlock* fbType, const Config& config) {
    int leftPorts  = static_cast<int>(fbType->eventInputs.size()  + fbType->inputVars.size());
    int rightPorts = static_cast<int>(fbType->eventOutputs.size() + fbType->outputVars.size());
    int maxPorts   = std::max(leftPorts, rightPorts);

    return config.margin * 2 + std::max(1, maxPorts) * config.portSpacing;
}

std::string generateSVG(const FuncBlock* fbType, const Config& config) {
    const int leftCommentX   = 10;
    const int leftTypeX      = 230;
    const int leftLineStartX = 300;
    const int blockStartX    = 330;
    const int portNameLeftX  = blockStartX + 10;

    const int blockWidth     = 360;
    const int blockEndX      = blockStartX + blockWidth;

    const int portNameRightX = blockEndX - 10;
    const int rightCircleX   = blockEndX;
    const int rightLineEndX  = rightCircleX + 40;
    const int rightTypeX     = rightLineEndX + 50;
    const int rightCommentX  = rightTypeX + 180; 

    const int blockHeight = calculateBlockHeight(fbType, config);
    const int totalWidth  = rightCommentX + 20;
    const int totalHeight = blockHeight + 20;

    const int textYOffset = config.textSize / 3;

    std::string svg;
    svg += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    svg += "<svg xmlns=\"http://www.w3.org/2000/svg\" "
           "width=\""  + std::to_string(totalWidth)  +
           "\" height=\"" + std::to_string(totalHeight) + "\">\n";

    svg += "  <rect x=\"" + std::to_string(blockStartX) +
           "\" y=\"10\" width=\"" + std::to_string(blockWidth) +
           "\" height=\"" + std::to_string(blockHeight) +
           "\" rx=\"8\" ry=\"8\" fill=\"" + config.blockColor +
           "\" stroke=\"" + config.borderColor +
           "\" stroke-width=\"2\" />\n";

    std::string displayName = fbType->name;
    svg += "  <text x=\"" +
           std::to_string(blockStartX + blockWidth / 2) +
           "\" y=\"" + std::to_string(10 + blockHeight / 2 + textYOffset) +
           "\" text-anchor=\"middle\" font-size=\"" +
           std::to_string(config.textSize * 2 / 3) +
           "\" font-family=\"Arial, sans-serif\" font-weight=\"bold\" "
           "fill=\"" + config.textColor + "\">" +
           displayName + "</text>\n";

    int currentY = 10 + config.margin;

    for (size_t i = 0; i < fbType->eventInputs.size(); ++i) {
        const auto& ev = fbType->eventInputs[i];

        if (!ev.comment.empty()) {
            svg += "  <text x=\"" + std::to_string(leftCommentX) +
                   "\" y=\"" + std::to_string(currentY + textYOffset) +
                   "\" text-anchor=\"start\" font-size=\"" +
                   std::to_string(config.textSize) +
                   "\" font-family=\"Arial, sans-serif\" fill=\"" +
                   config.textColor + "\">" + ev.comment + "</text>\n";
        }

        svg += "  <text x=\"" + std::to_string(leftTypeX) +
               "\" y=\"" + std::to_string(currentY + textYOffset) +
               "\" text-anchor=\"start\" font-size=\"" +
               std::to_string(config.textSize) +
               "\" font-family=\"Arial, sans-serif\" fill=\"" +
               config.textColor + "\">EVENT</text>\n";

        svg += "  <line x1=\"" + std::to_string(leftLineStartX) +
               "\" y1=\"" + std::to_string(currentY) +
               "\" x2=\"" + std::to_string(blockStartX - 10) +
               "\" y2=\"" + std::to_string(currentY) +
               "\" stroke=\"" + config.borderColor +
               "\" stroke-width=\"1\" />\n";

        svg += "  <circle cx=\"" + std::to_string(blockStartX) +
               "\" cy=\"" + std::to_string(currentY) +
               "\" r=\"" + std::to_string(config.portRadius) +
               "\" fill=\"" + config.eventPortColor +
               "\" stroke=\"" + config.borderColor +
               "\" stroke-width=\"1\" />\n";

        svg += "  <text x=\"" + std::to_string(portNameLeftX) +
               "\" y=\"" + std::to_string(currentY + textYOffset) +
               "\" text-anchor=\"start\" font-size=\"" +
               std::to_string(config.textSize) +
               "\" font-family=\"Arial, sans-serif\" fill=\"" +
               config.textColor + "\">" + ev.name + "</text>\n";

        currentY += config.portSpacing;
    }

    for (size_t i = 0; i < fbType->inputVars.size(); ++i) {
        const auto& p = fbType->inputVars[i];

        svg += "  <text x=\"" + std::to_string(leftTypeX) +
               "\" y=\"" + std::to_string(currentY + textYOffset) +
               "\" text-anchor=\"start\" font-size=\"" +
               std::to_string(config.textSize) +
               "\" font-family=\"Arial, sans-serif\" fill=\"" +
               config.textColor + "\">" + p.type + "</text>\n";

        svg += "  <line x1=\"" + std::to_string(leftLineStartX) +
               "\" y1=\"" + std::to_string(currentY) +
               "\" x2=\"" + std::to_string(blockStartX - 10) +
               "\" y2=\"" + std::to_string(currentY) +
               "\" stroke=\"" + config.borderColor +
               "\" stroke-width=\"1\" />\n";

        svg += "  <circle cx=\"" + std::to_string(blockStartX) +
               "\" cy=\"" + std::to_string(currentY) +
               "\" r=\"" + std::to_string(config.portRadius) +
               "\" fill=\"" + config.dataPortColor +
               "\" stroke=\"" + config.borderColor +
               "\" stroke-width=\"1\" />\n";

        svg += "  <text x=\"" + std::to_string(portNameLeftX) +
               "\" y=\"" + std::to_string(currentY + textYOffset) +
               "\" text-anchor=\"start\" font-size=\"" +
               std::to_string(config.textSize) +
               "\" font-family=\"Arial, sans-serif\" fill=\"" +
               config.textColor + "\">" + p.name + "</text>\n";

        currentY += config.portSpacing;
    }

    currentY = 10 + config.margin;

    for (size_t i = 0; i < fbType->eventOutputs.size(); ++i) {
        const auto& ev = fbType->eventOutputs[i];

        svg += "  <text x=\"" + std::to_string(portNameRightX) +
               "\" y=\"" + std::to_string(currentY + textYOffset) +
               "\" text-anchor=\"end\" font-size=\"" +
               std::to_string(config.textSize) +
               "\" font-family=\"Arial, sans-serif\" fill=\"" +
               config.textColor + "\">" + ev.name + "</text>\n";

        svg += "  <circle cx=\"" + std::to_string(rightCircleX) +
               "\" cy=\"" + std::to_string(currentY) +
               "\" r=\"" + std::to_string(config.portRadius) +
               "\" fill=\"" + config.eventPortColor +
               "\" stroke=\"" + config.borderColor +
               "\" stroke-width=\"1\" />\n";

        svg += "  <line x1=\"" + std::to_string(rightCircleX + 10) +
               "\" y1=\"" + std::to_string(currentY) +
               "\" x2=\"" + std::to_string(rightLineEndX) +
               "\" y2=\"" + std::to_string(currentY) +
               "\" stroke=\"" + config.borderColor +
               "\" stroke-width=\"1\" />\n";

        svg += "  <text x=\"" + std::to_string(rightTypeX) +
               "\" y=\"" + std::to_string(currentY + textYOffset) +
               "\" text-anchor=\"start\" font-size=\"" +
               std::to_string(config.textSize) +
               "\" font-family=\"Arial, sans-serif\" fill=\"" +
               config.textColor + "\">EVENT</text>\n";

        if (!ev.comment.empty()) {
            svg += "  <text x=\"" + std::to_string(rightCommentX) +
                   "\" y=\"" + std::to_string(currentY + textYOffset) +
                   "\" text-anchor=\"start\" font-size=\"" +
                   std::to_string(config.textSize) +
                   "\" font-family=\"Arial, sans-serif\" fill=\"" +
                   config.textColor + "\">" + ev.comment + "</text>\n";
        }

        currentY += config.portSpacing;
    }

    for (size_t i = 0; i < fbType->outputVars.size(); ++i) {
        const auto& p = fbType->outputVars[i];

        svg += "  <text x=\"" + std::to_string(portNameRightX) +
               "\" y=\"" + std::to_string(currentY + textYOffset) +
               "\" text-anchor=\"end\" font-size=\"" +
               std::to_string(config.textSize) +
               "\" font-family=\"Arial, sans-serif\" fill=\"" +
               config.textColor + "\">" + p.name + "</text>\n";

        svg += "  <circle cx=\"" + std::to_string(rightCircleX) +
               "\" cy=\"" + std::to_string(currentY) +
               "\" r=\"" + std::to_string(config.portRadius) +
               "\" fill=\"" + config.dataPortColor +
               "\" stroke=\"" + config.borderColor +
               "\" stroke-width=\"1\" />\n";

        svg += "  <line x1=\"" + std::to_string(rightCircleX + 10) +
               "\" y1=\"" + std::to_string(currentY) +
               "\" x2=\"" + std::to_string(rightLineEndX) +
               "\" y2=\"" + std::to_string(currentY) +
               "\" stroke=\"" + config.borderColor +
               "\" stroke-width=\"1\" />\n";

        svg += "  <text x=\"" + std::to_string(rightTypeX) +
               "\" y=\"" + std::to_string(currentY + textYOffset) +
               "\" text-anchor=\"start\" font-size=\"" +
               std::to_string(config.textSize) +
               "\" font-family=\"Arial, sans-serif\" fill=\"" +
               config.textColor + "\">" + p.type + "</text>\n";

        currentY += config.portSpacing;
    }

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