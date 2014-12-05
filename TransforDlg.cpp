// TransforDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Bank.h"
#include "TransforDlg.h"
#include "RegisteDlg.h"
#include "ADOConn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTransforDlg dialog

extern CString g_CustomerID;

CTransforDlg::CTransforDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTransforDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTransforDlg)
	m_DesAccountNum = _T("");
	m_DesAmount = 0.0;
	m_Password = _T("");
	//}}AFX_DATA_INIT
}


void CTransforDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTransforDlg)
	DDX_Text(pDX, IDC_DESACCOUNTNUM, m_DesAccountNum);
	DDX_Text(pDX, IDC_DESAMOUNT, m_DesAmount);
	DDX_Text(pDX, IDC_PASSWORD, m_Password);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTransforDlg, CDialog)
	//{{AFX_MSG_MAP(CTransforDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTransforDlg message handlers

void CTransforDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	if(m_DesAmount<=0)
	{
		AfxMessageBox("ת�˶�С�ڵ���0���޷�ת��");
		return;
	}
	CRegisteDlg dlg;
	if(m_DesAccountNum==g_CustomerID)
	{
		AfxMessageBox("Ҫת����˺�Ϊ���ڵ�¼���˺ţ��޷�ת��");
		GetDlgItem(IDC_PASSWORD)->SetWindowText("");
		return;
	}
	if(dlg.CheckUserID(m_DesAccountNum)==1)//���Ҫת�����˺��Ƿ���ڣ��ǿշ���0���շ���1
	{
		AfxMessageBox("Ҫת����˺Ų����ڣ���ȷ��");
		GetDlgItem(IDC_PASSWORD)->SetWindowText("");
		return;
	}
	if(dlg.QueryBalance()<m_DesAmount)
	{
		AfxMessageBox("���㣬�޷�ת��");
		GetDlgItem(IDC_PASSWORD)->SetWindowText("");
		return;
	}
	//ȷ������
	_bstr_t vSQL="Select * from account where account_number='"+g_CustomerID+"'";
	CADOConn m_ADOConn;
	m_ADOConn.OnInitADOConn(); //�����ݿ�����
	_RecordsetPtr m_pRecordset;//�����¼��ָ��
	m_pRecordset=m_ADOConn.GetRecordset(vSQL);
	CString strPswd;
	_variant_t theValue;
	theValue=m_pRecordset->GetCollect("account_pswd");
	if(theValue.vt!=VT_NULL)
		strPswd=(char *)_bstr_t(theValue);
	if(strPswd!=m_Password)
	{
		AfxMessageBox("��������벻��ȷ���޷�ת��");
		GetDlgItem(IDC_PASSWORD)->SetWindowText("");
		return;
	}
	/***********************************�޸�ת���˺ŵ�������ת������Ϣ****************/
	_bstr_t balanceSQL,depSQL,preBalance,custID;
	//ȡ����ǰ�����
	double balance;
	CString strBalance;
	preBalance="select * from account where account_number='"+g_CustomerID+"'";
	m_pRecordset=m_ADOConn.GetRecordset(preBalance);	
	theValue=m_pRecordset->GetCollect("balance");
	if(theValue.vt!=VT_NULL)
		strBalance=(char *)_bstr_t(theValue);
	balance=atof(strBalance);
	balance-=m_DesAmount;//����ȥȡ���Ϊ�µ����
	CString strA;
	strA.Format("%f",balance);
	balanceSQL="update account set balance='"+strA+"' where\
		account_number='"+g_CustomerID+"'";
	m_ADOConn.ExecuteSQL(balanceSQL);
	////////////////////////////////////////////��ѯ�ͻ�id
	custID="select distinct customer_id from account,depositor where account.account_number=\
		depositor.account_number and depositor.account_number='"+g_CustomerID+"'";

	m_pRecordset=m_ADOConn.GetRecordset(custID);	
	theValue=m_pRecordset->GetCollect("customer_id");
	CString m_CustomerId;
	if(theValue.vt!=VT_NULL)
		m_CustomerId=(char *)_bstr_t(theValue);
	//��Ӵ���¼
	CString strAmount;
	strAmount.Format("%f",m_DesAmount);
	depSQL="insert into depositor(account_number,customer_id,account_type,access_date,amount)\
		values('"+g_CustomerID+"','"+m_CustomerId+"','out',\
		convert(varchar,getdate(),120),'"+strAmount+"')";//��ȡϵͳʱ��
	m_ADOConn.ExecuteSQL(depSQL);

	/********************************�޸�ת���˺ŵ�������ת�����Ϣ**************/
	preBalance="select * from account where account_number='"+m_DesAccountNum+"'";
	m_pRecordset=m_ADOConn.GetRecordset(preBalance);	
	theValue=m_pRecordset->GetCollect("balance");
	if(theValue.vt!=VT_NULL)
		strBalance=(char *)_bstr_t(theValue);
	balance=atof(strBalance);
	balance+=m_DesAmount;//������ת���Ϊ�µ����
	strA.Format("%f",balance);
	balanceSQL="update account set balance='"+strA+"' where\
		account_number='"+m_DesAccountNum+"'";
	m_ADOConn.ExecuteSQL(balanceSQL);
	//////////////////////////////////////��ѯ�ͻ�id
	custID="select distinct customer_id from account,depositor where account.account_number=\
		depositor.account_number and depositor.account_number='"+m_DesAccountNum+"'";
	m_pRecordset=m_ADOConn.GetRecordset(custID);	
	theValue=m_pRecordset->GetCollect("customer_id");
	if(theValue.vt!=VT_NULL)
		m_CustomerId=(char *)_bstr_t(theValue);
	//��Ӵ���¼
	depSQL="insert into depositor(account_number,customer_id,account_type,access_date,amount)\
		values('"+m_DesAccountNum+"','"+m_CustomerId+"','in',\
		convert(varchar,getdate(),120),'"+strAmount+"')";//��ȡϵͳʱ��
	m_ADOConn.ExecuteSQL(depSQL);
	///////////////////////////////////////////////////////////////////
	AfxMessageBox("ת�˳ɹ�!");
	
	CDialog::OnOK();
}
