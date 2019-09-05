#ifndef CLFILESYSTEMWORKSPACECONFIG_HPP
#define CLFILESYSTEMWORKSPACECONFIG_HPP

#include "codelite_exports.h"
#include "macros.h"
#include <wx/arrstr.h>
#include "JSON.h"
#include <wx/sharedptr.h>
#include <map>
#include <wxStringHash.h>

class WXDLLIMPEXP_SDK clFileSystemWorkspaceConfig
{
protected:
    size_t m_flags = 0;
    wxStringMap_t m_buildTargets;
    wxArrayString m_compileFlags;
    wxString m_fileExtensions;
    wxString m_name;

public:
    JSONItem ToJSON() const;
    void FromJSON(const JSONItem& json);

    void SetBuildTargets(const wxStringMap_t& buildTargets) { this->m_buildTargets = buildTargets; }
    void SetCompileFlags(const wxArrayString& compileFlags) { this->m_compileFlags = compileFlags; }
    void SetCompileFlags(const wxString& compileFlags);
    void SetFileExtensions(const wxString& fileExtensions) { this->m_fileExtensions = fileExtensions; }
    void SetFlags(size_t flags) { this->m_flags = flags; }
    void SetName(const wxString& name) { this->m_name = name; }
    const wxStringMap_t& GetBuildTargets() const { return m_buildTargets; }
    const wxArrayString& GetCompileFlags() const { return m_compileFlags; }
    wxString GetCompileFlagsAsString() const;
    const wxString& GetFileExtensions() const { return m_fileExtensions; }
    size_t GetFlags() const { return m_flags; }
    const wxString& GetName() const { return m_name; }

    wxArrayString GetSearchPaths(const wxFileName& workspaceFile) const;

    clFileSystemWorkspaceConfig();
    ~clFileSystemWorkspaceConfig() {}

    typedef wxSharedPtr<clFileSystemWorkspaceConfig> Ptr_t;
};

class WXDLLIMPEXP_SDK clFileSystemWorkspaceSettings
{
    // Workspace settings
    // we use map to keep the entries sorted
    std::map<wxString, clFileSystemWorkspaceConfig::Ptr_t> m_configsMap;
    wxString m_selectedConfig;
    wxString m_workspaceType;

protected:
    JSONItem ToJSON(JSONItem& item) const;
    void FromJSON(const JSONItem& json);

public:
    clFileSystemWorkspaceSettings();
    virtual ~clFileSystemWorkspaceSettings();

    bool Save(const wxFileName& filename);
    bool Load(const wxFileName& filename);

    bool AddConfig(const wxString& name);
    bool DeleteConfig(const wxString& name);
    bool SetSelectedConfig(const wxString& name);
    clFileSystemWorkspaceConfig::Ptr_t GetSelectedConfig() const;
    clFileSystemWorkspaceConfig::Ptr_t GetConfig(const wxString& name) const;
    const std::map<wxString, clFileSystemWorkspaceConfig::Ptr_t>& GetConfigsMap() const { return m_configsMap; }
    void Clear();
};

#endif // CLFILESYSTEMWORKSPACECONFIG_HPP
