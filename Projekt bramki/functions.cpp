
#include "structures.hpp"
#include <vector>
#include <string>
#include <cctype>
#include <fstream>
#include <iostream>
/** Funkcja symuluje zachowanie bramki logicznej poprzez porównanie dwóch wartości podanych w parametrach
@first pierwszy węzeł bramki logicznej
@second  drugi węzeł bramki logicznej
@return zwracana wartość logiczna po wykonaniu obliczenia
*/
bool AndGate(bool first, bool second)
{
	if (first == true && second == true)
		return true;
	else
		return false;
}
bool OrGate(bool first, bool second)
{
	if (first == true || second == true)
		return true;
	else
		return false;
}
bool XorGate(bool first, bool second)
{
	if (first == second)
		return false;
	else
		return true;
}
bool NegGate(bool first)
{
	if (first == true)
		return false;
	else
		return true;
}
bool NandGate(bool first, bool second)
{
	if (first == true && second == true)
		return false;
	else
		return true;
}
bool NorGate(bool first, bool second)
{
	if (first == false && second == false)
		return true;
	else
		return false;
}
bool NxorGate(bool first, bool second)
{
	if (first == second)
		return true;
	else
		return false;
}
/** Funkcja dokonuje analizy z jaka bramka obecnie jest rozpatrywana
@gate  obecnie sprawdzana bramka
@firstInput pierwszy węzeł bramki
@secondInput drugi węzeł bramki
@return zwracane ID bramki razem z wartością
*/
Node evaluateGate(Gate gate, Node firstInput, Node secondInput)
{
	bool value;
	if (gate.gateType == "AND") {
		value = AndGate(firstInput.value, secondInput.value);
	}	
	else if (gate.gateType == "NAND") {
		value = NandGate(firstInput.value, secondInput.value);
	}
	else if (gate.gateType == "OR") {
		value = OrGate(firstInput.value, secondInput.value);
	}
	else if (gate.gateType == "XOR") {
		value = XorGate(firstInput.value, secondInput.value);
	}
	else if (gate.gateType == "NOR") {
		value = NorGate(firstInput.value, secondInput.value);
	}
	else if (gate.gateType == "NXOR") {
		value = NxorGate(firstInput.value, secondInput.value);
	}
	else if (gate.gateType == "NEG") {
		value = NegGate(firstInput.value);
	}
return { gate.outputNodeId,value };
}
/** funkcja przeszukuje vector węzłów pod względem szukanego ID węzła
@nodeId Unikalny numer dla każdego z węzłów
@nodes vector węzłów z którego będzie szukany 
@return jeżeli zostanie odnaleziony, węzeł zostanie zwrócony, w przeciwnym wypadku zostanie zwrócona wartość -1
*/
Node findNode(int nodeId, std::vector <Node> nodes)
{
	for (int nodeIndex = 0; nodeIndex < nodes.size(); nodeIndex++)
	{
		Node node = nodes[nodeIndex];
		if (node.id == nodeId)	return node;
	}
	return { -1, false }; //node with id -1 means that we have not found a node
}
/** funkcja pobiera "surowe dane" podane w parametrach i na ich podstawie tworzy bramkę, w przypadku bramki typu NEG zostaje wykonana metoda różniąca się o brak jednego wejścia
@gateData jest vectorem stringów z informacjami o bramce która ma zostać utworzona
@return bramka w postaci: gate = {string GateType,int FirstNodeId,int SecondNodeId,int OutputId} dla bramki NEG postać jest następująca gate = {string GateType,int FirstNodeId,int OutputId}
*/
Gate assignGate(std::vector <std::string> gateData)
{	 
	if (gateData[0] == "NEG") {
		Gate gate = { gateData[0],stoi(gateData[1]),-1,stoi(gateData[2]) };
		return gate;
	}
	else {
		Gate gate = { gateData[0],stoi(gateData[1]),stoi(gateData[2]),stoi(gateData[3]) };
		return gate;
	}
}
/** funkcja odczytuje dane z pliku tekstowego i zapisuje je do vectora
@PATH ścieżka pliku tekstowego
@return bramka w postaci: gate = {string GateType,int FirstNodeId,int SecondNodeId,int OutputId} dla bramki NEG postać jest następująca gate = {string GateType,int FirstNodeId,int OutputId}
*/
std::vector <std::string> readFile(const std::string PATH)
{
	std::ifstream file(PATH);
	std::vector <std::string> result;
	if (file.is_open())
	{
		char w[10000];
		while (file.getline(w, 10000))
		{
			std::string line = std::string(w);
			result.push_back(line);
		}
	}
	else
	{
		std::cout << "error 404";
	}
	file.close();
	return result;
}
/** funkcja zapisuje dane z vectora do pliku tekstowego w postaci omówionej w sprawozdaniu. Jeżeli wszystko przebiegnie pomyślnie program powiadomi użytkownika komunikatem
@inputVector jest vectorem z danymi które mają zostać zapisane

*/
void saveFIle(std::vector <std::string> inputVector, std::vector <Node> output, const std::string PATH)
{
	std::ofstream file(PATH);
	if (file.is_open())
	{
		for (int secondFileLinesIndex = 0; secondFileLinesIndex < inputVector.size(); secondFileLinesIndex++)
		{
			std::string line = inputVector[secondFileLinesIndex];
			int inputSize = line.size();
			file << "IN: ";
			for (int lineIndex = 0; lineIndex < inputSize; lineIndex++) file << line[lineIndex];
			file << " OUT: ";
			file << output[secondFileLinesIndex].id << ":";
			if (output[secondFileLinesIndex].value)
				file << "true";
			else
				file << "false";
			file << std::endl;
		}
	}
	file.close();
	std::cout << "File has been saven correctly" << std::endl;
}
/** funkcja odczytuje surowe dane ze stringa i przetwarza jest pod względem usunięcia znaków(dokłady opis jakich znajduje się w sprawozdaniu) które uniemożliwiają prosty odczyt z zapisanego przez nią vectora
@stringToSplit string który będzie przetwarzany
@return zwraca vector result z przetworzonymi informacjami 
*/
std::vector <std::string> splitString(std::string stringToSplit)
{
	static int lineIndex = 0; // index of line in input file
	int stringLenght = stringToSplit.length();
	int strIndexBehindFirstSeparator = 0;
	char firstSeparator = ':';
	char secondSeparator = ' ';
	bool firstSepIsFound = false;
	bool ifend = false; // if gatetype is in one string 
	bool fileType = true; // for fileType = true, function is splitting first file. for fileType = false function is splitting second file
	std::string gateType = "";
	std::vector <std::string> result;

	for (int stringIndex = 0; stringIndex < stringLenght; stringIndex++)
	{
		if ((isalpha(stringToSplit[stringIndex]) == true))
		{
			fileType = true;
			break;
		}
		else
			fileType = false;
	}
	if (fileType)
	{
		for (int stringIndex = 0; stringIndex < stringLenght; stringIndex++)
		{
			if (stringToSplit[stringIndex] == firstSeparator)
			{
				strIndexBehindFirstSeparator = stringIndex + 1;
				firstSepIsFound = true;
				break;
			}
			else
				firstSepIsFound = false;
		}
	}
	while (firstSepIsFound == false && lineIndex >= 2 && stringToSplit[strIndexBehindFirstSeparator] != secondSeparator && fileType)
	{
		gateType = gateType + stringToSplit[strIndexBehindFirstSeparator];
		strIndexBehindFirstSeparator++;
		ifend = true;
	}
	if (ifend == true)
	{
		result.push_back(gateType);
	}
	for (strIndexBehindFirstSeparator; strIndexBehindFirstSeparator < stringLenght; strIndexBehindFirstSeparator++)
	{
		if (stringToSplit[strIndexBehindFirstSeparator] != secondSeparator && stringToSplit[strIndexBehindFirstSeparator] != firstSeparator && (isalpha(stringToSplit[strIndexBehindFirstSeparator]) == false))
		{
			std::string finalString;
			finalString = stringToSplit[strIndexBehindFirstSeparator];
			result.push_back(finalString);
		}
	}
	lineIndex++;
	return result;
}
/** funkcja wypisuje informację o błędzie podczas odczytu pliku i instrukcję poprawnego utworzenia pliku wejściowego
*/
void help()
{
	std::cout << "Error during reading input files" << std::endl;
	std::cout << "---------------------------------------------------------------------------------------------------------------------" << std::endl;
	std::cout << "Instruction how to use program correctly: programName.exe -u firstFilePath.txt -i secondFilePath.txt -o resultsFliePath.txt" << std::endl;
	std::cout << "---------------------------------------------------------------------------------------------------------------------" << std::endl;
	std::cout << "first file has to look:" << std::endl;
	std::cout << "first line:" << "IN: " << "(example nodes) 1 2 3 4" << std::endl;
	std::cout << "second line:" << "OUT: " << "(one example output node of module) 7" << std::endl;
	std::cout << "next lines (3rd and more)" << "(example gate type) AND" << "(two example input nodes) 1 5 (one example output node) 6" << std::endl;
	std::cout << "every node must match with other parts to being able calculated" << std::endl;
	std::cout << "-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
	std::cout << "Each line in second file has to look:" << std::endl;
	std::cout << "(first input node Id)1" << ":" << "(first input node value which can be 0 = false or 1 = true)0" << " " << "next input nodes looks like same first one" << "last node is always output and its value is always equal zero" << std::endl;
	std::cout << "-----------------------------------------------------------------------------------------------------------------------" << std::endl;
}
/** funkcja sprawdza czy w argumentach konsoli znajdują się odpowiednie przełączniki **/
int checkArguments(std::vector<std::string> paths) {
	if (paths.size() == 7) {
		if (paths[1] == "-u" && paths[3] == "-i" && paths[5] == "-o") {
			return 1;
		}
		else {
			help();
			return -1;
		}
		
	}
	else
	{
		help();
		return -1;
	}
	}


