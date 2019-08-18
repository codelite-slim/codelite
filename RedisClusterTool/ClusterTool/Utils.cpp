#include "Utils.hpp"
#include <wx/utils.h>

wxString Utils::WrapWithQuotes(const wxString& str)
{
    wxString s = str;
    if(s.Contains(" ")) { s.Prepend("\"").Prepend("\""); }
    return s;
}

wxString Utils::WrapInShell(const wxString& cmd)
{
    wxString command;
#ifdef __WXMSW__
    wxChar* shell = wxGetenv(wxT("COMSPEC"));
    if(!shell) shell = (wxChar*)wxT("CMD.EXE");
    command << shell << wxT(" /C ");
    if(cmd.StartsWith("\"") && !cmd.EndsWith("\"")) {
        command << "\"" << cmd << "\"";
    } else {
        command << cmd;
    }
#else
    wxString tmp = cmd;
    command << wxT("/bin/sh -c '");
    // escape any single quoutes
    tmp.Replace("'", "\\'");
    command << tmp << wxT("'");
#endif
    return command;
}
