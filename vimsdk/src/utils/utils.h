#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include "../imsdk/config.h"

namespace utils 
{

bool isUser(const int64);
bool isGroup(const int64);
bool isApp(const int64);

#define IS_MSG_USER		isUser
#define IS_MSG_GROUP	isGroup
#define IS_MSG_APP		isApp

//	字符串和其他类型互转函数
template<class T>
T convertFromString(const std::string& s){
	std::stringstream ss(s);
	T val;
	ss >> val;
	return val;
}

template<class T>
std::string convertToString(T val){
	std::stringstream ss;
	ss << val;
	return ss.str();
}

template<class T>
bool sort_bypinyin(const T& v1, const T& v2){
    return v1.pinyin < v2.pinyin;
}

void log(int level, const char* file, const char * format, ...);

std::vector<std::string> split(std::string src, std::string delimit);

}
