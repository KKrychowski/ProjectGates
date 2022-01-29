#ifndef functions_hpp
#define functions_hpp
#include <vector>
void help();
bool AndGate(bool first, bool second);
bool OrGate(bool first, bool second);
bool NorGate(bool first, bool second);
bool XorGate(bool first, bool second);
bool NandGate(bool first, bool second);
bool NxorGate(bool first, bool second);
bool NegGate(bool first);
void saveFIle(std::vector <std::string> inputVector, std::vector <Node> output, const std::string PATH);
std::vector <std::string> readFile(const std::string path);
std::vector <std::string> splitString(std::string str);
Node evaluateGate(Gate gate, Node firstInput, Node secondInput);
Node findNode(int nodeId, std::vector <Node> nodes);
Gate assignGate(std::vector <std::string> gateData);
int checkArguments(std::vector<std::string> paths);

#endif