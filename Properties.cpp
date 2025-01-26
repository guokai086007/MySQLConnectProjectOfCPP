#include "Properties.h"

namespace fs = std::filesystem;
//构造函数
Properties::Properties()
{
	this->props = new map<string, string>();
	this->keys = new set<string>();
}
//析构函数
Properties::~Properties() {
	delete props;
	delete keys;
}
//设置配置文件路径
void Properties::setConfigFilePath(filesystem::path path) {
	this->config_path = path;
}

//判断配置文件是否存在
bool Properties::is_exists() {
	return fs::exists(this->config_path);
}

//去掉字符串前后的多余空格
void Properties::trim(string& s)//引用类型参数，传入的是哪个字符串对象就修改哪个字符串对象
{
    if (!s.empty())
    {
        s.erase(0, s.find_first_not_of(" "));//去掉字符串s前面的多余空格
        s.erase(s.find_last_not_of(" ") + 1);//再去掉后面的多余空格
    }
}
//将str字符串用pattern分割后存入vector动态数组中，最后返回分割后的vector
vector<string> Properties::split(const string& str, char pattern)
{
    vector<string> res;
    stringstream input(str);
    string temp;
    while (getline(input, temp, pattern))
    {
        res.push_back(temp);
    }
    return res;
}

/*读取这个Properties类对象的对象空间中的config_path字段所对应的配置文件的信息。
* 将成员变量config_path所以对应的.properties文件中的键值对儿都读入到this->props中
* 并把所有键都存放到this->keys，这样之后这个Properties类对象的对象空间中的props和keys两个集合里面就有了所有的关于
* 成员变量config_path所以对应的.properties文件的配置信息了。
* */
void Properties::load(std::string defaultValue = "None") {
    std::ifstream file(this->config_path);
    std::string line;
    while (getline(file, line)) {
        trim(line);
        //去空行
        if (line.empty() || line == "\r" || line[0] == '#')
        {
            continue;
        }
        //处理等号后为空的配置
        vector<string> res = split(line, '=');
        if (res.size() < 2)
        {
            res[1] = defaultValue;
        }
        int t = res[1].find("#");
        if (t != string::npos) {
            res[1].erase(t);
        }
        for_each(res.begin(), res.end(), [=](string& s)mutable {
            trim(s);
            });
        this->props->insert(make_pair(res[0], res[1]));
        this->keys->insert(res[0]);
    }
    file.close();
}

set<string>* Properties::getKeys() const {
    return this->keys;
}

map<string, string>* Properties::getProps() const {
    return this->props;
}

//根据键在this->props中读取值，如果键不存在就返回默认值defaultValue
string Properties::getValue(const string& key, const string& defaultValue) {
    if (props->find(key) == props->end())
    {
        return defaultValue;
    }
    string value = this->props->at(key);
    return value;
}

string Properties::getValue(const string& key)
{
    if (props->find(key) == props->end())
    {
        cerr << "Properties::getValue方法通过key获取值失败！！\n";
        exit(1);
    }
    string value = this->props->at(key);
    return value;
}

bool Properties::setValue(const string& key, const string& Value) {
    if (props->find(key) == props->end())
    {
        this->props->insert(make_pair(key, Value));
    }
    else {
        props->at(key) = Value;
    }

    std::ofstream outFile(this->config_path);
    for (const auto& pair : *props) {
        outFile << pair.first << " = " << pair.second << std::endl;
    }
    return true;
}






