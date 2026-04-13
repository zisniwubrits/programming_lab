// ProgrammingLabApp.cpp
#include "ProgrammingLabApp.h"
#include "MainFrame.h"

bool ProgrammingLabApp::OnInit()
{
    MainFrame* frame = new MainFrame(wxT("个人账本管理系统"));
    frame->Show(true);

    wxLocale locale(wxLANGUAGE_CHINESE_SIMPLIFIED);
    locale.AddCatalogLookupPathPrefix(wxT("locale"));
    
    return true;
}