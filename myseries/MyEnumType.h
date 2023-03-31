#pragma once

/*
	@�޶��������ö�����Ͷ���
	@author:mayabin
	@date:2023/03/15
	@description:�޶��������ö�ٱ���������"������::������"�ķ�ʽʹ�øñ���;��Ϊ�ñ��������������{}����
*/

//�����ļ�Ŀ¼�Ĳ㼶���
enum class LOOP_DEPTH {
	CURRENT_PATH,
	ALL_PATH
};

//ʱ�����ʱ�伶��
enum class TSTAMP_LEVEL {
	TS_NANOSEC,		//nano second
	TS_MICROSEC,	//micro second
	TS_MILLISEC,	//milli second
	TS_SECOND,		//second
	TS_MINUTES,		//minutes
	TS_HOUR			//hour
};

//�ļ���С��ȡֵ��λ
enum class FSIZE_UNIT {
	FUNIT_BYTE,
	FUNIT_KB,
	FUNIT_MB,
	FUNIT_GB
};

//json����չ���㼶
enum class EXPAND_LEVEL {
	FIRST_LEVEL,	//չ����һ�㼶
	SECOND_LEVEL	//չ�����в㼶
};
