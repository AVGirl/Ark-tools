// KeyDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RegisterManager.h"
#include "KeyDlg.h"
#include "afxdialogex.h"


// CKeyDlg �Ի���

IMPLEMENT_DYNAMIC(CKeyDlg, CDialog)

CKeyDlg::CKeyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKeyDlg::IDD, pParent)
	, m_strKeyNameStatic(_T(""))
	, m_strKeyNameEdit(_T(""))
{

}

CKeyDlg::~CKeyDlg()
{
}

void CKeyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_KEY_NAME, m_strKeyNameStatic);
	DDX_Text(pDX, IDC_EDIT, m_strKeyNameEdit);
}


BEGIN_MESSAGE_MAP(CKeyDlg, CDialog)
END_MESSAGE_MAP()


// CKeyDlg ��Ϣ�������


BOOL CKeyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	switch (m_nDlgType)
	{
	case enumRenameKey:
		SetWindowText(L"��������");
		m_strKeyNameStatic = L"�¼���";
		break;

	case enumRenameValue:
		SetWindowText(L"��������ֵ");
		m_strKeyNameStatic = L"�¼�ֵ";
		break;

	case enumCreateKey:
		SetWindowText(L"�����¼�");
		m_strKeyNameStatic = L"�¼���";
		break;

	case enumSetValueKey:
		SetWindowText(L"������ֵ");
		m_strKeyNameStatic = L"�¼�ֵ";
		break;
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
