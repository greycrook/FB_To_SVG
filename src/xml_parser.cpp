#include "fb_types.h"
#include <pugixml.hpp>
#include <iostream>
#include <vector>

Event parseEventInfo(const pugi::xml_node& eventNode) 
{
    Event event;
    event.name = eventNode.attribute("Name").as_string();
    event.comment = eventNode.attribute("Comment").as_string();
    return event;
}

PortData parsePortInfo(const pugi::xml_node& varNode) 
{
    PortData var;
    var.name = varNode.attribute("Name").as_string();
    var.type = varNode.attribute("Type").as_string();
    var.comment = varNode.attribute("Comment").as_string();
    return var;
}

void parseEventList(pugi::xml_node parentNode, const std::string& childName, std::vector<Event>& eventList)
{
    pugi::xml_node eventsNode = parentNode.child(childName.c_str());
    if (eventsNode) {
        for (pugi::xml_node eventNode : eventsNode.children("Event")) {
            eventList.push_back(parseEventInfo(eventNode));
        }
    }
}

void parsePortList(pugi::xml_node parentNode, const std::string& childName, std::vector<PortData>& varList) 
{
    pugi::xml_node varsNode = parentNode.child(childName.c_str());
    if (varsNode) {
        for (pugi::xml_node varNode : varsNode.children("VarDeclaration")) {
            varList.push_back(parsePortInfo(varNode));
        }
    }
}

FuncBlock* parseFuncBlock(const std::string& filename) 
{
    pugi::xml_document doc;
    
    if (!doc.load_file(filename.c_str())) {
        std::cout << "Error while loading file " << filename << std::endl;
        return nullptr;
    }
    
    pugi::xml_node fbTypeNode = doc.child("FBType");
    if (!fbTypeNode) {
        std::cout << "It's not FB! " << std::endl;
        return nullptr;
    }
    
    FuncBlock* fbType = new FuncBlock();
    
    fbType->name = fbTypeNode.attribute("Name").as_string();
    fbType->comment = fbTypeNode.attribute("Comment").as_string();
    
    pugi::xml_node interfaceList = fbTypeNode.child("InterfaceList");
    if (interfaceList) {
        parseEventList(interfaceList, "EventInputs", fbType->eventInputs);
        parseEventList(interfaceList, "EventOutputs", fbType->eventOutputs);
        parsePortList(interfaceList, "InputVars", fbType->inputVars);
        parsePortList(interfaceList, "OutputVars", fbType->outputVars);
    }
    
    return fbType;
}

void freeFBType(FuncBlock* fbType) {
    delete fbType;
}
