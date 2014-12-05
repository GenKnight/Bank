// AllLoanDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Bank.h"
#include "AllLoanDlg.h"
#include "ADOConn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAllLoanDlg dialog


CAllLoanDlg::CAllLoanDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAllLoanDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAllLoanDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAllLoanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAllLoanDlg)
	DDX_Control(pDX, IDC_LIST1, m_ListLoan);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAllLoanDlg, CDialog)
	//{{AFX_MSG_MAP(CAllLoanDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAllLoanDlg message handlers

BOOL CAllLoanDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	DWORD dwExStyle=LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|\
		LVS_EX_ONECLICKACTIVATE;
	m_ListLoan.ModifyStyle(0,LVS_REPORT|LVS_SINGLESEL|LVS_SHOWSELALWAYS);
	m_ListLoan.SetExtendedStyle(dwExStyle);
	m_ListLoan.InsertColumn(0,"�����",LVCFMT_CENTER,100,0);
	m_ListLoan.InsertColumn(1,"��������",	LVCFMT_CENTER,100,0);
	m_ListLoan.InsertColumn(2,"�ͻ�ID",LVCFMT_CENTER,100,0);
	m_ListLoan.InsertColumn(3,"�ͻ�����",LVCFMT_CENTER,100,0);
	m_ListLoan.InsertColumn(4,"�����",	LVCFMT_CENTER,100,0);
	m_ListLoan.InsertColumn(5,"��������",	LVCFMT_CENTER,100,0);
	ListAll();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAllLoanDlg::ListAll()
{
	CADOConn m_ADOConn;
	_bstr_t vSQL="select distinct L.loan_number,L.customer_id,C.customer_name,\
		L.branch_name,L.amount,L.loan_date from loan as L,customer as C where L.customer_id=C.customer_id";
	_RecordsetPtr m_pRecordset;
	m_ADOConn.OnInitADOConn();
	m_pRecordset=m_ADOConn.GetRecordset(vSQL);
	if(m_pRecordset->adoEOF) //��ɼ�¼���ǿյ�
		return;
	m_ListLoan.DeleteAllItems();//����б�
	m_pRecordset->MoveFirst();//�ƶ�����һ����¼
	CString num,branch,id,name,amount,date;
	int index=0;
	/**********************��ȡ����***************/
	while(!m_pRecordset->adoEOF)
	{
		num=(LPCTSTR)(_bstr_t)m_pRecordset->GetCollect("loan_number");
		branch=(LPCTSTR)(_bstr_t)m_pRecordset->GetCollect("branch_name");
		name=(LPCTSTR)(_bstr_t)m_pRecordset->GetCollect("customer_name");
		id=(LPCTSTR)(_bstr_t)m_pRecordset->GetCollect("customer_id");
		amount=(LPCTSTR)(_bstr_t)m_pRecordset->GetCollect("amount");
		date=(LPCTSTR)(_bstr_t)m_pRecordset->GetCollect("loan_date");

		m_ListLoan.InsertItem(index,num);//���б�������
		m_ListLoan.SetItemText(index,1,branch);
		m_ListLoan.SetItemText(index,2,id);
		m_ListLoan.SetItemText(index,3,name);
		m_ListLoan.SetItemText(index,4,amount);
		m_ListLoan.SetItemText(index,5,date);
		//���Ƽ�¼��������1
		m_pRecordset->MoveNext();
		index++;
	}
	m_ADOConn.ExitConnect();
	return;
}
