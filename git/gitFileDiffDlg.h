//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2011 by René Kraus (aka upCASE)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __GitFileDiffDlg__
#define __GitFileDiffDlg__

#include <wx/wx.h>

class GitCommitEditor;

class GitFileDiffDlg : public wxDialog
{
  GitCommitEditor* m_editor;
  public:
    GitFileDiffDlg(wxWindow* parent);
	~GitFileDiffDlg();
    void SetDiff(const wxString& diff);
};

#endif //__GitFileDiffDlg__

