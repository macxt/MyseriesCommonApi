#pragma once
#include <vector>
#include <map>

/*
	@���峣�õ����ͱ���������г�ͻ���ڿ�������Լ��������ռ�
	@author:mayabin
	@date:2023/03/15
	@description:�޶��������ö�ٱ���������"������::������"�ķ�ʽʹ�øñ���;��Ϊ�ñ��������������{}����
*/

using GEB_UChar = unsigned char;
using GEB_UShort = unsigned short;
using GEB_Short = short;
using GEB_UInt = unsigned int;
using GEB_Int = int;
using GEB_Float32 = float;
using GEB_Float64 = double;
using GEB_LL = long long;
using GEB_ULL = unsigned long long;

using MapStrStrT = std::map<std::string, std::string>;
using MapStrMapStrStrT = std::map<std::string, MapStrStrT>;

using VecStr = std::vector<std::string>;
using VecMapStrStrT = std::vector<MapStrStrT>;

