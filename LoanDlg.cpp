// LoanDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Bank.h"
#include "LoanDlg.h"
#include "ADOConn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoanDlg dialog


CLoanDlg::CLoanDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoanDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoanDlg)
	m_BranchName = _T("");
	m_CustomerID = _T("");
	m_Amount = 0.0;
	m_LoanNumber = _T("");
	//}}AFX_DATA_INIT
}


void CLoanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoanDlg)
	DDX_Text(pDX, IDC_BRANCHNAME, m_BranchName);
	DDX_Text(pDX, IDC_CUSTOMERID, m_CustomerID);
	DDX_Text(pDX, IDC_AMOUNT, m_Amount);
	DDX_Text(pDX, IDC_LOANNUMBER, m_LoanNumber);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoanDlg, CDialog)
	//{{AFX_MSG_MAP(CLoanDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoanDlg message handlers

void CLoanDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	if(m_CustomerID==""||m_BranchName==""||m_LoanNumber=="")
	{
		AfxMessageBox("������Ϣ������������������");
		return;
	}
	if(!CheckCustomerID())
	{
		AfxMessageBox("�ÿͻ������ڣ�����ע��");
		return;
	}
	if(!CheckBranch())
	{
		AfxMessageBox("�����в����ڣ�����������");
		return;
	}
	if(CheckLoanNumber())
	{
		AfxMessageBox("�ô�����Ѿ����ڣ�����������");
		return;
	}
	if(m_Amount<=0)
	{
		AfxMessageBox("�������ȷ������������");
		return;
	}
	InsertLoan();
	AfxMessageBox("�ɹ�Ϊ�ÿͻ��������!");
	CDialog::OnOK();
}

int CLoanDlg::CheckBranch()
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

int CLoanDlg::CheckCustomerID()
{
	_bstr_t vSQL;
	vSQL="select * from customer where customer_id='"+m_CustomerID+"'";
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

int CLoanDlg::CheckLoanNumber()
{
	_bstr_t vSQL;
	vSQL="select * from loan where loan_number='"+m_LoanNumber+"'";
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

void CLoanDlg::InsertLoan()
{
	_bstr_t loanSQL;
	CString str;
	str.Format("%f",m_Amount);
	loanSQL="insert into loan(loan_number,customer_id,branch_name,loan_date,\
		amount) values('"+m_LoanNumber+"','"+m_CustomerID+"','"+m_BranchName+"',\
		convert(varchar,getdate(),120),'"+str+"')";
	CADOConn m_ADOConn;

	m_ADOConn.OnInitADOConn();
	m_ADOConn.ExecuteSQL(loanSQL);	//���˺���Ϣ����loan����
	m_ADOConn.ExitConnect();
}
