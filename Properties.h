#pragma once
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <algorithm>
#include <sstream>
#include <vector>
#include <set>
#include <filesystem>
using namespace std;
class Properties
{
    private:
        std::filesystem::path config_path;
        map<string, string>* props = nullptr;
        set<string>* keys = nullptr;
        void trim(string& s);
        vector<string> split(const string& str, char pattern);
    public:
        Properties();
        virtual ~Properties();
        void setConfigFilePath(filesystem::path path);
        bool is_exists();
        void load(string defaultValue);
        void load() { load("None");  };
        set<string>* getKeys() const;
        map<string, string>* getProps() const;
        string getValue(const string& key, const string& defaultValue);
        string getValue(const string& key);
        bool setValue(const string& key, const string& Value);

    public:
        static Properties* getInstance() {//µ¥ÀýÄ£Ê½
            static Properties properties;
            return &properties;
        }
};

