#include "fb_types.h"
#include <sstream>
#include <string>
#include <algorithm>
#include <fstream>

std::string generateSVG(const FuncBlock* fbType, const Config& cfg) {
    std::ostringstream svg;
    
    if (!fbType) return "";
    
    // Рассчитаем общее количество портов с каждой стороны
    int leftPortsCount = fbType->eventInputs.size() + fbType->inputVars.size();
    int rightPortsCount = fbType->eventOutputs.size() + fbType->outputVars.size();
    
    // Если нет портов с какой-то стороны, используем минимум 1 для расчетов
    int calcPortsLeft = std::max(leftPortsCount, 1);
    int calcPortsRight = std::max(rightPortsCount, 1);
    int maxPorts = std::max(calcPortsLeft, calcPortsRight);
    
    // Высота блока зависит от максимального количества портов
    int blockHeight = cfg.margin * 2 + maxPorts * cfg.portSpacing;
    blockHeight = std::max(blockHeight, cfg.height); // Не меньше минимальной
    
    // Увеличим ширину SVG, чтобы текст помещался с обеих сторон
    // Левая часть для текста входных портов: примерно 250px
    // Правая часть для текста выходных портов: примерно 250px
    // Плюс сам блок 250px + отступы
    int svgWidth = 250 + cfg.width + 250; // 250 слева + блок + 250 справа
    int svgHeight = blockHeight + 100;
    
    // Центрируем блок на холсте
    int rectX = 250; // Сдвигаем блок вправо, чтобы слева было место для текста
    int rectY = 50;
    
    svg << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    svg << "<svg width=\"" << svgWidth << "\" height=\"" << svgHeight 
        << "\" xmlns=\"http://www.w3.org/2000/svg\">\n";
    
    // Основной прямоугольник блока
    svg << "  <rect x=\"" << rectX << "\" y=\"" << rectY << "\" width=\"" 
        << cfg.width << "\" height=\"" << blockHeight << "\" rx=\"10\" ry=\"10\" fill=\"" 
        << cfg.blockColor << "\" stroke=\"" << cfg.borderColor << "\" stroke-width=\"2\"/>\n";
    
    // Название блока в центре
    int textX = rectX + cfg.width / 2;
    int textY = rectY + blockHeight / 2;
    svg << "  <text x=\"" << textX << "\" y=\"" << textY 
        << "\" text-anchor=\"middle\" dominant-baseline=\"middle\" font-size=\"" 
        << cfg.textSize << "\" fill=\"" << cfg.textColor << "\" font-family=\"Arial\">"
        << fbType->name << "</text>\n";
    
    // Вспомогательная функция для добавления порта
    auto addPort = [&](int index, int totalPorts, bool isLeft, bool isEvent, 
                       const std::string& portName, const std::string& portType, 
                       const std::string& comment) {
        
        // Координата Y порта (равномерное распределение)
        int yPos = rectY + cfg.margin + (blockHeight - 2 * cfg.margin) * (index + 1) / (totalPorts + 1);
        
        // Координата X кружка на границе
        int circleX = isLeft ? rectX : rectX + cfg.width;
        
        // Цвет кружка в зависимости от типа порта
        std::string portColor = isEvent ? cfg.eventPortColor : cfg.dataPortColor;
        
        // Кружок порта
        svg << "  <circle cx=\"" << circleX << "\" cy=\"" << yPos << "\" r=\"" 
            << cfg.portRadius << "\" fill=\"" << portColor << "\" stroke=\"" 
            << cfg.borderColor << "\" stroke-width=\"1\"/>\n";
        
        // Горизонтальная линия (30px) ВНЕ БЛОКА - немного длиннее
        int lineStartX, lineEndX;
        int lineLength = 30; // Увеличил длину линии
        if (isLeft) {
            // Для входных: линия идет ВЛЕВО от кружка (за пределы блока)
            lineStartX = circleX - cfg.portRadius;
            lineEndX = lineStartX - lineLength;
        } else {
            // Для выходных: линия идет ВПРАВО от кружка (за пределы блока)
            lineStartX = circleX + cfg.portRadius;
            lineEndX = lineStartX + lineLength;
        }
        
        svg << "  <line x1=\"" << lineStartX << "\" y1=\"" << yPos << "\" x2=\"" 
            << lineEndX << "\" y2=\"" << yPos << "\" stroke=\"" << cfg.borderColor 
            << "\" stroke-width=\"1\"/>\n";
        
        // Текст порта
        if (isLeft) {
            // ВХОДНЫЕ СЛЕВА: комментарий - тип данных [линия ВНЕ БЛОКА] [кружок] название порта
            // 1. Текст слева от линии (комментарий - тип) - больше места слева
            std::string leftText;
            if (!comment.empty()) {
                leftText = comment + " - " + portType;
            } else {
                leftText = portType;
            }
            
            // Текст выравниваем по правому краю, ставим еще левее
            int textLeftX = lineEndX - 10; // Отступ от конца линии
            
            svg << "  <text x=\"" << textLeftX << "\" y=\"" << yPos 
                << "\" text-anchor=\"end\" dominant-baseline=\"middle\" font-size=\"" 
                << cfg.textSize << "\" fill=\"" << cfg.textColor << "\" font-family=\"Arial\">"
                << leftText << "</text>\n";
            
            // 2. Название порта справа от кружка (внутри блока)
            svg << "  <text x=\"" << (circleX + 15) << "\" y=\"" << yPos 
                << "\" text-anchor=\"start\" dominant-baseline=\"middle\" font-size=\"" 
                << cfg.textSize << "\" fill=\"" << cfg.textColor << "\" font-family=\"Arial\">"
                << portName << "</text>\n";
                
        } else {
            // ВЫХОДНЫЕ СПРАВА: название порта [кружок] [линия ВНЕ БЛОКА] тип данных - комментарий
            // 1. Название порта слева от кружка (внутри блока)
            svg << "  <text x=\"" << (circleX - 15) << "\" y=\"" << yPos 
                << "\" text-anchor=\"end\" dominant-baseline=\"middle\" font-size=\"" 
                << cfg.textSize << "\" fill=\"" << cfg.textColor << "\" font-family=\"Arial\">"
                << portName << "</text>\n";
            
            // 2. Текст справа от линии (тип - комментарий) - больше места справа
            std::string rightText;
            if (!comment.empty()) {
                rightText = portType + " - " + comment;
            } else {
                rightText = portType;
            }
            
            // Текст выравниваем по левому краю, ставим еще правее
            int textRightX = lineEndX + 10; // Отступ от конца линии
            
            svg << "  <text x=\"" << textRightX << "\" y=\"" << yPos 
                << "\" text-anchor=\"start\" dominant-baseline=\"middle\" font-size=\"" 
                << cfg.textSize << "\" fill=\"" << cfg.textColor << "\" font-family=\"Arial\">"
                << rightText << "</text>\n";
        }
    };
    
    // Добавляем входные порты (слева)
    int portIndex = 0;
    
    // EventInputs сначала
    for (const auto& event : fbType->eventInputs) {
        addPort(portIndex++, leftPortsCount, true, true, 
                event.name, "EVENT", event.comment);
    }
    
    // InputVars после
    for (const auto& var : fbType->inputVars) {
        addPort(portIndex++, leftPortsCount, true, false, 
                var.name, var.type, var.comment);
    }
    
    // Добавляем выходные порты (справа)
    portIndex = 0;
    
    // EventOutputs сначала
    for (const auto& event : fbType->eventOutputs) {
        addPort(portIndex++, rightPortsCount, false, true, 
                event.name, "EVENT", event.comment);
    }
    
    // OutputVars после
    for (const auto& var : fbType->outputVars) {
        addPort(portIndex++, rightPortsCount, false, false, 
                var.name, var.type, var.comment);
    }
    
    svg << "</svg>";
    return svg.str();
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