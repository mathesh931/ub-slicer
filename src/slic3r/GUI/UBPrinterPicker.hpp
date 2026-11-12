#pragma once

#include <wx/bitmap.h>
#include <wx/dialog.h>
#include <wx/string.h>
#include <wx/timer.h>

class wxWebView;
class wxWebViewEvent;

namespace Slic3r { namespace GUI {

class PlaterPresetComboBox;

// UB Slicer: clicking the printer box opens this picker instead of the drop-down list. It covers the
// main window: the window is blurred behind it and every printer is a card (drawing, name below).
// The page is resources/web/ubslicer/printer_picker.html; a pick selects that entry of the combo box
// exactly as the drop-down list would.
class UBPrinterPicker : public wxDialog
{
public:
    static void open(PlaterPresetComboBox* combo);

private:
    UBPrinterPicker(wxWindow* frame, const wxRect& screen_rect, const wxBitmap& shot, const wxString& init_script);
    void on_message(wxWebViewEvent& evt);

    wxWebView* m_web { nullptr };
    wxBitmap   m_shot;
    wxString   m_init_script;
    wxTimer    m_timeout;
    int        m_pick { -1 };
};

}} // namespace Slic3r::GUI
