// ADOConn.h: interface for the CADOConn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADOCONN_H__27D0B44B_258B_4AE0_B715_83BA8B1883E8__INCLUDED_)
#define AFX_ADOCONN_H__27D0B44B_258B_4AE0_B715_83BA8B1883E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CADOConn  
{
	//��ӳ�Ա����
public:
	_RecordsetPtr m_pRecordset;//��¼��ָ��
	_ConnectionPtr m_pConnection;//���ݿ�����ָ��

public:
	CADOConn();
	virtual ~CADOConn();
	void OnInitADOConn();//��������Դ
	_RecordsetPtr& GetRecordset(_bstr_t bstrSQL);//��ȡ�򿪵ļ�¼��ָ��
	BOOL ExecuteSQL(_bstr_t bstrSQL);//ִ��sql���
	void ExitConnect();//�Ͽ�����


};

#endif // !defined(AFX_ADOCONN_H__27D0B44B_258B_4AE0_B715_83BA8B1883E8__INCLUDED_)
