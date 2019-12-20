
/*

    Goldleaf - Multipurpose homebrew tool for Nintendo Switch
    Copyright (C) 2018-2019  XorTroll

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

#include <ui/ui_Utils.hpp>
#include <ui/ui_MainApplication.hpp>

extern ui::MainApplication::Ref global_app;
extern set::Settings global_settings;

namespace ui
{
    String clipboard;

    void SetClipboard(String Path)
    {
        clipboard = Path;
    }
    
    void ClearClipboard()
    {
        clipboard = "";
    }

    bool ClipboardEmpty()
    {
        return clipboard.empty();
    }
    
    bool ClipboardNotEmpty()
    {
        return !clipboard.empty();
    }

    void ShowPowerTasksDialog(String Title, String Message)
    {
        int sopt = global_app->CreateShowDialog(Title, Message, { set::GetDictionaryEntry(233), set::GetDictionaryEntry(232), set::GetDictionaryEntry(18) }, true);
        if(sopt < 0) return;
        spsmInitialize();
        spsmShutdown(sopt == 1);
        spsmExit();
    }

    String AskForText(String Guide, String Initial, int MaxSize)
    {
        String out = "";
        char tmpout[FS_MAX_PATH] = { 0 };
        SwkbdConfig kbd;
        Result rc = swkbdCreate(&kbd, 0);
        if(R_SUCCEEDED(rc))
        {
            swkbdConfigMakePresetDefault(&kbd);
            if(MaxSize > 0) swkbdConfigSetStringLenMax(&kbd, (u32)MaxSize);
            if(Guide != "") swkbdConfigSetGuideText(&kbd, Guide.AsUTF8().c_str());
            if(Initial != "") swkbdConfigSetInitialText(&kbd, Initial.AsUTF8().c_str());
            rc = swkbdShow(&kbd, tmpout, sizeof(tmpout));
            if(R_SUCCEEDED(rc)) out = String(tmpout);
        }
        swkbdClose(&kbd);
        return out;
    }

    void HandleResult(Result OSError, String Context)
    {
        if(OSError != 0)
        {
            err::Error err = err::DetermineError(OSError);
            char displayerr[0x10] = {0};
            sprintf(displayerr, "%04d-%04d", 2000 + R_MODULE(err.OSError), R_DESCRIPTION(err.OSError));
            String emod = err.Module + " (" + std::to_string(R_MODULE(err.OSError)) + ")";
            String edesc = err.Description + " (" + std::to_string(R_DESCRIPTION(err.OSError)) + ")";
            global_app->CreateShowDialog(set::GetDictionaryEntry(266), Context + "\n\n" + set::GetDictionaryEntry(266) + ": " + std::string(displayerr) + " (" + hos::FormatHex(err.OSError) + ")\n" + set::GetDictionaryEntry(264) + ": " + emod + "\n" + set::GetDictionaryEntry(265) + ": " + edesc + "", { set::GetDictionaryEntry(234) }, false);
        }
    }
}