/**************************************************************************/
/*  register_types.cpp                                                    */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/

#include "register_types.h"

#ifdef TOOLS_ENABLED
#include "editor/gmake_2d_editor_plugin.h"
#include "editor/plugins/editor_plugin.h"
#endif

void initialize_gmake_2d_ide_module(ModuleInitializationLevel p_level) {
#ifdef TOOLS_ENABLED
    if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR) {
        EditorPlugins::add_by_type<GMake2DEditorPlugin>();
    }
#else
    (void)p_level;
#endif
}

void uninitialize_gmake_2d_ide_module(ModuleInitializationLevel p_level) {
    (void)p_level;
}
