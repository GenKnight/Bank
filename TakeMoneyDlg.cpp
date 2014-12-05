// TakeMoneyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Bank.h"
#include "TakeMoneyDlg.h"
#include "ADOConn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTakeMoneyDlg dialog

extern CString g_CustomerID;

CTakeMoneyDlg::CTakeMoneyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTakeMoneyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTakeMoneyDlg)
	m_Amount = 0.0;
	//}}AFX_DATA_INIT
}


void CTakeMoneyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTakeMoneyDlg)
	DDX_Text(pDX, IDC_TAKEMONEY, m_Amount);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTakeMoneyDlg, CDialog)
	//{{AFX_MSG_MAP(CTakeMoneyDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTakeMoneyDlg message handlers

void CTakeMoneyDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	if(m_Amount<=0)
	{
		AfxMessageBox("ȡ���С�ڵ���0���޷�ȡ��");
		return;
	}
	//��ȡϵͳʱ��
	//�޸�account���е��������depositor���еĴ���¼
	CADOConn m_ADOConn;
	_RecordsetPtr m_pRecordset;
	m_ADOConn.OnInitADOConn();

	_bstr_t balanceSQL,depSQL,preBalance,custID;
	//ȡ����ǰ�����
	_variant_t theValue;
	double balance;
	CString strBalance;
	preBalance="select * from account where account_number='"+g_CustomerID+"'";
	m_pRecordset=m_ADOConn.GetRecordset(preBalance);	
	theValue=m_pRecordset->GetCollect("balance");
	if(theValue.vt!=VT_NULL)
		strBalance=(char *)_bstr_t(theValue);
	balance=atof(strBalance);
	balance-=m_Amount;//����ȥȡ���Ϊ�µ����
	if(balance<0)
		AfxMessageBox("���㣬�޷�ȡ�");
	else
	{
		CString strA;
		strA.Format("%f",balance);
		balanceSQL="update account set balance='"+strA+"' where\
			account_number='"+g_CustomerID+"'";
		m_ADOConn.ExecuteSQL(balanceSQL);
		//��ѯ�ͻ�id
		custID="select distinct customer_id from account,depositor where account.account_number=\
			depositor.account_number";
		m_pRecordset=m_ADOConn.GetRecordset(custID);	
		theValue=m_pRecordset->GetCollect("customer_id");
		CString m_CustomerId;
		if(theValue.vt!=VT_NULL)
			m_CustomerId=(char *)_bstr_t(theValue);
		//��Ӵ���¼
		strA.Format("%f",m_Amount);
		depSQL="insert into depositor(account_number,customer_id,account_type,access_date,amount)\
			values('"+g_CustomerID+"','"+m_CustomerId+"','take',\
			convert(varchar,getdate(),120),'"+strA+"')";//��ȡϵͳʱ��
		m_ADOConn.ExecuteSQL(depSQL);
		AfxMessageBox("ȡ��ɹ�!");
	}
	m_ADOConn.ExitConnect();
	CDialog::OnOK();
}
