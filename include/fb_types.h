#pragma once
#include <string>
#include <vector>


struct PortData {
    std::string name;
    std::string type;
    std::string comment;
};

struct Event {
    std::string name;
    std::string comment;
};

struct FuncBlock {
    std::string name;
    std::string comment;
    
    std::vector<Event> eventInputs;
    std::vector<Event> eventOutputs;
    std::vector<PortData> inputVars;
    std::vector<PortData> outputVars;
};

struct Config {
    int width = 250;
    int height = 180;
    int margin = 20;
    int portSpacing = 25;
    int portRadius = 5;
    int textSize = 12;
    
    std::string blockColor = "#F1F1F1";
    std::string borderColor = "#333333";
    std::string textColor = "#000000";
    std::string eventPortColor = "#FF6B6B";
    std::string dataPortColor = "#4ECDC4";
};