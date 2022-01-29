#ifndef structures_hpp
#define structures_hpp
#include <string>
struct Node
{
	int id = 0;
	bool value = false;
};
struct Gate
{
	std::string gateType;
	int firstInputNodeId = 0;
	int secondInputNodeId = 0;
	int outputNodeId = 0;
};
#endif