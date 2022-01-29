
#include <iostream>
#include <fstream>
#include <vector>
#include<string>
#include<cctype>
#include "structures.hpp"
#include "functions.hpp"
using namespace std;

int main(int argc, char* argv[])
{
	vector <string> paths;
	for (int i = 0; i < argc; i++)
	{
		paths.push_back(argv[i]);
	}
	if (checkArguments(paths) == -1)
	{
		return -1;
	}
	const string FIRSTFILEPATH = paths[2]; //path to first file
		const string SECONDFILEPATH = paths[4]; // path to second file
		const string RESULTSFILEPATH = paths[6]; // path to results file
	vector <Gate> gates;
	vector <Node> nodes;
	vector <Node> results;
	vector <string> gatesData;
	vector <string> inputData;
	vector <string> firstFileLines;
	vector <string> secondFileLines;
	firstFileLines = readFile(FIRSTFILEPATH);
	secondFileLines = readFile(SECONDFILEPATH);
	int secondFIleVectorSize = secondFileLines.size();
	int outputNodeId = 0;
	bool firstGateIsNotNeg = true;
	
	for (int secondFileLinesIndex = 0; secondFileLinesIndex < secondFIleVectorSize; secondFileLinesIndex++)
	{
		secondFileLines = readFile(SECONDFILEPATH);
		secondFileLines = splitString(secondFileLines[secondFileLinesIndex]);
		for (int gatesDataIndex = 0; gatesDataIndex < firstFileLines.size(); gatesDataIndex++)
		{
			if (secondFileLines.size() == 2 && gatesDataIndex == 0)
			{
				gatesData = splitString(firstFileLines[0]);
				bool Value = stoi(secondFileLines[1]);
				nodes.push_back({ stoi(gatesData[0]),Value });
				firstGateIsNotNeg = false;
			}
			if (gatesDataIndex == 0 && firstGateIsNotNeg == true)
			{
				gatesData = splitString(firstFileLines[0]);

				for (int valuesIndex = 1, idIndex = 0,quantityOfInputNodes = secondFileLines.size()/2, currentNode=0; currentNode < quantityOfInputNodes; valuesIndex = valuesIndex + 2, idIndex++,currentNode++)
				{
					bool Value = stoi(secondFileLines[valuesIndex]);
					nodes.push_back({ stoi(gatesData[idIndex]),Value });
				}
			}
			else if (gatesDataIndex == 1)
			{
				gatesData = splitString(firstFileLines[1]);
				outputNodeId = stoi(gatesData[0]);
				gatesData.clear();
			}
			else if (gatesDataIndex > 1)
			{
				gatesData = splitString(firstFileLines[gatesDataIndex]);
				gates.push_back(assignGate(gatesData));
			}
		}
		for (int gateIndex = 0; gateIndex < gates.size(); gateIndex++)
		{
			Gate gate = gates[gateIndex];
			Node firstInput = findNode(gate.firstInputNodeId, nodes);
			Node secondInput = findNode(gate.secondInputNodeId, nodes);

			if (firstInput.id != -1 && secondInput.id != -1)
			{
				Node output = evaluateGate(gate, firstInput, secondInput);
				nodes.push_back(output);
			}
			else
			{
				if (gate.gateType == "NEG") {
					Node output = evaluateGate(gate, firstInput, secondInput);
					nodes.push_back(output);
				}
				else {
					cout << "Eror type: Check if files are consistent with switchers of main function args or if files have been written correctly" << endl;
					cout << "---------------------------------------------------------------------------------------------------------------------" << endl;
					help();
					return 0;
				}
			}
		}
		Node finalResult = findNode(outputNodeId, nodes);
		if (finalResult.id == -1) {
			cout << ("Cannot find OutputNode") << endl;		
			help();
			return 0;
		}
		results.push_back(finalResult);
		nodes.clear();
		gates.clear();
	}
	secondFileLines = readFile(SECONDFILEPATH);
	saveFIle(secondFileLines, results, RESULTSFILEPATH);
}
