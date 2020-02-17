/*
** EPITECH PROJECT, 2020
** OOP_nanotekspice_2019
** File description:
** TODO: add description
*/

#include "../../../include/parser/Parser.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>

void nts::Parser::loadFile()
{
    std::string line;
    size_t section = 0;
    while (_file.getDataLine(line)) {

        this->cutComment(line);

        if (line.empty())
            continue;
        try {
            if (this->detectPartFile(line, section))
                continue;
            if (section == 1)
                this->parseLineChipset(line);
            else
                this->parseLineLink(line);
        } catch (nts::NTSError const &e) {
            throw e;
        }
    }
}

bool nts::Parser::detectPartFile(std::string &line, size_t &section)
{
    if (line.find(".chipsets:") != std::string::npos) {
        if (section != 0)
            throw (FileError("Chipset Section already declared", "File"));
        else
            section = 1;
        return true;
    } else if (line.find(".links:") != std::string::npos) {
        if (section != 1)
            throw (FileError("No Chipset Section", "File"));
        else
            section = 2;
        return true;
    } else if (section == 0) {
        throw (FileError("No Chipset Section found", "File"));
    }
    return false;
}

void nts::Parser::parseLineLink(std::string &line)
{
    std::istringstream ss(line);
    std::string linked1;
    std::string linked2;
    size_t pinLink1;
    size_t pinLink2;

    ss >> linked1;
    ss >> linked2;

    if (linked2.empty())
        throw (FileError("Link : One or more links is missing", "File"));
    if (linked1.find(':') == std::string::npos || linked2.find(':') == std::string::npos)
        throw (FileError("Name or pin of linked component is missing for linkage", "File"));

    pinLink1 = std::stoi(linked1.substr(linked1.find(':') + 1));
    pinLink2 = std::stoi(linked2.substr(linked2.find(':') + 1));
    linked1.erase(linked1.find(':'));
    linked2.erase(linked2.find(':'));

    _circuit.setLink(linked1, pinLink1, linked2, pinLink2);
}

void nts::Parser::parseLineChipset(std::string &line)
{
    std::istringstream ss(line);
    std::string type;
    std::string name;
    std::unique_ptr<nts::IComponent> newComponent;

    ss >> type;
    ss >> name;

    if (name.empty())
        throw (FileError("Chipset : Type or name missing", "File"));

    newComponent = _circuit.createComponent(type, name);
    _circuit.addCircuit(newComponent);

    /*
    if (!type.compare("input"))
        _circuit.addInput(newComponent);
    else if (!type.compare("output"))
        _circuit.addOutput(newComponent);
    else if (!type.compare("clock"))
        _circuit.addClock(newComponent);
    else
        _circuit.addComponent(newComponent);
    _circuit.addCircuit(newComponent.get());
     */
}