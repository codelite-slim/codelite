#ifndef UTILS_HPP
#define UTILS_HPP

#include <wx/filefn.h>
#include <wx/string.h>

class Utils
{
public:
    static wxString WrapWithQuotes(const wxString& str);
    static wxString WrapInShell(const wxString& cmd);
};

class DirSaver
{
    wxString m_originPath;

public:
    DirSaver() { m_originPath = ::wxGetCwd(); }
    ~DirSaver() { ::wxSetWorkingDirectory(m_originPath); }
};
#endif // UTILS_HPP
