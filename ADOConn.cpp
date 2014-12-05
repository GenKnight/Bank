// ADOConn.cpp: implementation of the CADOConn class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Bank.h"
#include "ADOConn.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CADOConn::CADOConn()
{

}

CADOConn::~CADOConn()
{

}
void CADOConn::OnInitADOConn()
{
	::CoInitialize(NULL);//��ʼ��com����
	try
	{
		m_pConnection.CreateInstance("ADODB.Connection");
		_bstr_t strConnect;
	//	strConnect="DSN=SQL Server;Provider=SQLOLEDB;Data Source=LI-PC;Initial Catalog=Bank.mdb;Integrated security=SSPI;UserID=sa;password=910722";
		strConnect="File Name=ADO.udl";
		m_pConnection->Open(strConnect,"","",adModeUnknown);
	}
	catch(_com_error e)
	{
		AfxMessageBox("�����ݿ�ʧ��");
		AfxMessageBox(e.ErrorMessage());
	}
}

void CADOConn::ExitConnect()
{
	if(m_pRecordset!=NULL)//�رռ�¼���ͶϿ�����Դ
		m_pRecordset->Close();
	m_pConnection->Close();
	::CoUninitialize();//�ͷ�com����
}

_RecordsetPtr& CADOConn::GetRecordset(_bstr_t bstrSQL)
{
	//�򿪼�¼�������ؼ�¼ָ��
	try
	{
		if(m_pConnection==NULL)
			OnInitADOConn();
		m_pRecordset.CreateInstance(__uuidof(Recordset));
		m_pRecordset->Open(bstrSQL,m_pConnection.GetInterfacePtr(),
			adOpenDynamic,adLockOptimistic,adCmdText);

	}
	catch(_com_error e)
	{
		AfxMessageBox("�򿪼�¼��ʧ��");
		AfxMessageBox(e.ErrorMessage());
	}
	return m_pRecordset;
}

BOOL CADOConn::ExecuteSQL(_bstr_t bstrSQL)
{
	try
	{
		if(m_pConnection==NULL)
			OnInitADOConn();
		m_pConnection->Execute(bstrSQL,NULL,adCmdText);
		return true;
	}
	catch(_com_error e)
	{
		CString str;
		str.Format("���ܴ򿪼�¼��!%s",e.ErrorMessage());
		AfxMessageBox(str);
		return false;
	}
}
