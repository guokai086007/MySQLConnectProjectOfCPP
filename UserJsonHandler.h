#pragma once
#include <string>
#include<vector>
#include<iomanip>
#include <filesystem>
#include <sstream>
#include <fstream>

#include "json.h" 
#include "UserManager.h"

using namespace std;
class UserJsonHandler
{
	public:
		static vector<User> readUserJsonFile(string path);
		static vector<User> readUserJsonString(string JsonString);
		static bool writeUsersToJsonFile(vector<User> users,string path);
		static bool writeUserToJsonFile(User user,string path);
};

