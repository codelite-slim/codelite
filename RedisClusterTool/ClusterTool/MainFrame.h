#ifndef MAINFRAME_H
#define MAINFRAME_H
#include "wxcrafter.h"

class ClusterPage;
class MainFrame : public MainFrameBaseClass
{
    static MainFrame* m_mainFrame;

public:
    MainFrame(wxWindow* parent);
    virtual ~MainFrame();

    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void Save(ClusterPage* page);
    static void SetFrame(MainFrame* frame);
    static void Log(const wxString& message, int depth = 0);
    
protected:
    virtual void OnRunInstances(wxCommandEvent& event);
    virtual void OnPreferences(wxCommandEvent& event);
    ClusterPage* GetActivePage();

protected:
    virtual void OnDeploy(wxCommandEvent& event);
    virtual void OnSaveUI(wxUpdateUIEvent& event);
    virtual void OnSave(wxCommandEvent& event);
    virtual void OnNewCluster(wxCommandEvent& event);
};
#endif // MAINFRAME_H
