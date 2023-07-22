#include "CSVtoMarkdown.h"

CSVtoMarkdown_Panel::CSVtoMarkdown_Panel(wxWindow* _parent, const std::string& _name, const mrtApp::AppColours* _colours)
	: CSVto_PanelBase(_parent, _name, _colours)
{
	SetupDataInputSection();
	SetupOutputSettingsSection();
	SetupSpecificOutputSectionItems();
	SetupDataOutputSection();

	SetSizer(m_MainSizer);
}

void CSVtoMarkdown_Panel::SetupSpecificOutputSectionItems()
{

}

void CSVtoMarkdown_Panel::PopulateOutputDataTextBox()
{
#if defined(MRT_DEBUG)
	auto start = std::chrono::high_resolution_clock::now();
#endif

	{
		mrt::MarkdownTableGenerator markdownTableGenerator(m_CSVData->GetHeaderNames(), m_CSVData->GetTableData(), m_CSVData->GetMaxColumnWidths());

		markdownTableGenerator.GenerateMarkdownTable(m_TableStyle->GetSelection(), m_TextAlignment->GetSelection(), m_BoldFirstRow->GetValue(), m_BoldFirstCol.GetValue());

		m_OutputDataTextBox->AppendText(std::forward<std::string>(markdownTableGenerator.GetMarkdownTableStream().str()));
	}

#if defined(MRT_DEBUG)
	auto stop = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

	OutputDebugString(L"[MRT_DEBUG] Time taken by 'PopulateOutputDataTextBox' (MARKDOWN CLASS): " + std::to_string(duration.count()) + " milliseconds.\n");
#endif

	std::lock_guard<std::mutex> lock(m_OutputDataMutex);

	m_TextBoxThreadRunning = false;

	if (!m_ListViewThreadRunning)
	{
		LockOrUnlockItems(false);
	}
}

void CSVtoMarkdown_Panel::LockOrUnlockItems(bool lock)
{

}