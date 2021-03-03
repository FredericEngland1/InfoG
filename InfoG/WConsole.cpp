#include "WConsole.h"

#include <iostream>

WConsole::WConsole(std::string wName) : wName(wName)
{
}

void WConsole::render()
{
    if (!p_open) return;

    ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin(wName.c_str(), &p_open))
    {
        ImGui::End();
        return;
    }


    if (ImGui::BeginPopupContextItem())
    {
        if (ImGui::MenuItem("Close Console"))
            p_open = false;
        ImGui::EndPopup();
    }

    if (ImGui::SmallButton("Clear")) { clearLog(); }
    ImGui::SameLine();
    bool copyToLog = ImGui::SmallButton("Copy");

    ImGui::Separator();

    // TODO commandes

    textFilter.Draw("Filter", 180);

    ImGui::Separator();

    ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
    if (ImGui::BeginPopupContextWindow())
    {
        if (ImGui::Selectable("Clear")) clearLog();
        ImGui::EndPopup();
    }

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));
    if (copyToLog) ImGui::LogToClipboard();

    for (std::pair<std::string, LogType> logEntry : logs)
    {
        if (!textFilter.PassFilter(logEntry.first.c_str())) continue; // If it doesn't pass the filter continue to next entry

        ImVec4 color;

        switch (logEntry.second)
        {
        case information:
            color = ImVec4(0.8f, 1.0, 1.0f, 1.0f);

            break;
        case error:
            color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);

            break;
        default:
            break;
        }

        ImGui::PushStyleColor(ImGuiCol_Text, color);

        ImGui::TextUnformatted(logEntry.first.c_str());

        ImGui::PopStyleColor();

        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) ImGui::SetScrollHereY(1.0f);
    }

    if (copyToLog) ImGui::LogFinish();

    ImGui::PopStyleVar();

    ImGui::EndChild();
    ImGui::End();
}

void WConsole::addLog(std::string log, LogType logType)
{
    logs.push_back({ log, logType });
}

void WConsole::removeLog(std::string log)
{
	auto match = std::find_if(logs.begin(), logs.end(), [&log](const std::pair<std::string, LogType>& element) { return element.first == log; });
	if (match != logs.end()) logs.erase(match);
}

void WConsole::removeLog(unsigned int logNum)
{
	logs.erase(logs.begin() + logNum);
}

void WConsole::clearLog()
{
	logs.clear();
}
