/*
** EPITECH PROJECT, 2020
** OOP_nanotekspice_2019
** File description:
** TODO: add description
*/

#include "../../../include/parser/Parser.hpp"
#include "../../../include/errors/Errors.hpp"
#include <iostream>
#include <csignal>
#include <cstdlib>
#include <algorithm>

void nts::Parser::prompt()
{
    std::string command;

    /*
    std::unique_ptr<nts::IComponent>output = _circuit.createOutput("B");
    std::unique_ptr<nts::IComponent>output2 = _circuit.createOutput("A");
    std::unique_ptr<nts::IComponent>input = _circuit.createInput("A");
    _circuit.addOutput(output2);
    _circuit.addOutput(output);
    _circuit.addInput(input);
    */

    signal(SIGINT, signalHandler);
    this->displayPrompt();
    while (std::getline(std::cin, command) && !_exit) {
        this->cleanCommand(command);
        if (_cmdMap.find(command) != _cmdMap.end())
            (this->*_cmdMap[command])(command);
        else if (command.find("=") != std::string::npos)
            (this->*_cmdMap["changeValueInput"])(command);
        else
            std::cout << "Unknow command" << std::endl;
        if (_exit) break;
        this->displayPrompt();
    }
}

void nts::Parser::cleanCommand(std::string &command)
{
    command.erase(std::remove(command.begin(), command.end(), ' '), command.end());
    command.erase(std::remove(command.begin(), command.end(), '\t'), command.end());
}

void nts::Parser::displayPrompt() const
{
    std::cout << "> ";
}

void nts::Parser::signalHandler(int signum)
{
    nts::Parser::_loop = false;
    (void)signum;
}