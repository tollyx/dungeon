//
// Created by Adrian on 2017-09-23.
//

#include <cstdio>
#include "imgui.h"

namespace ImGui {
  int BeginTable(const char* columnsId, const char** headers, float* widths, int count, bool draw_border)
  {
    if(count<=0)
      return 0;

    // Draw column headers
    ImGuiStyle & style = ImGui::GetStyle();
    const ImVec2 firstTextSize = ImGui::CalcTextSize(headers[0], NULL, true);

    ImGui::BeginChild(columnsId, ImVec2(0,firstTextSize.y + 2 * style.ItemSpacing.y), true);

    char str_id[256];
    sprintf(str_id, "tbl0_%s", columnsId);
    ImGui::Columns(count, str_id, draw_border);

    float offset = 0.0f;
    for(int i=0; i < count; i++)
    {
      ImGui::SetColumnOffset(i, offset);

      if(widths[i] <= 0)
      {
        const ImVec2 textsize = ImGui::CalcTextSize(headers[i], NULL, true);
        const float colSizeX = (textsize.x + 2 * style.ItemSpacing.x);
        widths[i] = colSizeX + 1;
      }

      if(i < (count-1))
      {
        float curOffset = offset;
        offset = ImGui::GetColumnOffset(i+1);
        widths[i] = offset - curOffset + 1;
      }

      ImGui::Text(headers[i]);
      ImGui::NextColumn();
    }

    ImGui::Columns(1);
    ImGui::EndChild();

    // Draw body
    str_id[3] = '1';
    columnsId = str_id;

    ImGui::BeginChild(columnsId, ImVec2(0,0), true);
    ImGui::Columns(count, columnsId, draw_border);

    offset = 0.0f;
    for(int i=0; i < count; i++)
    {
      ImGui::SetColumnOffset(i, offset);
      offset += widths[i] - 1;
    }

    return 1;
  }

  void EndTable()
  {
    ImGui::Columns(1);
    ImGui::EndChild();
  }
}
