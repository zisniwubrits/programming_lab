// ProgrammingLabApp.cpp
#include "ProgrammingLabApp.h"
#include "MainFrame.h"

bool ProgrammingLabApp::OnInit()
{
    MainFrame* frame = new MainFrame("Programming Lab - CRUD Demo");
    frame->Show(true);
    return true;
}