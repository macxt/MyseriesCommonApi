# MyseriesCommonApi

#该模块以 "myseries" 命名，按照自己的理解实现了一些通用接口功能分享出来供大家学习使用，后续还会不断的更新；具体包含以下几方面：

1. 字符串的处理
2. 文件及文件夹处理
3. 时间的处理
4. utf8及gb2312之间编码转换的处理
5. json字符串解析处理
6. 线程池的实现
7. 自定义日志系统的实现

#用法：只需在自己的工程中包含 #include "myseries_basic.hpp"  即可

#如有任何问题或发现bug，可联系 mayb@geovis.com.cn


DemoCode

日志信息的输出：输出类似下面的代码即可将日志信息打印到终端并自动换行；

MYLOG_INFO << "Remain " << m_SubmitTaskQueue.TaskSize() << " tasks to be processed.";


多任务并行执行线程池的使用： 只需构造一个线程池对象，并调用该线程池对外提供的唯一接口 CommitTask() 即可，如下：

MyThreadPool poolObj(nThreadNum);

poolObj.CommitTask(passInFunc, FuncParam1, FuncParam2, ...);    //在该对象被析构时线程自动阻塞直至任务函数执行完成


# Update Date: 2023/04/25

# Update Content: 

	增加了整个工程中算子处理的基础框架，具体算子需要进行注册、继承基类并重写两个纯虚函数完成相应功能的实现，充分利用了C++多态的特性。