#include "ClusterConfig.h"
#include "ClusterPage.h"
#include "MainFrame.h"
#include "SettingsDlg.h"
#include <wx/aboutdlg.h>
#include <wx/ffile.h>
#include <wx/filename.h>
#include <wx/listctrl.h>
#include <wx/msgdlg.h>
#include <wx/textdlg.h>

MainFrame* MainFrame::m_mainFrame = nullptr;

MainFrame::MainFrame(wxWindow* parent)
    : MainFrameBaseClass(parent)
{
    wxArrayString clusters = ClusterConfig::GetAllClusters();
    for(const wxString& cluster : clusters) {
        ClusterConfig conf(cluster);
        conf.Load();
        ClusterPage* p = new ClusterPage(m_notebook, &conf);
        m_notebook->AddPage(p, p->GetClusterName(), false);
    }
    //    m_listbook85->GetListView()->SetColumnWidth(0, 300);
    Layout();
    SetLabel("cluster-tool");
}

MainFrame::~MainFrame() {}

void MainFrame::OnExit(wxCommandEvent& event)
{
    wxUnusedVar(event);
    Close();
}

void MainFrame::OnAbout(wxCommandEvent& event)
{
    wxUnusedVar(event);
    wxAboutDialogInfo info;
    info.SetCopyright(_("Public Domain"));
    info.SetLicence(_("Public Domain"));
    info.SetDescription(_("Create Redis Cluster Tool"));
    ::wxAboutBox(info);
}
void MainFrame::OnNewCluster(wxCommandEvent& event)
{
    wxUnusedVar(event);
    wxString name = ::wxGetTextFromUser("Cluster Name", "New Cluster", "New Cluster", this);
    if(name.IsEmpty()) { return; }

    wxArrayString clusters = ClusterConfig::GetAllClusters();
    if(clusters.Index(name) != wxNOT_FOUND) {
        ::wxMessageBox("A cluster with this name already exists", "Cluster Tool", wxICON_WARNING | wxCENTRE);
        return;
    }
    clusters.Add(name);
    ClusterConfig::SetClusters(clusters);
    ClusterPage* page = new ClusterPage(m_notebook);
    page->GetTextCtrlName()->SetValue(name);
    page->GetTextCtrlName()->Enable(false);
    m_notebook->AddPage(page, name, true, wxNOT_FOUND);
}

void MainFrame::OnSave(wxCommandEvent& event)
{
    wxUnusedVar(event);
    Save(GetActivePage());
}

void MainFrame::OnSaveUI(wxUpdateUIEvent& event)
{
    event.Enable(GetActivePage() && !GetActivePage()->GetClusterPath().IsEmpty());
}

void MainFrame::Save(ClusterPage* page)
{
    if(!page) { return; }
    ClusterConfig conf(page);
    conf.SetFirstPort(page->GetFirstPort());
    conf.SetPath(page->GetClusterPath());
    conf.SetSize(page->GetClusterSize());
    conf.SetHasReplica(page->HasReplica());
    conf.Save();
}

void MainFrame::OnDeploy(wxCommandEvent& event)
{
    ClusterPage* page = GetActivePage();
    if(!page) { return; }
    if(page->GetClusterPath().IsEmpty() || page->GetClusterPath() == "/" ||
       !wxFileName::DirExists(page->GetClusterPath())) {
        wxMessageBox("Invalid or empty path", "Error", wxICON_ERROR | wxCENTER);
        return;
    }

    ClusterConfig conf(page);

    conf.Deploy();
    ::wxMessageBox(wxString() << "'" << conf.GetName() << "' configuration files were created at "
                              << conf.GetDeploymentPath());
}

ClusterPage* MainFrame::GetActivePage()
{
    if(m_notebook->GetSelection() == wxNOT_FOUND) { return NULL; }
    ClusterPage* page = dynamic_cast<ClusterPage*>(m_notebook->GetPage(m_notebook->GetSelection()));
    return page;
}

void MainFrame::OnPreferences(wxCommandEvent& event)
{
    SettingsDlg dlg(this);
    if(dlg.ShowModal() == wxID_OK) {
        wxConfigBase* conf = wxConfigBase::Get();
        conf->Write("/Settings/redis-server", dlg.GetRedisServer());
        conf->Write("/Settings/redis-cli", dlg.GetRedisCli());
        conf->Flush();
    }
}
void MainFrame::OnRunInstances(wxCommandEvent& event)
{
    // Execute the instances
    ClusterPage* page = GetActivePage();
    if(!page) { return; }
    if(page->GetClusterPath().IsEmpty() || page->GetClusterPath() == "/" ||
       !wxFileName::DirExists(page->GetClusterPath())) {
        wxMessageBox("Invalid or empty path", "Error", wxICON_ERROR | wxCENTER);
        return;
    }
    ClusterConfig conf(page);
    conf.Run();
}

void MainFrame::OnRunUI(wxUpdateUIEvent& event) {}

void MainFrame::SetFrame(MainFrame* frame)
{
    m_mainFrame = frame;
}

void MainFrame::Log(const wxString& message, int depth)
{
    wxString prefix = wxString(' ', (4 * depth));
    m_mainFrame->GetStcLog()->ScrollToEnd();
    m_mainFrame->GetStcLog()->AppendText(prefix + message + "\n");
    m_mainFrame->GetStcLog()->SetInsertionPointEnd();
    m_mainFrame->GetStcLog()->ClearSelections();
    m_mainFrame->GetStcLog()->ScrollToEnd();
}
