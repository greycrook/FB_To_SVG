#include "fb_types.h"
#include <string>
#include <algorithm>
#include <fstream>

void addPortToSVG(std::string& svg, int index, int totalPorts, bool isLeft, bool isEvent,
                  const std::string& portName, const std::string& portType, 
                  const std::string& comment, int rectX, int rectY, int blockHeight, 
                  int cfgWidth, const Config& cfg) {
    
    int yPos = rectY + cfg.margin + (blockHeight - 2 * cfg.margin) * (index + 1) / (totalPorts + 1);
    
    int circleX;
    if (isLeft) {
        circleX = rectX;
    } else {
        circleX = rectX + cfgWidth;
    }
    
    std::string portColor;
    if (isEvent) {
        portColor = cfg.eventPortColor;
    }
    else {
        portColor = cfg.dataPortColor;
    }


    svg += "  <circle cx=\"" + std::to_string(circleX) + 
           "\" cy=\"" + std::to_string(yPos) + 
           "\" r=\"" + std::to_string(cfg.portRadius) + 
           "\" fill=\"" + portColor + 
           "\" stroke=\"" + cfg.borderColor + "\" stroke-width=\"1\"/>\n";
    
    
    int lineStartX, lineEndX;
    int lineLength = 30;
    if (isLeft) {
        lineStartX = circleX - cfg.portRadius;
        lineEndX = lineStartX - lineLength;
    } else {
        lineStartX = circleX + cfg.portRadius;
        lineEndX = lineStartX + lineLength;
    }
    
    svg += "  <line x1=\"" + std::to_string(lineStartX) + 
           "\" y1=\"" + std::to_string(yPos) + 
           "\" x2=\"" + std::to_string(lineEndX) + 
           "\" y2=\"" + std::to_string(yPos) + 
           "\" stroke=\"" + cfg.borderColor + "\" stroke-width=\"1\"/>\n";
    
    if (isLeft) {
        std::string leftText;
        if (!comment.empty()) {
            leftText = comment + " - " + portType;
        } else {
            leftText = portType;
        }
        
        int textLeftX = lineEndX - 10;
        
        svg += "  <text x=\"" + std::to_string(textLeftX) + 
               "\" y=\"" + std::to_string(yPos) + 
               "\" text-anchor=\"end\" dominant-baseline=\"middle\" font-size=\"" + 
               std::to_string(cfg.textSize) + "\" fill=\"" + cfg.textColor + 
               "\" font-family=\"Arial\">" + leftText + "</text>\n";
        
        svg += "  <text x=\"" + std::to_string(circleX + 15) + 
               "\" y=\"" + std::to_string(yPos) + 
               "\" text-anchor=\"start\" dominant-baseline=\"middle\" font-size=\"" + 
               std::to_string(cfg.textSize) + "\" fill=\"" + cfg.textColor + 
               "\" font-family=\"Arial\">" + portName + "</text>\n";
            
    } else {
        svg += "  <text x=\"" + std::to_string(circleX - 15) + 
               "\" y=\"" + std::to_string(yPos) + 
               "\" text-anchor=\"end\" dominant-baseline=\"middle\" font-size=\"" + 
               std::to_string(cfg.textSize) + "\" fill=\"" + cfg.textColor + 
               "\" font-family=\"Arial\">" + portName + "</text>\n";
        
        std::string rightText;
        if (!comment.empty()) {
            rightText = portType + " - " + comment;
        } else {
            rightText = portType;
        }
        
        int textRightX = lineEndX + 10;
        
        svg += "  <text x=\"" + std::to_string(textRightX) + 
               "\" y=\"" + std::to_string(yPos) + 
               "\" text-anchor=\"start\" dominant-baseline=\"middle\" font-size=\"" + 
               std::to_string(cfg.textSize) + "\" fill=\"" + cfg.textColor + 
               "\" font-family=\"Arial\">" + rightText + "</text>\n";
    }
}

std::string generateSVG(const FuncBlock* fbType, const Config& cfg) {
    if (!fbType) return "";
    
    int leftPortsCount = fbType->eventInputs.size() + fbType->inputVars.size();
    int rightPortsCount = fbType->eventOutputs.size() + fbType->outputVars.size();
    
    int maxPorts = std::max(leftPortsCount, rightPortsCount);
    
    int blockHeight = cfg.margin * 2 + maxPorts * cfg.portSpacing;
    
    int svgWidth = 350 + cfg.width + 350;
    int svgHeight = blockHeight + 100;
    
    int rectX = 250;
    int rectY = 50;
    
    std::string svg = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    svg += "<svg width=\"" + std::to_string(svgWidth) + 
           "\" height=\"" + std::to_string(svgHeight) + 
           "\" xmlns=\"http://www.w3.org/2000/svg\">\n";
    
    svg += "  <rect x=\"" + std::to_string(rectX) + 
           "\" y=\"" + std::to_string(rectY) + 
           "\" width=\"" + std::to_string(cfg.width) + 
           "\" height=\"" + std::to_string(blockHeight) + 
           "\" rx=\"10\" ry=\"10\" fill=\"" + cfg.blockColor + 
           "\" stroke=\"" + cfg.borderColor + "\" stroke-width=\"2\"/>\n";
    
    int textX = rectX + cfg.width / 2;
    int textY = rectY + blockHeight / 2;
    svg += "  <text x=\"" + std::to_string(textX) + 
           "\" y=\"" + std::to_string(textY) + 
           "\" text-anchor=\"middle\" dominant-baseline=\"middle\" font-size=\"" + 
           std::to_string(cfg.textSize) + "\" fill=\"" + cfg.textColor + 
           "\" font-family=\"Arial\">" + fbType->name + "</text>\n";
    
    int portIndex = 0;
    
    for (int i = 0; i < fbType->eventInputs.size(); i++) {
        const Event& event = fbType->eventInputs[i];
        addPortToSVG(svg, portIndex, leftPortsCount, true, true,
                    event.name, "EVENT", event.comment,
                    rectX, rectY, blockHeight, cfg.width, cfg);
        portIndex++;
    }
    
    for (int i = 0; i < fbType->inputVars.size(); i++) {
        const PortData& var = fbType->inputVars[i];
        addPortToSVG(svg, portIndex, leftPortsCount, true, false,
                    var.name, var.type, var.comment,
                    rectX, rectY, blockHeight, cfg.width, cfg);
        portIndex++;
    }
    
    portIndex = 0;
    
    for (int i = 0; i < fbType->eventOutputs.size(); i++) {
        const Event& event = fbType->eventOutputs[i];
        addPortToSVG(svg, portIndex, rightPortsCount, false, true,
                    event.name, "EVENT", event.comment,
                    rectX, rectY, blockHeight, cfg.width, cfg);
        portIndex++;
    }
    
    for (int i = 0; i < fbType->outputVars.size(); i++) {
        const PortData& var = fbType->outputVars[i];
        addPortToSVG(svg, portIndex, rightPortsCount, false, false,
                    var.name, var.type, var.comment,
                    rectX, rectY, blockHeight, cfg.width, cfg);
        portIndex++;
    }
    
    svg += "</svg>";
    return svg;
}

bool saveSVGToFile(const std::string& filename, const std::string& svgContent) {
    std::ofstream out(filename);
    if (!out.is_open()) {
        return false;
    }
    out << svgContent;
    out.close();
    return true;
}