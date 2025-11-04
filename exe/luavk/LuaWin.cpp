////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "LuaWin.hpp"
#include <yq/tachyon/MyImGui.hpp>
#include <yq/tachyon/api/Payload.hpp>
#include <yq/tachyon/api/WidgetMetaWriter.hpp>
#include <yq/tachyon/ui/UIWriters.hxx>
#include <ImGuiFileDialog.h>

using namespace yq;
//using namespace yq::lua;
using namespace yq::tachyon;

YQ_TACHYON_IMPLEMENT(LuaWin)

LuaWin::LuaWin(TachyonID luavm) : m_lua(luavm)
{
}

LuaWin::~LuaWin()
{
}

void    LuaWin::_script(const std::filesystem::path& fp)
{
}

void    LuaWin::imgui(ViContext&u) 
{
    Widget::imgui(UI,u);

    if(m_fileMode != FileMode::None){
        ImVec2  minSize = { (float)(0.5 * width()), (float)(0.5 * height()) };
        if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey", ImGuiWindowFlags_NoCollapse, minSize)) {
            if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
                std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
                
                switch(m_fileMode){
                case FileMode::None:
                    break;
                }
            }
            ImGuiFileDialog::Instance()->Close();
        }
    }
}

void LuaWin::cmd_lua_file()
{
}

void LuaWin::cmd_user_input(const Payload& pay)
{
    if(pay.arguments().empty())
        return ;
    auto   line    = to_string(pay.arguments()[0]);
    if(!line)
        return ;
    
    
}

////////////////////////////////////

void LuaWin::init_meta()
{
    auto w = writer<LuaWin>();
    w.description("Lua Window");
    auto app        = w.imgui(UI, APP);
    
    auto mmb        = app.menubar(MAIN);
    mmb.uid("mmb");
    
    auto file       = mmb.menu("Lua");
    file.menuitem("Run...");
}
