// ProgrammingLabApp.cpp
#include "ProgrammingLabApp.h"
#include "MainFrame.h"

bool ProgrammingLabApp::OnInit()
{
    MainFrame* frame = new MainFrame("Programming Lab - CRUD Demo");
    frame->Show(true);

    wxLocale locale(wxLANGUAGE_CHINESE_SIMPLIFIED);
    locale.AddCatalogLookupPathPrefix(wxT("locale"));
    
    return true;
}