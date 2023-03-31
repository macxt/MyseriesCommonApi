#pragma once

/*
	@定义常用的类型别名，如果有冲突后期可以添加自己的命名空间
	@author:mayabin
	@date:2023/03/15
	@description:限定作用域的枚举变量必须以"类型名::变量名"的方式使用该变量;因为该变量的作用域仅在{}以内
*/


using GIntBig = long long;
using GUIntBig = unsigned long long;

using MapStrStrT = std::map<std::string, std::string>;

using VecStr = std::vector<std::string>;
using VecMapStrStrT = std::vector<MapStrStrT>;

