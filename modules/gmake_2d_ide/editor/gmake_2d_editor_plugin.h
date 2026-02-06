/**************************************************************************/
/*  gmake_2d_editor_plugin.h                                              */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/

#pragma once

#include "editor/plugins/editor_plugin.h"

class Button;
class Label;
class OptionButton;
class PanelContainer;

class GMake2DEditorPlugin : public EditorPlugin {
    GDCLASS(GMake2DEditorPlugin, EditorPlugin)

    struct TemplateDescriptor {
        const char *id;
        const char *name;
        const char *description;
    };

    static const TemplateDescriptor TEMPLATE_DESCRIPTORS[];

    PanelContainer *template_panel = nullptr;
    OptionButton *template_selector = nullptr;
    Label *template_summary = nullptr;
    Button *apply_template_button = nullptr;

    void _enforce_2d_mode();
    void _update_template_preview(int p_index);
    void _on_template_selected(int p_index);
    void _on_apply_template_pressed();

protected:
    void _notification(int p_what);

public:
    String get_plugin_name() const override;
    bool has_main_screen() const override;

    GMake2DEditorPlugin();
};
