//
// Created by Adrian on 2017-09-23.
//

#ifndef DUNGEON_IMGUI_USER_H
#define DUNGEON_IMGUI_USER_H

namespace ImGui {
  /*  // [table src] https://github.com/ocornut/imgui/issues/513#issuecomment-240388455

      // Usage:
      static const char *headers[] = {
          "Index", "Color", "Flip?", "Filename"
      };
      static float widths[ IM_ARRAYSIZE(headers) ] = {};
      if( ImGui::BeginTable("WinTextureContent", headers, widths, IM_ARRAYSIZE(headers)) ) {
          // Draw as many rows as needed
          for( int i = 0; i < 10; ++i ) {
              ImGui::Text("%d", i);                              ImGui::NextColumn();
              ImGui::ColorButton( ImVec4(0.5f,0.2f,i*0.3f,1.f)); ImGui::NextColumn();
              ImGui::Text("%s", i % 2 ? "yes" : "no");           ImGui::NextColumn();
              ImGui::Text(__FILE__);                             ImGui::NextColumn();
          }
          ImGui::EndTable();
      }
  */
  int  BeginTable(const char* columnsId, const char** headers, float *widths, int count, bool border=true);
  void EndTable();
}

#endif //DUNGEON_IMGUI_USER_H
