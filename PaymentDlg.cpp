// PaymentDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Bank.h"
#include "PaymentDlg.h"
#include "ADOConn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPaymentDlg dialog


CPaymentDlg::CPaymentDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPaymentDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPaymentDlg)
	m_LoanNumber = _T("");
	m_Amount = 0.0;
	//}}AFX_DATA_INIT
}


void CPaymentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPaymentDlg)
	DDX_Text(pDX, IDC_PAYMENTNUMBER, m_LoanNumber);
	DDX_Text(pDX, IDC_PAYMENTAMOUNT, m_Amount);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPaymentDlg, CDialog)
	//{{AFX_MSG_MAP(CPaymentDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPaymentDlg message handlers

void CPaymentDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	if(!CheckLoan())
	{
		AfxMessageBox("�ô���Ų�����");
		return;
	}
	if(m_Amount<=0)
	{
		AfxMessageBox("������ȷ");
		return;
	}
	if(!InsertLoan())
	{
		CString str;
		str.Format("��������࣡��");
		AfxMessageBox(str);
	}
	else
	{
		CString str;
		str.Format("�ɹ�Ϊ�ÿͻ�����%f",m_Amount);
		AfxMessageBox(str);
		CDialog::OnOK();
	}
}

int CPaymentDlg::CheckLoan()
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

int CPaymentDlg::InsertLoan()
{
	_bstr_t paySQL,amountSQL,loanSQL;	
	CADOConn m_ADOConn;
	_RecordsetPtr m_pRecordset;
	m_ADOConn.OnInitADOConn();
	
	amountSQL="select amount from loan where loan_number='"+m_LoanNumber+"'";
	m_pRecordset=m_ADOConn.GetRecordset(amountSQL);
	double amount;
	CString astr;
	_variant_t theValue;
	theValue=m_pRecordset->GetCollect("amount");
	if(theValue.vt!=VT_NULL)
		astr=(char *)_bstr_t(theValue);
	amount=atof(astr);
	amount-=m_Amount;
	if(amount<0)
		return 0;
	CString str;
	str.Format("%f",m_Amount);
	paySQL="insert into payment(payment_number,payment_date,payment_amount)\
		values('"+m_LoanNumber+"',convert(varchar,getdate(),120),'"+str+"')";
	m_ADOConn.ExecuteSQL(paySQL);	//���˺���Ϣ����payment����
	CString fstr;
	fstr.Format("%f",amount);
	loanSQL="Update loan set amount='"+fstr+"' where loan_number='"+m_LoanNumber+"'";	
	m_ADOConn.ExecuteSQL(loanSQL);//�޸�loan���е�amount
	m_ADOConn.ExitConnect();
	return 1;
}
