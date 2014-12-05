// BranchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Bank.h"
#include "BranchDlg.h"
#include "ADOConn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBranchDlg dialog


CBranchDlg::CBranchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBranchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBranchDlg)
	m_Assets = _T("");
	m_BranchCity = _T("");
	m_BranchName = _T("");
	m_FindBranch = _T("");
	//}}AFX_DATA_INIT
}


void CBranchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBranchDlg)
	DDX_Control(pDX, IDC_LIST1, m_ListBranch);
	DDX_Text(pDX, IDC_ASSETS, m_Assets);
	DDX_Text(pDX, IDC_BRANCHCITY, m_BranchCity);
	DDX_Text(pDX, IDC_BRANCHNAME, m_BranchName);
	DDX_Text(pDX, IDC_FINDBRANCHNAME, m_FindBranch);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBranchDlg, CDialog)
	//{{AFX_MSG_MAP(CBranchDlg)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnClickList1)
	ON_BN_CLICKED(IDC_FINDALL, OnFindall)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBranchDlg message handlers

BOOL CBranchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	/*********************�����б��ķ��***********************************************/
	DWORD dwExStyle=LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|\
		LVS_EX_ONECLICKACTIVATE;
	m_ListBranch.ModifyStyle(0,LVS_REPORT|LVS_SINGLESEL|LVS_SHOWSELALWAYS);
	m_ListBranch.SetExtendedStyle(dwExStyle);
	m_ListBranch.InsertColumn(0,"��������",LVCFMT_CENTER,150,0);
	m_ListBranch.InsertColumn(1,"�������ڳ���",	LVCFMT_CENTER,150,0);
	m_ListBranch.InsertColumn(2,"�����ʲ�",LVCFMT_CENTER,150,0);

//	ListAll();
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBranchDlg::ListAll()
{
	CADOConn m_ADOConn;
	_bstr_t vSQL="select * from branch";
	_RecordsetPtr m_pRecordset;
	m_ADOConn.OnInitADOConn();
	m_pRecordset=m_ADOConn.GetRecordset(vSQL);
	if(m_pRecordset->adoEOF) //��ɼ�¼���ǿյ�
		return;
	m_ListBranch.DeleteAllItems();//����б�
	m_pRecordset->MoveFirst();//�ƶ�����һ����¼
	CString branch,city,assets;
	int index=0;
	/**********************��ȡ����***************/
	while(!m_pRecordset->adoEOF)
	{
		branch=(LPCTSTR)(_bstr_t)m_pRecordset->GetCollect("branch_name");
		city=(LPCTSTR)(_bstr_t)m_pRecordset->GetCollect("branch_city");
		assets=(LPCTSTR)(_bstr_t)m_pRecordset->GetCollect("assets");

		m_ListBranch.InsertItem(index,branch);//���б�������
		m_ListBranch.SetItemText(index,1,city);
		m_ListBranch.SetItemText(index,2,assets);
		//���Ƽ�¼��������1
		m_pRecordset->MoveNext();
		index++;
	}
	m_ADOConn.ExitConnect();
	return;
}

void CBranchDlg::OnClickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nItem=m_ListBranch.GetNextItem(-1,LVNI_SELECTED);//��ȡ��ǰѡ���е�����ֵ
	if(nItem!=-1)
	{
		m_BranchName=m_ListBranch.GetItemText(nItem,0);
		m_BranchCity=m_ListBranch.GetItemText(nItem,1);
		m_Assets=m_ListBranch.GetItemText(nItem,2);
	}
	UpdateData(false);
	*pResult = 0;
}

void CBranchDlg::RefreshData()
{
	m_BranchName="";
	m_BranchCity="";
	m_Assets="";
	m_FindBranch="";
	UpdateData(false);
}

void CBranchDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	_bstr_t vSQL;
	vSQL="select * from branch where branch_name='"+m_FindBranch+"'";
	CADOConn m_ADOConn;
	_RecordsetPtr m_Recordset;
	m_ADOConn.OnInitADOConn();
	m_Recordset=m_ADOConn.GetRecordset(vSQL);
	if(m_Recordset->adoEOF)
	{
		MessageBox("�޴˼�¼");
		m_ADOConn.ExitConnect();
		return;
	}
	else
	{
		m_Recordset->MoveFirst();
		m_ListBranch.DeleteAllItems();
		CString branch,city,assets;
		int index=0;
		/**********************��ȡ����***************/
		while(!m_Recordset->adoEOF)
		{
			branch=(LPCTSTR)(_bstr_t)m_Recordset->GetCollect("branch_name");
			city=(LPCTSTR)(_bstr_t)m_Recordset->GetCollect("branch_city");
			assets=(LPCTSTR)(_bstr_t)m_Recordset->GetCollect("assets");
			
			m_ListBranch.InsertItem(index,branch);//���б�������
			m_ListBranch.SetItemText(index,1,city);
			m_ListBranch.SetItemText(index,2,assets);
			//���Ƽ�¼��������1
			m_Recordset->MoveNext();
			index++;
		}
		m_ADOConn.ExitConnect();
	}
	//	CDialog::OnOK();
}

void CBranchDlg::OnFindall() 
{
	// TODO: Add your control notification handler code here
	ListAll();	
}

void CBranchDlg::OnAdd() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_BranchName==""||m_BranchCity==""||m_Assets=="")
	{
		MessageBox("������Ϣ���������޷����");
		return;
	}

	_bstr_t vSQL,fSQL;
	fSQL="select * from branch where branch_name='"+m_BranchName+"'";
	vSQL="insert into branch(branch_name,branch_city,assets)\
		values('"+m_BranchName+"','"+m_BranchCity+"','"+m_Assets+"')";
	CADOConn m_ADOConn;
	_RecordsetPtr m_Recordset;
	m_ADOConn.OnInitADOConn();	
	m_Recordset=m_ADOConn.GetRecordset(fSQL);
	if(!m_Recordset->adoEOF)
	{
		MessageBox("��������Ϣ�Ѿ����ڣ��޷����");
		return;
	}
	m_ADOConn.ExecuteSQL(vSQL);
	MessageBox("��ӳɹ�");
	m_ADOConn.ExitConnect();
}
/*
void CBranchDlg::OnDelete() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_BranchName==""||m_BranchCity==""||m_Assets=="")
	{
		MessageBox("��ѡ��Ҫɾ��������");
		return;
	}
	CString str;
	str.Format("ȷ��Ҫɾ��%s������Ϣ��",m_BranchName);
	if(MessageBox(str,"����",MB_YESNO)==IDYES)
	{
		_bstr_t vSQL;
		vSQL="delete from branch where branch_name='"+m_BranchName+"'";
		CADOConn m_ADOConn;
		m_ADOConn.OnInitADOConn();	
		m_ADOConn.ExecuteSQL(vSQL);
		MessageBox("ɾ���ɹ�");
		m_ADOConn.ExitConnect();
	}
}*/

