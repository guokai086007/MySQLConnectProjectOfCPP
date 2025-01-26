#include "Properties.h"

namespace fs = std::filesystem;
//���캯��
Properties::Properties()
{
	this->props = new map<string, string>();
	this->keys = new set<string>();
}
//��������
Properties::~Properties() {
	delete props;
	delete keys;
}
//���������ļ�·��
void Properties::setConfigFilePath(filesystem::path path) {
	this->config_path = path;
}

//�ж������ļ��Ƿ����
bool Properties::is_exists() {
	return fs::exists(this->config_path);
}

//ȥ���ַ���ǰ��Ķ���ո�
void Properties::trim(string& s)//�������Ͳ�������������ĸ��ַ���������޸��ĸ��ַ�������
{
    if (!s.empty())
    {
        s.erase(0, s.find_first_not_of(" "));//ȥ���ַ���sǰ��Ķ���ո�
        s.erase(s.find_last_not_of(" ") + 1);//��ȥ������Ķ���ո�
    }
}
//��str�ַ�����pattern�ָ�����vector��̬�����У���󷵻طָ���vector
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

/*��ȡ���Properties�����Ķ���ռ��е�config_path�ֶ�����Ӧ�������ļ�����Ϣ��
* ����Ա����config_path���Զ�Ӧ��.properties�ļ��еļ�ֵ�Զ������뵽this->props��
* �������м�����ŵ�this->keys������֮�����Properties�����Ķ���ռ��е�props��keys��������������������еĹ���
* ��Ա����config_path���Զ�Ӧ��.properties�ļ���������Ϣ�ˡ�
* */
void Properties::load(std::string defaultValue = "None") {
    std::ifstream file(this->config_path);
    std::string line;
    while (getline(file, line)) {
        trim(line);
        //ȥ����
        if (line.empty() || line == "\r" || line[0] == '#')
        {
            continue;
        }
        //����Ⱥź�Ϊ�յ�����
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

//���ݼ���this->props�ж�ȡֵ������������ھͷ���Ĭ��ֵdefaultValue
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
        cerr << "Properties::getValue����ͨ��key��ȡֵʧ�ܣ���\n";
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






