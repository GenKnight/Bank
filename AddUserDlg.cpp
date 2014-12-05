// AddUserDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Bank.h"
#include "AddUserDlg.h"
#include "RegisteDlg.h"
#include "ADOConn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddUserDlg dialog


CAddUserDlg::CAddUserDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddUserDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddUserDlg)
	m_AccountNum = _T("");
	m_Amount = 0.0;
	m_CustomerID = _T("");
	m_Password = _T("");
	m_Password2 = _T("");
	m_BranchName = _T("");
	//}}AFX_DATA_INIT
}


void CAddUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddUserDlg)
	DDX_Text(pDX, IDC_ACCOUNTNUM, m_AccountNum);
	DDX_Text(pDX, IDC_AMOUNT, m_Amount);
	DDX_Text(pDX, IDC_CUSTOMERID, m_CustomerID);
	DDX_Text(pDX, IDC_PASSWORD, m_Password);
	DDX_Text(pDX, IDC_PASSWORD2, m_Password2);
	DDX_Text(pDX, IDC_BRANCHNAME, m_BranchName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddUserDlg, CDialog)
	//{{AFX_MSG_MAP(CAddUserDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddUserDlg message handlers

void CAddUserDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	//���ͻ��Ƿ���ڣ��ж��˺��Ƿ��ظ��������Ƿ�Ϊ�գ�����Ƿ����0
	CRegisteDlg dlg;
	if(dlg.CheckCustomerID(m_CustomerID)==1)//��¼������
	{
		AfxMessageBox("�ÿͻ������ڣ���ע�����������");
		GetDlgItem(IDC_CUSTOMERID)->SetWindowText("");
		GetDlgItem(IDC_ACCOUNTNUM)->SetWindowText("");
		GetDlgItem(IDC_BRANCHNAME)->SetWindowText("");
		GetDlgItem(IDC_PASSWORD)->SetWindowText("");
		GetDlgItem(IDC_PASSWORD2)->SetWindowText("");
		GetDlgItem(IDC_AMOUNT)->SetWindowText("");
		return;
	}
	if(CheckAccountNum())
	{
		AfxMessageBox("���˺��Ѿ���ע���������������");
		GetDlgItem(IDC_ACCOUNTNUM)->SetWindowText("");
		GetDlgItem(IDC_BRANCHNAME)->SetWindowText("");
		GetDlgItem(IDC_PASSWORD)->SetWindowText("");
		GetDlgItem(IDC_PASSWORD2)->SetWindowText("");
		GetDlgItem(IDC_AMOUNT)->SetWindowText("");
		return;
	}
	if(!CheckBranch())
	{
		AfxMessageBox("�����в����ڣ�����������");
		GetDlgItem(IDC_BRANCHNAME)->SetWindowText("");
		GetDlgItem(IDC_PASSWORD)->SetWindowText("");
		GetDlgItem(IDC_PASSWORD2)->SetWindowText("");
		GetDlgItem(IDC_AMOUNT)->SetWindowText("");
		return;
	}
	if(m_Password!=m_Password2)
	{
		AfxMessageBox("������������벻һ�£�����������");
		GetDlgItem(IDC_PASSWORD)->SetWindowText("");
		GetDlgItem(IDC_PASSWORD2)->SetWindowText("");
		GetDlgItem(IDC_AMOUNT)->SetWindowText("");
		return;
	}
	if(m_Amount<=0)
	{
		AfxMessageBox("����Ľ��㣬����������");
		GetDlgItem(IDC_AMOUNT)->SetWindowText("");
		return;
	}
	InsertAccountNum();
	AfxMessageBox("�ÿͻ������ɹ�");
	CDialog::OnOK();
}

int CAddUserDlg::CheckAccountNum()
{
	_bstr_t vSQL;
	vSQL="select * from account where account_number='"+m_AccountNum+"'";
	CADOConn m_ADOConn;
	_RecordsetPtr m_Recordset;
	m_ADOConn.OnInitADOConn();
	m_Recordset=m_ADOConn.GetRecordset(vSQL);
	if(m_Recordset->adoEOF)//��¼����
	{
		m_ADOConn.ExitConnect();
		return 0;
	}
	else
	{
		m_ADOConn.ExitConnect();
		return 1;
	}
}

void CAddUserDlg::InsertAccountNum()
{
	_bstr_t accountSQL,depositorSQL;
	CString str;
	str.Format("%f",m_Amount);
	accountSQL="insert into account(account_number,account_pswd,branch_name,balance)\
		values('"+m_AccountNum+"','"+m_Password+"','"+m_BranchName+"','"+str+"')";
	depositorSQL="insert into depositor(account_number,customer_id,account_type,access_date,\
		amount) values('"+m_AccountNum+"','"+m_CustomerID+"','save',convert(varchar,getdate()\
		,120),'"+str+"')";
	CADOConn m_ADOConn;

	m_ADOConn.OnInitADOConn();
	m_ADOConn.ExecuteSQL(accountSQL);	//���˺���Ϣ����account����
	m_ADOConn.ExecuteSQL(depositorSQL);//����¼����depositor����
	m_ADOConn.ExitConnect();
}

int CAddUserDlg::CheckBranch()
{
	_bstr_t vSQL;
	vSQL="select * from branch where branch_name='"+m_BranchName+"'";
	CADOConn m_ADOConn;
	_RecordsetPtr m_Recordset;
	m_ADOConn.OnInitADOConn();
	m_Recordset=m_ADOConn.GetRecordset(vSQL);
	if(m_Recordset->adoEOF)//��¼����
	{
		m_ADOConn.ExitConnect();
		return 0;
	}
	else
	{
		m_ADOConn.ExitConnect();
		return 1;
	}
}
