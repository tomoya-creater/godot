/**************************************************************************/
/*  gmake_2d_editor_plugin.cpp                                            */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/

#include "gmake_2d_editor_plugin.h"

#include "editor/editor_interface.h"
#include "editor/editor_main_screen.h"
#include "editor/editor_node.h"
#include "editor/gui/editor_toaster.h"
#include "scene/scene_string_names.h"
#include "scene/gui/button.h"
#include "scene/gui/label.h"
#include "scene/gui/option_button.h"
#include "scene/gui/panel_container.h"
#include "scene/gui/box_container.h"

const GMake2DEditorPlugin::TemplateDescriptor GMake2DEditorPlugin::TEMPLATE_DESCRIPTORS[] = {
    { "platformer", "Platformer", "2D side-scroller starter with jump and movement." },
    { "roguelike", "Roguelike", "Room loop starter with interaction-focused defaults." },
    { "visual_novel", "Visual Novel", "Dialogue-first starter with branching flow scaffolding." },
    { "top_down_shooter", "Top-down Shooter", "Action starter with movement, shoot, and dash controls." },
};

void GMake2DEditorPlugin::_enforce_2d_mode() {
    EditorMainScreen *main_screen = EditorNode::get_editor_main_screen();
    if (main_screen == nullptr) {
        return;
    }

    if (main_screen->is_button_enabled(EditorMainScreen::EDITOR_3D)) {
        main_screen->set_button_enabled(EditorMainScreen::EDITOR_3D, false);
    }

    const int selected_index = main_screen->get_selected_index();
    if (selected_index == EditorMainScreen::EDITOR_3D || selected_index < 0) {
        EditorInterface *editor_interface = get_editor_interface();
        if (editor_interface != nullptr) {
            editor_interface->set_main_screen_editor("2D");
        }
    }
}

void GMake2DEditorPlugin::_update_template_preview(int p_index) {
    if (template_summary == nullptr) {
        return;
    }

    const int template_count = static_cast<int>(sizeof(TEMPLATE_DESCRIPTORS) / sizeof(TemplateDescriptor));
    if (p_index < 0 || p_index >= template_count) {
        template_summary->set_text("Template information is unavailable.");
        return;
    }

    const TemplateDescriptor &descriptor = TEMPLATE_DESCRIPTORS[p_index];
    String preview = String("id: ") + descriptor.id + "\n" + descriptor.description;
    template_summary->set_text(preview);
}

void GMake2DEditorPlugin::_on_template_selected(int p_index) {
    _update_template_preview(p_index);
}

void GMake2DEditorPlugin::_on_apply_template_pressed() {
    if (template_selector == nullptr) {
        return;
    }

    const int selected = template_selector->get_selected();
    const int template_count = static_cast<int>(sizeof(TEMPLATE_DESCRIPTORS) / sizeof(TemplateDescriptor));
    if (selected < 0 || selected >= template_count) {
        return;
    }

    const String message = String("Template selected: ") + TEMPLATE_DESCRIPTORS[selected].name + " (bootstrap only)";
    if (EditorToaster::get_singleton() != nullptr) {
        EditorToaster::get_singleton()->popup_str(message, EditorToaster::SEVERITY_INFO);
    }
}

void GMake2DEditorPlugin::_notification(int p_what) {
    switch (p_what) {
        case NOTIFICATION_READY: {
            _enforce_2d_mode();
        } break;
        case NOTIFICATION_EXIT_TREE: {
            if (template_panel != nullptr) {
                remove_control_from_container(CONTAINER_CANVAS_EDITOR_SIDE_LEFT, template_panel);
            }
        } break;
    }
}

String GMake2DEditorPlugin::get_plugin_name() const {
    return "GMake2D";
}

bool GMake2DEditorPlugin::has_main_screen() const {
    return false;
}

GMake2DEditorPlugin::GMake2DEditorPlugin() {
    template_panel = memnew(PanelContainer);
    template_panel->set_name("GMake2DTemplatePanel");

    VBoxContainer *layout = memnew(VBoxContainer);
    layout->add_theme_constant_override("separation", 6);
    template_panel->add_child(layout);

    Label *title = memnew(Label);
    title->set_text("2D Template Quick Start");
    layout->add_child(title);

    template_selector = memnew(OptionButton);
    template_selector->set_h_size_flags(Control::SIZE_EXPAND_FILL);
    const int template_count = static_cast<int>(sizeof(TEMPLATE_DESCRIPTORS) / sizeof(TemplateDescriptor));
    for (int i = 0; i < template_count; i++) {
        template_selector->add_item(TEMPLATE_DESCRIPTORS[i].name, i);
    }
    template_selector->connect(SceneStringName(item_selected), callable_mp(this, &GMake2DEditorPlugin::_on_template_selected));
    layout->add_child(template_selector);

    template_summary = memnew(Label);
    template_summary->set_text("Select a template to view starter details.");
    layout->add_child(template_summary);

    apply_template_button = memnew(Button);
    apply_template_button->set_text("Apply Template (Bootstrap)");
    apply_template_button->connect(SceneStringName(pressed), callable_mp(this, &GMake2DEditorPlugin::_on_apply_template_pressed));
    layout->add_child(apply_template_button);

    add_control_to_container(CONTAINER_CANVAS_EDITOR_SIDE_LEFT, template_panel);
    _update_template_preview(0);
}
