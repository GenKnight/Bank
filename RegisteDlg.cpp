// RegisteDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Bank.h"
#include "RegisteDlg.h"
#include "ADOConn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegisteDlg dialog

extern CString g_CustomerID;

CRegisteDlg::CRegisteDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRegisteDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRegisteDlg)
	m_UserID = _T("");
	m_City = _T("");
	m_Street = _T("");
	m_UserName = _T("");
	m_Telephone = _T("");
	//}}AFX_DATA_INIT
}


void CRegisteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRegisteDlg)
	DDX_Text(pDX, IDC_USERID, m_UserID);
	DDX_Text(pDX, IDC_CITY, m_City);
	DDX_Text(pDX, IDC_STREET, m_Street);
	DDX_Text(pDX, IDC_USERNAME, m_UserName);
	DDX_Text(pDX, IDC_TELEPHONE, m_Telephone);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRegisteDlg, CDialog)
	//{{AFX_MSG_MAP(CRegisteDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegisteDlg message handlers

void CRegisteDlg::OnOK() 
{
	// TODO: Add extra validation here
	//ע���û����ȼ���û�ID�Ƿ����
	//�ж��������������Ƿ�һ�£������벻��Ϊ��
	//����������ݲ������ݿ�Customer����
	BOOL bLogOn=FALSE;
	UpdateData(true);
	if(m_UserID==""||m_Telephone==""||m_UserName==""||m_City==""||m_Street=="")
	{
		AfxMessageBox("�������Ϣ�����������������");
		return;
	}
	if(CheckUserID(m_UserID)==0)
	{
		AfxMessageBox("�ÿͻ�ID�Ѿ���ע�ᣬ����������");
		EmptyEdit();
		return;
	}
	InsertUserInfo();
	AfxMessageBox("�û�ע��ɹ�");
	CDialog::OnOK();
}

int CRegisteDlg::CheckUserID(CString m_UserID)
{
	//�ȼ���˻��Ƿ���ڣ��շ���1
	_bstr_t vSQL="Select * from account where account_number='"+m_UserID+"'";
	CADOConn m_ADOConn;
	m_ADOConn.OnInitADOConn(); //�����ݿ�����
	_RecordsetPtr m_pRecordset;//�����¼��ָ��
	m_pRecordset=m_ADOConn.GetRecordset(vSQL);
	if(m_pRecordset->adoEOF) //��ɼ�¼���ǿյ�,��ʾû�б�ע�ᣬ�շ���1
		return 1;
	else
		return 0;//��ע�ᣬ�ǿշ���0
}

void CRegisteDlg::InsertUserInfo()
{//����û���Ϣ
	_bstr_t vSQL="Insert into customer(customer_id,customer_name,customer_street,customer_city,\
		customer_tel) values('"+m_UserID+"','"+m_UserName+"','"+m_Street+"','"+m_City+"',\
		'"+m_Telephone+"')";
	CADOConn m_ADOConn;
	m_ADOConn.OnInitADOConn(); //�����ݿ�����
	m_ADOConn.ExecuteSQL(vSQL);//ִ�в������
	m_ADOConn.ExitConnect();//�Ͽ�����Դ����
}

void CRegisteDlg::EmptyEdit()
{
	GetDlgItem(IDC_USERID)->SetWindowText("");
	GetDlgItem(IDC_TELEPHONE)->SetWindowText("");
	GetDlgItem(IDC_USERNAME)->SetWindowText("");
	GetDlgItem(IDC_STREET)->SetWindowText("");
	GetDlgItem(IDC_CITY)->SetWindowText("");
}

double CRegisteDlg::QueryBalance()
{
	_bstr_t vSQL="select balance from account where account_number='"+g_CustomerID+"'";
	CADOConn m_ADOConn;
	m_ADOConn.OnInitADOConn();
	_RecordsetPtr m_pRecordset;
	m_pRecordset=m_ADOConn.GetRecordset(vSQL);
	double balance;
	CString strBalance;
	_variant_t theValue;
	theValue=m_pRecordset->GetCollect("balance");
	if(theValue.vt!=VT_NULL)
		strBalance=(char *)_bstr_t(theValue);
	balance=atof(strBalance);
	return balance;
}

int CRegisteDlg::CheckCustomerID(CString m_UserID)
{
	//�ȼ��ͻ��Ƿ���ڣ��շ���1
	_bstr_t vSQL="Select * from customer where customer_id='"+m_UserID+"'";
	CADOConn m_ADOConn;
	m_ADOConn.OnInitADOConn(); //�����ݿ�����
	_RecordsetPtr m_pRecordset;//�����¼��ָ��
	m_pRecordset=m_ADOConn.GetRecordset(vSQL);
	if(m_pRecordset->adoEOF) //��ɼ�¼���ǿյ�,��ʾû�б�ע�ᣬ�շ���1
		return 1;
	else
		return 0;//��ע�ᣬ�ǿշ���0
}
