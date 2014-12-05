// AddEmpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Bank.h"
#include "AddEmpDlg.h"
#include "ADOConn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddEmpDlg dialog

extern CString g_CustomerID;

CAddEmpDlg::CAddEmpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddEmpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddEmpDlg)
	m_EmployeeID = _T("");
	m_EmpPassword = _T("");
	m_EmpPassword2 = _T("");
	m_EmpTelephone = _T("");
	m_StartDate = _T("");
	m_EmpName = _T("");
	//}}AFX_DATA_INIT
}


void CAddEmpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddEmpDlg)
	DDX_Text(pDX, IDC_EMPLOYEEID, m_EmployeeID);
	DDX_Text(pDX, IDC_EMPPASSWORD, m_EmpPassword);
	DDX_Text(pDX, IDC_EMPPASSWORD2, m_EmpPassword2);
	DDX_Text(pDX, IDC_EMPTELEPHONE, m_EmpTelephone);
	DDX_Text(pDX, IDC_STARTDATE, m_StartDate);
	DDX_Text(pDX, IDC_EMPNAME, m_EmpName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddEmpDlg, CDialog)
	//{{AFX_MSG_MAP(CAddEmpDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddEmpDlg message handlers

void CAddEmpDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	if(m_EmployeeID==""||m_EmpPassword==""||m_EmpPassword2==""||m_EmpName==""||\
		m_StartDate==""||m_EmpTelephone=="")
	{
		AfxMessageBox("������Ϣ������������������");
		return;
	}
	if(!CheckEmployee())//��¼����
	{
		AfxMessageBox("�ù�Աid�Ѿ����ڣ�����������");
		GetDlgItem(IDC_EMPLOYEEID)->SetWindowText("");
		GetDlgItem(IDC_EMPPASSWORD)->SetWindowText("");
		GetDlgItem(IDC_EMPPASSWORD2)->SetWindowText("");
		GetDlgItem(IDC_EMPNAME)->SetWindowText("");
		GetDlgItem(IDC_EMPTELEPHONE)->SetWindowText("");
		GetDlgItem(IDC_STARTDATE)->SetWindowText("");
		return;
	}
	if(m_EmpPassword!=m_EmpPassword2)
	{
		AfxMessageBox("������������벻һ�£�����������");
		GetDlgItem(IDC_EMPPASSWORD)->SetWindowText("");
		GetDlgItem(IDC_EMPPASSWORD2)->SetWindowText("");
		return;
	}
	InsertEmployee();
	AfxMessageBox("�ɹ���ӹ�Ա��Ϣ");
	CDialog::OnOK();
}

int CAddEmpDlg::CheckEmployee()
{
	//�ȼ���û�ID�Ƿ���ڣ��շ���1
	_bstr_t vSQL="Select * from employee where employee_id='"+m_EmployeeID+"'";
	CADOConn m_ADOConn;
	m_ADOConn.OnInitADOConn(); //�����ݿ�����
	_RecordsetPtr m_pRecordset;//�����¼��ָ��
	m_pRecordset=m_ADOConn.GetRecordset(vSQL);
	if(m_pRecordset->adoEOF) //��ɼ�¼���ǿյ�,��ʾû�б�ע�ᣬ�շ���1
		return 1;
	else
		return 0;//��ע�ᣬ�ǿշ���0
}

void CAddEmpDlg::InsertEmployee()
{
	_bstr_t empSQL,worksforSQL;
	empSQL="insert into employee(employee_id,employee_pswd,employee_name,start_date,\
		employee_tel) values('"+m_EmployeeID+"','"+m_EmpPassword+"','"+m_EmpName+"',\
		convert(varchar,getdate(),120),'"+m_EmpTelephone+"')";
	worksforSQL="insert into works_for(worker_employee_id,manager_employee_id)\
		values('"+m_EmployeeID+"','"+g_CustomerID+"')";
	CADOConn m_ADOConn;
	m_ADOConn.OnInitADOConn();
	m_ADOConn.ExecuteSQL(empSQL);	//���˺���Ϣ����employee����
	m_ADOConn.ExecuteSQL(worksforSQL);//����¼����works_for����
	m_ADOConn.ExitConnect();
}
