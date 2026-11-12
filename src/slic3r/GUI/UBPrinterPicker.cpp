#include "UBPrinterPicker.hpp"

#include "GUI.hpp"
#include "GUI_App.hpp"
#include "I18N.hpp"
#include "MainFrame.hpp"
#include "PresetComboBoxes.hpp"
#include "Widgets/WebView.hpp"
#include "libslic3r/Preset.hpp"
#include "libslic3r/PresetBundle.hpp"
#include "libslic3r/Utils.hpp"
#include "libslic3r/format.hpp"

#include <algorithm>
#include <string>

#include <boost/filesystem.hpp>
#include <nlohmann/json.hpp>
#include <wx/base64.h>
#include <wx/dcbuffer.h>
#include <wx/dcmemory.h>
#include <wx/dcscreen.h>
#include <wx/event.h>
#include <wx/image.h>
#include <wx/imagjpeg.h>
#include <wx/mstream.h>
#include <wx/timer.h>
#include <wx/webview.h>

namespace Slic3r { namespace GUI {

namespace {

// file:// address of the model's cover picture (resources/profiles/<vendor>/<model>_cover.png), if any.
std::string cover_url(const PresetBundle& bundle, const std::string& model)
{
    for (const auto& vendor : bundle.vendors)
        for (const auto& printer_model : vendor.second.models)
            if (printer_model.name == model) {
                const boost::filesystem::path path = boost::filesystem::path(resources_dir()) / "profiles" / vendor.second.id / (model + "_cover.png");
                if (boost::filesystem::exists(path))
                    return into_u8(file_url_from_path(path));
            }
    return {};
}

// "Unbound3D Hercules" -> "Hercules"
std::string short_model(const std::string& model)
{
    const std::string vendor = "Unbound3D ";
    return model.rfind(vendor, 0) == 0 ? model.substr(vendor.size()) : model;
}

} // namespace

void UBPrinterPicker::open(PlaterPresetComboBox* combo)
{
    wxWindow* frame = wxGetApp().mainframe;
    if (combo == nullptr || frame == nullptr || !combo->IsEnabled())
        return;

    // One card per printer in the list (printer models and the user's own printers), as in the drop-down list.
    PresetBundle&       bundle   = *wxGetApp().preset_bundle;
    const int           selected = combo->GetSelection();
    nlohmann::json      cards    = nlohmann::json::array();
    for (unsigned int i = 0; i < combo->GetCount(); ++i) {
        const auto marker = reinterpret_cast<std::size_t>(combo->GetClientData(i));
        if (marker >= PresetComboBox::LABEL_ITEM_DISABLED && marker < PresetComboBox::LABEL_ITEM_MAX)
            continue; // headings and "Select/Remove printers" entries
        std::string preset_name = into_u8(combo->GetItemAlias(i));
        if (preset_name.empty())
            preset_name = into_u8(combo->GetString(i));
        const Preset* preset = bundle.printers.find_preset(preset_name);
        if (preset == nullptr)
            continue;
        const std::string model     = preset->config.has("printer_model") ? preset->config.opt_string("printer_model") : std::string();
        const bool        connected = preset->config.has("print_host") && !preset->config.opt_string("print_host").empty();
        std::string       kind;
        if (preset->is_system)
            kind = _u8L("Printer model");
        else if (connected)
            // TRN: under a printer card in the printer picker, %1% is the printer model, e.g. "Hercules"
            kind = Slic3r::format(_u8L("%1% · connected"), short_model(model));
        else
            // TRN: under a printer card in the printer picker, %1% is the printer model, e.g. "Hercules"
            kind = Slic3r::format(_u8L("%1% · your settings"), short_model(model));
        cards.push_back({{"i", static_cast<int>(i)},
                         {"name", into_u8(combo->GetString(i))},
                         {"kind", kind},
                         {"model", model},
                         {"sel", static_cast<int>(i) == selected},
                         {"cover", cover_url(bundle, model)}});
    }
    if (cards.empty())
        return;

    // The main window as it looks now: the page blurs it in, so the picker seems to rise over the window.
    const wxPoint top_left = frame->ClientToScreen(wxPoint(0, 0));
    const wxSize  size     = frame->GetClientSize();
    wxBitmap      shot;
    std::string   background;
#ifdef __WXMSW__
    // (Not on macOS: reading the screen there asks for the "Screen Recording" permission.)
    shot = wxBitmap(size);
    {
        wxScreenDC screen;
        wxMemoryDC memory(shot);
        memory.Blit(0, 0, size.x, size.y, &screen, top_left.x, top_left.y);
    }
    wxImage image = shot.ConvertToImage();
    image.Rescale(std::max(1, size.x / 2), std::max(1, size.y / 2), wxIMAGE_QUALITY_HIGH);
    image.SetOption(wxIMAGE_OPTION_QUALITY, 80);
    wxMemoryOutputStream jpeg;
    if (image.SaveFile(jpeg, wxBITMAP_TYPE_JPEG))
        background = "data:image/jpeg;base64," +
                     into_u8(wxBase64Encode(jpeg.GetOutputStreamBuffer()->GetBufferStart(), jpeg.GetSize()));
#endif

    nlohmann::json data = {{"bg", background},
                           {"dark", wxGetApp().dark_mode()},
                           {"title", _u8L("Select printer")},
                           {"sub", _u8L("Click a printer to use it")},
                           {"hint", _u8L("Click outside or press Esc to close")},
                           {"cards", cards}};

    int pick = -1;
    {
        UBPrinterPicker dialog(frame, wxRect(top_left, size), shot, from_u8("ubInit(" + data.dump() + ");"));
        if (dialog.m_web == nullptr)
            return;
        dialog.ShowModal();
        pick = dialog.m_pick;
    }

    // Select it exactly as the drop-down list does.
    if (pick >= 0 && pick < static_cast<int>(combo->GetCount()) && pick != combo->GetSelection()) {
        combo->SetSelection(pick);
        wxCommandEvent event(wxEVT_COMBOBOX, combo->GetId());
        event.SetEventObject(combo);
        event.SetInt(pick);
        event.SetString(combo->GetString(pick));
        combo->GetEventHandler()->ProcessEvent(event);
    }
}

UBPrinterPicker::UBPrinterPicker(wxWindow* frame, const wxRect& screen_rect, const wxBitmap& shot, const wxString& init_script)
    : wxDialog(frame, wxID_ANY, wxEmptyString, screen_rect.GetPosition(), screen_rect.GetSize(), wxBORDER_NONE | wxFRAME_NO_TASKBAR)
    , m_shot(shot)
    , m_init_script(init_script)
{
    // Until the page is ready the dialog shows the window as it was, so nothing seems to jump.
    SetBackgroundStyle(wxBG_STYLE_PAINT);
    Bind(wxEVT_PAINT, [this](wxPaintEvent&) {
        wxAutoBufferedPaintDC dc(this);
        if (m_shot.IsOk())
            dc.DrawBitmap(m_shot, 0, 0);
        else {
            dc.SetBackground(wxBrush(wxColour(43, 43, 43)));
            dc.Clear();
        }
    });
    Bind(wxEVT_LEFT_DOWN, [this](wxMouseEvent&) { EndModal(wxID_CANCEL); });
    Bind(wxEVT_CHAR_HOOK, [this](wxKeyEvent& e) {
        if (e.GetKeyCode() == WXK_ESCAPE)
            EndModal(wxID_CANCEL);
        else
            e.Skip();
    });

    const boost::filesystem::path page = boost::filesystem::path(resources_dir()) / "web" / "ubslicer" / "printer_picker.html";
    m_web = WebView::CreateWebView(this, file_url_from_path(page));
    if (m_web == nullptr)
        return;
    m_web->SetSize(0, 0, 1, 1); // grows to the whole dialog once the page has drawn

    auto loaded = std::make_shared<bool>(false);
    Bind(wxEVT_WEBVIEW_LOADED, [this, loaded](wxWebViewEvent&) {
        if (*loaded)
            return;
        *loaded = true;
        WebView::RunScript(m_web, m_init_script);
    }, m_web->GetId());
    Bind(wxEVT_WEBVIEW_SCRIPT_MESSAGE_RECEIVED, &UBPrinterPicker::on_message, this, m_web->GetId());

    // If the page never answers, don't leave the window covered.
    m_timeout.SetOwner(this);
    Bind(wxEVT_TIMER, [this](wxTimerEvent&) {
        if (m_web->GetSize().GetWidth() <= 1 && IsModal())
            EndModal(wxID_CANCEL);
    }, m_timeout.GetId());
    m_timeout.StartOnce(5000);
}

void UBPrinterPicker::on_message(wxWebViewEvent& evt)
{
    try {
        const nlohmann::json msg = nlohmann::json::parse(into_u8(evt.GetString()));
        if (msg.contains("ready")) {
            m_web->SetSize(GetClientSize());
            m_web->SetFocus();
        } else if (msg.contains("pick")) {
            m_pick = msg["pick"].get<int>();
            if (IsModal())
                EndModal(wxID_OK);
        } else if (msg.contains("close")) {
            if (IsModal())
                EndModal(wxID_CANCEL);
        }
    } catch (...) {
        // not a picker message
    }
}

}} // namespace Slic3r::GUI
