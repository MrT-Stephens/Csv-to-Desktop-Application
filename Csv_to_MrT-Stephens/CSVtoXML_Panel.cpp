#include "CSVtoXML_Panel.h"

CSVtoXML_Panel::CSVtoXML_Panel(wxWindow* _parent, const std::string& _name, const mrtApp::AppColours* _colours)
	: CSVto_PanelBase(_parent, _name, _colours)
{
	SetupDataInputSection();
	SetupOutputSettingsSection();
	SetupSpecificOutputSectionItems();
	SetupDataOutputSection();

	SetSizer(m_MainSizer);
}

void CSVtoXML_Panel::SetupSpecificOutputSectionItems()
{
	m_OutputSettingsSizer3 = new wxBoxSizer(wxHORIZONTAL);

	m_ExcludePrologCheckBox = new wxCheckBox(this, wxID_ANY, "Exclude prolog", wxDefaultPosition, wxDefaultSize);
	m_ExcludePrologCheckBox->SetMinSize({ 120, 30 });
	m_ExcludePrologCheckBox->SetOwnFont(MAIN_FONT_TEXT(11));
	m_ExcludePrologCheckBox->SetOwnBackgroundColour(m_Colours->BACKGROUND);
	m_ExcludePrologCheckBox->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_ExcludePrologCheckBox->SetToolTip("Exclude the prolog in the generated XML");

	m_ExcludePrologCheckBox->Bind(wxEVT_CHECKBOX, [this](wxCommandEvent& event)
		{
			if (!m_OutputDataTextBox->IsEmpty())
			{
				PopulateData();
			}
		}
	);

	m_MinifyXmlCheckBox = new wxCheckBox(this, wxID_ANY, "Minify XML statements", wxDefaultPosition, wxDefaultSize);
	m_MinifyXmlCheckBox->SetMinSize({ 120, 30 });
	m_MinifyXmlCheckBox->SetOwnFont(MAIN_FONT_TEXT(11));
	m_MinifyXmlCheckBox->SetOwnBackgroundColour(m_Colours->BACKGROUND);
	m_MinifyXmlCheckBox->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_MinifyXmlCheckBox->SetToolTip("Wites all the XML statements onto one row");

	m_MinifyXmlCheckBox->Bind(wxEVT_CHECKBOX, [this](wxCommandEvent& event)
		{
			if (!m_OutputDataTextBox->IsEmpty())
			{
				PopulateData();
			}
		}
	);

	m_LeftSizer = new wxBoxSizer(wxVERTICAL);
	m_LeftSizer->Add(m_ExcludePrologCheckBox, 0, wxRIGHT | wxLEFT | wxTOP | wxEXPAND | wxCENTER, FromDIP(10));
	m_LeftSizer->Add(m_MinifyXmlCheckBox, 0, wxALL | wxEXPAND | wxCENTER, FromDIP(10));

	m_NameSpaceTextBox = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
	m_NameSpaceTextBox->SetValue("namespace");
	m_NameSpaceTextBox->SetMinSize({ 120, 30 });
	m_NameSpaceTextBox->SetOwnFont(MAIN_FONT_TEXT(11));
	m_NameSpaceTextBox->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_NameSpaceTextBox->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_NameSpaceTextBox->SetToolTip("Set a namespace for the data, empty for no namespace");

	m_NameSpaceTextBox->Bind(wxEVT_TEXT, [this](wxCommandEvent& event)
		{
			if (!m_OutputDataTextBox->IsEmpty())
			{
				m_TextBoxTimer.Start(2000);
			}
		}
	);

	m_XmlnsTextBox = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
	m_XmlnsTextBox->SetValue("namespaceURI");
	m_XmlnsTextBox->SetMinSize({ 120, 30 });
	m_XmlnsTextBox->SetOwnFont(MAIN_FONT_TEXT(11));
	m_XmlnsTextBox->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_XmlnsTextBox->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_XmlnsTextBox->SetToolTip("Set a URI for the namespace, empty for no URI");

	m_XmlnsTextBox->Bind(wxEVT_TEXT, [this](wxCommandEvent& event)
		{
			if (!m_OutputDataTextBox->IsEmpty())
			{
				m_TextBoxTimer.Start(2000);
			}
		}
	);

	m_MiddleSizer = new wxBoxSizer(wxVERTICAL);
	m_MiddleSizer->Add(m_NameSpaceTextBox, 0, wxRIGHT | wxLEFT | wxTOP | wxEXPAND | wxCENTER, FromDIP(10));
	m_MiddleSizer->Add(m_XmlnsTextBox, 0, wxALL | wxEXPAND | wxCENTER, FromDIP(10));

	m_RootNameTextBox = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
	m_RootNameTextBox->SetValue("root");
	m_RootNameTextBox->SetMinSize({ 120, 30 });
	m_RootNameTextBox->SetOwnFont(MAIN_FONT_TEXT(11));
	m_RootNameTextBox->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_RootNameTextBox->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_RootNameTextBox->SetToolTip("Set a root name for the data");

	m_RootNameTextBox->Bind(wxEVT_TEXT, [this](wxCommandEvent& event)
		{
			if (!m_OutputDataTextBox->IsEmpty())
			{
				m_TextBoxTimer.Start(2000);
			}
		}
	);

	m_TextBoxTimer.Bind(wxEVT_TIMER, [this](wxTimerEvent& event)
		{
			if (m_CSVData != nullptr)
			{
				m_TextBoxTimer.Stop();
				PopulateData();
			}
		}
	);

	m_ElementNameTextBox = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
	m_ElementNameTextBox->SetValue("element");
	m_ElementNameTextBox->SetMinSize({ 120, 30 });
	m_ElementNameTextBox->SetOwnFont(MAIN_FONT_TEXT(11));
	m_ElementNameTextBox->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_ElementNameTextBox->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_ElementNameTextBox->SetToolTip("Set an element name for the data");

	m_ElementNameTextBox->Bind(wxEVT_TEXT, [this](wxCommandEvent& event)
		{
			if (!m_OutputDataTextBox->IsEmpty())
			{
				m_TextBoxTimer.Start(2000);
			}
		}
	);

	m_RightSizer = new wxBoxSizer(wxVERTICAL);
	m_RightSizer->Add(m_RootNameTextBox, 0, wxRIGHT | wxLEFT | wxTOP | wxEXPAND | wxCENTER, FromDIP(10));
	m_RightSizer->Add(m_ElementNameTextBox, 0, wxALL | wxEXPAND | wxCENTER, FromDIP(10));

	m_OutputSettingsSizer3->Add(m_LeftSizer, 1, wxALL | wxEXPAND | wxCENTER, FromDIP(0));
	m_OutputSettingsSizer3->Add(m_MiddleSizer, 1, wxALL | wxEXPAND | wxCENTER, FromDIP(0));
	m_OutputSettingsSizer3->Add(m_RightSizer, 1, wxALL | wxEXPAND | wxCENTER, FromDIP(0));

	m_MainSizer->Add(m_OutputSettingsSizer3, 0, wxEXPAND | wxALL, FromDIP(0));
}

void CSVtoXML_Panel::OutputFile()
{
	wxFileDialog saveFileDialog(this, "Download File", (wxStandardPaths::Get().GetDocumentsDir()), "", "Extensible Markup Language (*.xml)|*.xml", wxFD_SAVE);

	if (saveFileDialog.ShowModal() == wxID_OK)
	{
		std::ofstream file(saveFileDialog.GetPath().ToStdString());

		if (!file.is_open())
		{
			mrt::MrT_UniDialog errorDialog(this, "Error", "Failed to save file!\nPlease try to re-save the file.",
				m_Colours, wxICON(wxICON_ERROR), mrt::MrT_UniDialogType_OK, { 400, 200 });

			errorDialog.ShowModal();
		}
		else
		{
			file << std::move(m_OutputDataTextBox->GetValue().ToStdString());
		}
		file.close();
	}
}

void CSVtoXML_Panel::PopulateOutputDataTextBox()
{
#if defined(MRT_DEBUG)
	auto start = std::chrono::high_resolution_clock::now();
#endif

	{
		const std::vector<std::string>& header = m_CSVData->GetHeaderNames();

		mrt::XML_Node<std::string> root(m_RootNameTextBox->GetValue().ToStdString());

		if (!m_XmlnsTextBox->IsEmpty() && !m_NameSpaceTextBox->IsEmpty())
		{
			root.EmplaceAttribute("xmlns", m_XmlnsTextBox->GetValue().ToStdString());
		}

		for (size_t i0 = 0; i0 < m_CSVData->GetRowCount(); ++i0)
		{
			mrt::XML_Node<std::string> element(m_ElementNameTextBox->GetValue().ToStdString());

			for (size_t i1 = 0; i1 < m_CSVData->GetColumnCount(); ++i1)
			{
				const std::vector<std::string>& row = m_CSVData->GetRowData(i0);

				element.EmplaceChild(header[i1], row[i1]);
			}

			root.AddChild(element);
		}

		mrt::XML_Document<std::string> doc(root, "1.0", m_NameSpaceTextBox->GetValue().ToStdString(), !m_ExcludePrologCheckBox->GetValue(), !m_MinifyXmlCheckBox->GetValue());

		mrt::XML_Document<std::string>::OStrStream ss;

		doc.WriteDocumentToStream(&ss, doc);

		m_OutputDataTextBox->SetValue(ss.str());
	}

#if defined(MRT_DEBUG)
	auto stop = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

	OutputDebugString(L"[MRT_DEBUG] Time taken by 'PopulateOutputDataTextBox' (XML CLASS): " + std::to_string(duration.count()) + " milliseconds.\n");
#endif

	std::lock_guard<std::mutex> lock(m_OutputDataMutex);

	m_TextBoxThreadRunning = false;

	if (!m_ListViewThreadRunning)
	{
		LockOrUnlockItems(false);
	}
}

void CSVtoXML_Panel::LockOrUnlockItems(bool lock)
{
	m_UndoBtn->Enable(!lock);
	m_RedoBtn->Enable(!lock);
	m_LoadData->Enable(!lock);
	m_ExampleData->Enable(!lock);
	m_LowercaseBtn->Enable(!lock);
	m_UppercaseBtn->Enable(!lock);
	m_CapitalizeBtn->Enable(!lock);
	m_ClearBtn->Enable(!lock);
	m_TransposeBtn->Enable(!lock);
	m_DeleteBlanksBtn->Enable(!lock);
	m_RootNameTextBox->Enable(!lock);
	m_ElementNameTextBox->Enable(!lock);
	m_MinifyXmlCheckBox->Enable(!lock);
	m_ExcludePrologCheckBox->Enable(!lock);
}