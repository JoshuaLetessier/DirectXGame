#pragma once
#include "Script.h"
#include <vector>
using namespace std;

class scriptManager
{
public:
	scriptManager();
	~scriptManager();

	template <typename T>
	T* createNewScript();

	vector<Script*> scriptList;

};

template <typename T>
T* scriptManager::createNewScript()
{
	T* newScript = new T();
	scriptList.push_back((Script*)newScript);
	return newScript;
}