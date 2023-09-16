#pragma once

#include <deque>
#include <format>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <filesystem>

namespace mrt
{
	  /*****************************/
     /* XML_Attribute Declaration */
    /*****************************/

	template <class _StrType>
	struct XML_Attribute
	{
		_StrType m_Name;
		_StrType m_Value;
	};

	  /************************/
     /* XML_Node Declaration */
    /************************/

	template <class _StrType>
	class XML_Node
	{
	public:
		// Type Aliases
		using ValueType = typename _StrType::value_type;
		using SizeType = typename _StrType::size_type;
	private:
		_StrType m_Name;
		_StrType m_Value;
		std::vector<XML_Attribute<_StrType>> m_Attributes;
		std::vector<XML_Node<_StrType>> m_Children;
	public:
		// Constructors
		XML_Node();
		XML_Node(const _StrType& name, const _StrType& value = _StrType());

		// Copiers & Assignments
		XML_Node(const XML_Node& other);
		XML_Node(XML_Node&& other) noexcept;
		XML_Node& operator=(const XML_Node& other);

		// Member Functions
		void SetName(_StrType name);
		const _StrType& GetName() const;

		void SetValue(_StrType value);
		const _StrType& GetValue() const;

		XML_Node<_StrType>& AddAttribute(const _StrType& name, const _StrType& value);
		template <class... Args> XML_Node<_StrType>& EmplaceAttribute(Args&&... args);

		XML_Node<_StrType>& AddChild(const XML_Node& child);
		template <class... Args> XML_Node<_StrType>& EmplaceChild(Args&&... args);

		XML_Attribute<_StrType>& GetAttribute(SizeType index);
		const XML_Attribute<_StrType>& GetAttribute(SizeType index) const;

		XML_Node<_StrType>& GetChild(SizeType index);
		const XML_Node<_StrType>& GetChild(SizeType index) const;

		SizeType GetChildCount() const;
		SizeType GetAttributeCount() const;

		std::vector<XML_Attribute<_StrType>>& GetAllAttributes();
		const std::vector<XML_Attribute<_StrType>>& GetAllAttributes() const;

		std::vector<XML_Node<_StrType>>& GetAllChildren();
		const std::vector<XML_Node<_StrType>>& GetAllChildren() const;
	};

	enum class XML_Document_FileError
	{
		SUCCESS = 0,
		FAILED_TO_OPEN = 1,
		FILE_EMPTY = 2
	};

	  /****************************/
     /* XML_Document Declaration */
    /****************************/

	template <class _StrType>
	class XML_Document
	{
	private:
		_StrType m_Version;
		XML_Node<_StrType> m_Root;
	public:
		// Type Aliases
		using ValueType = typename _StrType::value_type;
		using SizeType = typename _StrType::size_type;
		using IStream = typename std::basic_istream<ValueType>;
		using OStream = typename std::basic_ostream<ValueType>;
		using OStrStream = typename std::basic_ostringstream<ValueType>;
		using IStrStream = typename std::basic_istringstream<ValueType>;
		using OFstream = typename std::basic_ofstream<ValueType>;
		using IFstream = typename std::basic_ifstream<ValueType>;

		// Constructors
		XML_Document();
		XML_Document(const _StrType& version);
		XML_Document(const XML_Node<_StrType>& root, const _StrType& version);

		// Copiers & Assignments
		XML_Document(const XML_Document<_StrType>& other);
		XML_Document(XML_Document<_StrType>&& other) noexcept;
		XML_Document& operator=(const XML_Document<_StrType>& other);

		// Member Functions
		XML_Node<_StrType>& GetRoot();
		const XML_Node<_StrType>& GetRoot() const;

		_StrType& GetVersion();
		const _StrType& GetVersion() const;

		// Functions made for reading and writing to files. USE THESE IF OUTPUTTING OR INPUTTING TO FILE.
		static XML_Document_FileError ReadDocument(const _StrType& path, XML_Document& document);

		static XML_Document_FileError WriteDocument(const _StrType& path, const XML_Document& document, bool addProlog = true, bool newLines = true);

		// Recursive called functions to write nodes to ouput streams.
		static void WriteChildrenNodes(OStream* fs, const XML_Node<_StrType>& node, bool newLines, size_t tabs = 1);

		// Universal read and write functions. USE THESE IF OUTPUTTING OR INPUTTING TO STRING STREAM, COUT, etc.
		static void ReadDocumentFromStream(IStream* fs, XML_Document& document);

		static void WriteDocumentToStream(OStream* fs, const XML_Document& document, bool addProlog = true, bool newLines = true);
	};

	  /************************/
     /* MrT Global Functions */
    /************************/

	template <class _StrType>
	static _StrType getXMLprolog(const _StrType& version);

	template <class _StrType>
	static _StrType getStartNode(const XML_Node<_StrType>& node);

	template <class _StrType>
	static _StrType getEndNode(const XML_Node<_StrType>& node);

	template <class _StrType>
	static _StrType getTabs(size_t amount);

	template <class _StrType>
	static _StrType getNewLine(bool newLines);

	template <class _CastType, class _InType>
	static _CastType string_cast(const _InType& str);

	namespace mrtInternal
	{
		enum class XML_Token_Type
		{
			TAG_START = 0,
			TAG_END = 1,
			TAG_SELF_END = 2,
			VALUE = 3,
			END_OF_STRING = 4
		};

		  /*************************/
	     /* XML_Token Declaration */
	    /*************************/

		template <class _StrType>
		struct XML_Token
		{
			XML_Token_Type m_Type;
			_StrType m_Data;
		};

		  /*****************************/
	     /* XML_Tokenizer Declaration */
	    /*****************************/

		template <class _StrType>
		class XML_Tokenizer
		{
		public:
			// Type Aliases
			using SizeType = typename _StrType::size_type;
			using ValueType = typename _StrType::value_type;
			using IStream = typename std::basic_istream<typename _StrType::value_type>;
		private:
			// Member Variables
			_StrType m_InputData;
			SizeType m_Index;
		public:
			// Constructors
			XML_Tokenizer();
			XML_Tokenizer(IStream* inputStream);
			XML_Tokenizer(const _StrType& inputData);
			XML_Tokenizer(_StrType::const_iterator begin, _StrType::const_iterator end);

			// Copiers & Assignments
			XML_Tokenizer(const XML_Tokenizer& other) = delete;
			XML_Tokenizer(XML_Tokenizer&& other) noexcept = delete;
			XML_Tokenizer& operator=(const XML_Tokenizer& other) = delete;

			// Member Functions
			void SetInputData(const _StrType& inputData);

			XML_Token<_StrType> Next();

			bool HasNext() const;
		};

		  /*********************************/
	     /* XML_Tokens_Parser Declaration */
	    /*********************************/

		template <class _StrType>
		class XML_Tokenizer_Parser
		{
		public:
			// Type Aliases
			using ValueType = typename _StrType::value_type;
			using SizeType = typename _StrType::size_type;
			using IStream = typename std::basic_istream<typename _StrType::value_type>;
		private:
			// Member Variables
			_StrType m_InputData;
			XML_Node<_StrType>* m_Root;
			std::deque<_StrType> m_CurrentEndTags;
			std::deque<XML_Node<_StrType>*> m_CurrentNodes;
		public:
			// Constructors
			XML_Tokenizer_Parser(XML_Node<_StrType>* const root, IStream* inputStream);
			XML_Tokenizer_Parser(XML_Node<_StrType>* const root, const _StrType& inputString);
			XML_Tokenizer_Parser(XML_Node<_StrType>* const root, _StrType::const_iterator begin, _StrType::const_iterator end);

			// Copiers & Assignments
			XML_Tokenizer_Parser(const XML_Tokenizer_Parser& other) = delete;
			XML_Tokenizer_Parser(XML_Tokenizer_Parser&& other) noexcept = delete;
			XML_Tokenizer_Parser& operator=(const XML_Tokenizer_Parser& other) = delete;

			// Member Functions
			void Parse();

			// Static Functions
			static XML_Node<_StrType> ReadNode(const _StrType& line);

			static void ReadAttributes(const _StrType& line, XML_Node<_StrType>* const node);
		};
	}
}

  /***************************/
 /* XML_Node Implementation */
/***************************/

template <class _StrType>
mrt::XML_Node<_StrType>::XML_Node() : m_Name(), m_Value() { }

template <class _StrType>
mrt::XML_Node<_StrType>::XML_Node(const _StrType& name, const _StrType& value) : m_Name(name), m_Value(value) { }

template <class _StrType>
mrt::XML_Node<_StrType>::XML_Node(const XML_Node& other) : m_Name(other.m_Name), m_Value(other.m_Value), m_Attributes(other.m_Attributes), m_Children(other.m_Children) { }

template <class _StrType>
mrt::XML_Node<_StrType>::XML_Node(XML_Node&& other) noexcept :
	m_Name(std::forward<_StrType>(other.m_Name)),
	m_Value(std::forward<_StrType>(other.m_Value)),
	m_Attributes(std::forward<std::vector<XML_Attribute<_StrType>>>(other.m_Attributes)),
	m_Children(std::forward<std::vector<XML_Node<_StrType>>>(other.m_Children)) { }

template <class _StrType>
mrt::XML_Node<_StrType>& mrt::XML_Node<_StrType>::operator=(const XML_Node& other)
{
	m_Name = other.m_Name;
	m_Value = other.m_Value;
	m_Attributes = other.m_Attributes;
	m_Children = other.m_Children;

	return *this;
}

template <class _StrType>
void mrt::XML_Node<_StrType>::SetName(_StrType name) { m_Name = name; }

template <class _StrType>
const _StrType& mrt::XML_Node<_StrType>::GetName() const { return m_Name; }

template <class _StrType>
void mrt::XML_Node<_StrType>::SetValue(_StrType value) { m_Value = value; }

template <class _StrType>
const _StrType& mrt::XML_Node<_StrType>::GetValue() const { return m_Value; }

template <class _StrType>
mrt::XML_Node<_StrType>::SizeType mrt::XML_Node<_StrType>::GetChildCount() const { return m_Children.size(); }

template <class _StrType>
mrt::XML_Node<_StrType>::SizeType mrt::XML_Node<_StrType>::GetAttributeCount() const { return m_Attributes.size(); }

template <class _StrType>
mrt::XML_Node<_StrType>& mrt::XML_Node<_StrType>::AddAttribute(const _StrType& name, const _StrType& value)
{
	m_Attributes.emplace_back(name, value);

	return *this;
}

template <class _StrType> template <class... Args>
mrt::XML_Node<_StrType>& mrt::XML_Node<_StrType>::EmplaceAttribute(Args&&... args)
{
	m_Attributes.emplace_back(std::forward<Args>(args)...);

	return *this;
}

template <class _StrType>
mrt::XML_Node<_StrType>& mrt::XML_Node<_StrType>::AddChild(const XML_Node& child)
{
	m_Children.emplace_back(child);

	return m_Children.back();
}

template <class _StrType> template <class... Args>
mrt::XML_Node<_StrType>& mrt::XML_Node<_StrType>::EmplaceChild(Args&&... args)
{
	m_Children.emplace_back(std::forward<Args>(args)...);

	return m_Children.back();
}

template <class _StrType>
mrt::XML_Attribute<_StrType>& mrt::XML_Node<_StrType>::GetAttribute(SizeType index)
{
	if (index >= m_Attributes.size())
	{
		throw std::out_of_range("Index out of range.");
	}
	else
	{
		return m_Attributes[index];
	}
}

template <class _StrType>
const mrt::XML_Attribute<_StrType>& mrt::XML_Node<_StrType>::GetAttribute(SizeType index) const
{
	if (index >= m_Attributes.size())
	{
		throw std::out_of_range("Index out of range.");
	}
	else
	{
		return m_Attributes[index];
	}
}

template <class _StrType>
mrt::XML_Node<_StrType>& mrt::XML_Node<_StrType>::GetChild(SizeType index)
{
	if (index >= m_Children.size())
	{
		throw std::out_of_range("Index out of range.");
	}
	else
	{
		return m_Children[index];
	}
}

template <class _StrType>
const mrt::XML_Node<_StrType>& mrt::XML_Node<_StrType>::GetChild(SizeType index) const
{
	if (index >= m_Children.size())
	{
		throw std::out_of_range("Index out of range.");
	}
	else
	{
		return m_Children[index];
	}
}

template <class _StrType>
std::vector<mrt::XML_Attribute<_StrType>>& mrt::XML_Node<_StrType>::GetAllAttributes() { return m_Attributes; }

template <class _StrType>
const std::vector<mrt::XML_Attribute<_StrType>>& mrt::XML_Node<_StrType>::GetAllAttributes() const { return m_Attributes; }

template <class _StrType>
std::vector<mrt::XML_Node<_StrType>>& mrt::XML_Node<_StrType>::GetAllChildren() { return m_Children; }

template <class _StrType>
const std::vector<mrt::XML_Node<_StrType>>& mrt::XML_Node<_StrType>::GetAllChildren() const { return m_Children; }

  /*******************************/
 /* XML_Document Implementation */
/*******************************/

template <class _StrType>
mrt::XML_Document<_StrType>::XML_Document() : m_Version(mrt::string_cast<_StrType>("1.0")) { }

template <class _StrType>
mrt::XML_Document<_StrType>::XML_Document(const _StrType& version) :
	m_Version(version) { }

template <class _StrType>
mrt::XML_Document<_StrType>::XML_Document(const XML_Node<_StrType>& root, const _StrType& version) :
	m_Root(root), m_Version(version) { }

template <class _StrType>
mrt::XML_Document<_StrType>::XML_Document(const XML_Document& other) :
	m_Version(other.m_Version), m_Root(other.m_Root) { }

template <class _StrType>
mrt::XML_Document<_StrType>::XML_Document(XML_Document<_StrType>&& other) noexcept :
	m_Version(std::forward<_StrType>(other.m_Version)),
	m_Root(std::forward<XML_Node<_StrType>>(other.m_Root)) { }

template <class _StrType>
mrt::XML_Document<_StrType>& mrt::XML_Document<_StrType>::operator=(const XML_Document& other)
{
	m_Version = other.m_Version;
	m_Root = other.m_Root;

	return *this;
}

template <class _StrType>
mrt::XML_Node<_StrType>& mrt::XML_Document<_StrType>::GetRoot() { return m_Root; }

template <class _StrType>
const mrt::XML_Node<_StrType>& mrt::XML_Document<_StrType>::GetRoot() const { return m_Root; }

template <class _StrType>
_StrType& mrt::XML_Document<_StrType>::GetVersion() { return m_Version; }

template <class _StrType>
const _StrType& mrt::XML_Document<_StrType>::GetVersion() const { return m_Version; }

template <class _StrType>
mrt::XML_Document_FileError mrt::XML_Document<_StrType>::ReadDocument(const _StrType& path, XML_Document& document)
{
	IFstream fs(std::filesystem::path(path), std::ios::in);

	if (!fs.is_open())
	{
		return XML_Document_FileError::FAILED_TO_OPEN;
	}

	{
		_StrType inputString((std::istreambuf_iterator<ValueType>(fs)), std::istreambuf_iterator<ValueType>());

		inputString.erase(std::remove_if(inputString.begin(), inputString.end(), [](ValueType c) { return c == '\n'; }), inputString.end());

		if (inputString.empty())
		{
			return XML_Document_FileError::FILE_EMPTY;
		}

		SizeType pos = inputString.find(mrt::string_cast<_StrType>("?>"));

		if (pos != _StrType::npos)
		{
			pos += 2;

			inputString = inputString.substr(pos);
		}

		mrtInternal::XML_Tokenizer_Parser<_StrType> parser(&document.GetRoot(), inputString.cbegin(), inputString.cend());

		parser.Parse();
	}

	fs.close();
	return XML_Document_FileError::SUCCESS;
}

template <class _StrType>
mrt::XML_Document_FileError mrt::XML_Document<_StrType>::WriteDocument(const _StrType& path, const XML_Document& document, bool addProlog, bool newLines)
{
	OFstream fs(std::filesystem::path(path), std::ios::out);

	if (!fs.is_open())
	{
		return XML_Document_FileError::FAILED_TO_OPEN;
	}

	if (addProlog)
	{
		fs << getXMLprolog<_StrType>(document.GetVersion()) << getNewLine<_StrType>(newLines);
	}

	fs << getStartNode(document.GetRoot()) << getNewLine<_StrType>(newLines);

	WriteChildrenNodes(&fs, document.GetRoot(), newLines);

	fs << getEndNode(document.GetRoot()) << getNewLine<_StrType>(newLines);

	fs.close();
	return XML_Document_FileError::SUCCESS;
}

template <class _StrType>
void mrt::XML_Document<_StrType>::WriteChildrenNodes(OStream* fs, const XML_Node<_StrType>& node, bool newLines, size_t tabs)
{
	for (const XML_Node<_StrType>& child : node.GetAllChildren())
	{
		if (child.GetChildCount() > 0)
		{
			*fs << (newLines ? getTabs<_StrType>(tabs) : _StrType()) << getStartNode<_StrType>(child) << getNewLine<_StrType>(newLines);

			WriteChildrenNodes(fs, child, newLines, tabs + 1);

			*fs << (newLines ? getTabs<_StrType>(tabs) : _StrType()) << getEndNode<_StrType>(child) << getNewLine<_StrType>(newLines);
		}
		else
		{
			*fs << (newLines ? getTabs<_StrType>(tabs) : _StrType()) << getStartNode<_StrType>(child) << child.GetValue() << getEndNode(child) << getNewLine<_StrType>(newLines);
		}
	}
}

template <class _StrType>
void mrt::XML_Document<_StrType>::ReadDocumentFromStream(IStream* inStream, XML_Document& document)
{
	_StrType inputString((std::istreambuf_iterator<ValueType>(*inStream)), std::istreambuf_iterator<ValueType>());

	inputString.erase(std::remove_if(inputString.begin(), inputString.end(), [](ValueType c) { return c == '\n'; }), inputString.end());

	if (inputString.empty()) return;

	SizeType pos = inputString.find(mrt::string_cast<_StrType>("?>"));

	if (pos != _StrType::npos)
	{
		pos += 2;

		inputString = inputString.substr(pos);
	}

	mrtInternal::XML_Tokenizer_Parser<_StrType> parser(&document.GetRoot(), inputString.cbegin(), inputString.cend());

	parser.Parse();
}

template <class _StrType>
void mrt::XML_Document<_StrType>::WriteDocumentToStream(OStream* fs, const XML_Document& document, bool addProlog, bool newLines)
{
	if (addProlog)
	{
		*fs << getXMLprolog<_StrType>(document.GetVersion()) << getNewLine<_StrType>(newLines);
	}

	*fs << getStartNode(document.GetRoot()) << getNewLine<_StrType>(newLines);

	WriteChildrenNodes(fs, document.GetRoot(), newLines);

	*fs << getEndNode(document.GetRoot()) << getNewLine<_StrType>(newLines);
}

  /***************************************/
 /* MrT Global Functions Implementation */
/***************************************/

template <>
std::string mrt::getXMLprolog(const std::string& version)
{
	return std::format("<?xml version=\"{}\" encoding=\"UTF-8\"?>", version);
}

template <>
std::wstring mrt::getXMLprolog(const std::wstring& version)
{
	return std::format(L"<?xml version=\"{}\" encoding=\"UTF-8\"?>", version);
}

template <>
std::u8string mrt::getXMLprolog(const std::u8string& version)
{
	return u8"<?xml version=\"" + version + u8"\" encoding=\"UTF-8\"?>";
}

template <class _StrType>
_StrType mrt::getStartNode(const XML_Node<_StrType>& node)
{
	typename XML_Document<_StrType>::OStrStream ss;

	ss << "<" << node.GetName();

	for (const XML_Attribute<_StrType>& attribute : node.GetAllAttributes())
	{
		ss << " " << attribute.m_Name << "=\"" << attribute.m_Value << "\"";
	}

	ss << ">";

	return ss.str();
}

template <>
std::string mrt::getEndNode(const XML_Node<std::string>& node)
{
	return std::format("</{}>", node.GetName());
}

template <>
std::wstring mrt::getEndNode(const XML_Node<std::wstring>& node)
{
	return std::format(L"</{}>", node.GetName());
}

template <>
std::u8string mrt::getEndNode(const XML_Node<std::u8string>& node)
{
	return u8"</" + node.GetName() + u8">";
}

template <class _StrType>
_StrType mrt::getTabs(size_t amount)
{
	return _StrType(amount, '\t');
}

template <>
std::string mrt::getNewLine(bool newLines)
{
	return newLines ? "\n" : "";
}

template <>
std::wstring mrt::getNewLine(bool newLines)
{
	return newLines ? L"\n" : L"";
}

template <>
std::u8string mrt::getNewLine(bool newLines)
{
	return newLines ? u8"\n" : u8"";
}

template <class _CastType, class _InType>
_CastType mrt::string_cast(const _InType& str)
{
	typename std::basic_ostringstream<typename _CastType::value_type> ss;
	ss << str;
	return ss.str();
}

  /********************************/
 /* XML_Tokenizer Implementation */
/********************************/

template <class _StrType>
mrt::mrtInternal::XML_Tokenizer<_StrType>::XML_Tokenizer() : m_InputData(), m_Index(0) { }

template <class _StrType>
mrt::mrtInternal::XML_Tokenizer<_StrType>::XML_Tokenizer(IStream* inputStream)
	: m_InputData((std::istreambuf_iterator<ValueType>(*inputStream)), std::istreambuf_iterator<ValueType>()), m_Index(0) { }

template <class _StrType>
mrt::mrtInternal::XML_Tokenizer<_StrType>::XML_Tokenizer(const _StrType& inputData) : m_InputData(inputData), m_Index(0) { }

template <class _StrType>
mrt::mrtInternal::XML_Tokenizer<_StrType>::XML_Tokenizer(_StrType::const_iterator begin, _StrType::const_iterator end) : m_InputData(begin, end), m_Index(0) { }

template <class _StrType>
void mrt::mrtInternal::XML_Tokenizer<_StrType>::SetInputData(const _StrType& inputData) { m_InputData = inputData; m_Index = 0; }

template <class _StrType>
mrt::mrtInternal::XML_Token<_StrType> mrt::mrtInternal::XML_Tokenizer<_StrType>::Next()
{
	if (m_Index >= m_InputData.size())
	{
		return { XML_Token_Type::END_OF_STRING, _StrType() };
	}

	if (m_InputData[m_Index] == '<')
	{
		if (m_Index + 1 < m_InputData.size() && m_InputData[m_Index + 1] == '/')
		{
			_StrType data;

			while (m_Index < m_InputData.size() && m_InputData[m_Index] != '>')
			{
				data += m_InputData[m_Index];
				m_Index++;
			}

			data += m_InputData[m_Index];
			m_Index++;

			return { XML_Token_Type::TAG_END, data };
		}
		else
		{
			_StrType data;

			while (m_Index < m_InputData.size() && m_InputData[m_Index] != '>')
			{
				data += m_InputData[m_Index];
				m_Index++;
			}

			data += m_InputData[m_Index];

			if (data[data.size() - 1] == '>' && data[data.size() - 2] == '/')
			{
				m_Index++;
				return { XML_Token_Type::TAG_SELF_END, data };
			}
			else
			{
				m_Index++;
				return { XML_Token_Type::TAG_START, data };
			}
		}
	}
	else
	{
		_StrType data;

		while (m_Index < m_InputData.size() && m_InputData[m_Index] != '<')
		{
			data += m_InputData[m_Index];
			m_Index++;
		}

		return { XML_Token_Type::VALUE, data };
	}
}

template <class _StrType>
bool mrt::mrtInternal::XML_Tokenizer<_StrType>::HasNext() const { return (m_Index < m_InputData.size()) ? true : false; }

  /************************************/
 /* XML_Tokens_Parser Implementation */
/************************************/

template <class _StrType>
mrt::mrtInternal::XML_Tokenizer_Parser<_StrType>::XML_Tokenizer_Parser(XML_Node<_StrType>* const root, IStream* inputStream)
	: m_Root(root), m_InputData((std::istreambuf_iterator<ValueType>(*inputStream)), std::istreambuf_iterator<ValueType>()) { }

template <class _StrType>
mrt::mrtInternal::XML_Tokenizer_Parser<_StrType>::XML_Tokenizer_Parser(XML_Node<_StrType>* const root, const _StrType& inputString)
	: m_Root(root), m_InputData(inputString) { }

template <class _StrType>
mrt::mrtInternal::XML_Tokenizer_Parser<_StrType>::XML_Tokenizer_Parser(XML_Node<_StrType>* const root, _StrType::const_iterator begin, _StrType::const_iterator end)
	: m_Root(root), m_InputData(begin, end) { }

template <class _StrType>
void mrt::mrtInternal::XML_Tokenizer_Parser<_StrType>::Parse()
{
	XML_Tokenizer<_StrType> tokenizer(m_InputData.cbegin(), m_InputData.cend());

	*m_Root = ReadNode(tokenizer.Next().m_Data);

	m_CurrentEndTags.push_back(getEndNode<_StrType>(*m_Root));

	m_CurrentNodes.push_back(m_Root);

	while (tokenizer.HasNext())
	{
		XML_Token<_StrType> token = tokenizer.Next();

		switch (token.m_Type)
		{
		case XML_Token_Type::TAG_START:
		{
			m_CurrentNodes.push_back(&m_CurrentNodes.back()->AddChild(ReadNode(token.m_Data)));

			m_CurrentEndTags.push_back(getEndNode<_StrType>(*m_CurrentNodes.back()));

			break;
		}
		case XML_Token_Type::TAG_END:
		{
			if (token.m_Data == m_CurrentEndTags.back())
			{
				m_CurrentEndTags.pop_back();
				m_CurrentNodes.pop_back();
			}
			else
			{
				throw std::runtime_error("Invalid XML file (Missing an end tag).");
			}

			break;
		}
		case XML_Token_Type::TAG_SELF_END:
		{
			m_CurrentNodes.back()->AddChild(ReadNode(token.m_Data));

			break;
		}
		case XML_Token_Type::VALUE:
		{
			m_CurrentNodes.back()->SetValue(token.m_Data);

			break;
		}
		}
	}
}

template <class _StrType>
mrt::XML_Node<_StrType> mrt::mrtInternal::XML_Tokenizer_Parser<_StrType>::ReadNode(const _StrType& line)   //Reads the start node and returns a node with the name, attributes and value.
{
	XML_Node<_StrType> node;

	if (line.find(mrt::string_cast<_StrType>("/>")) != _StrType::npos)
	{
		typename _StrType::size_type pos = line.find(' ');

		if (pos != _StrType::npos)
		{
			node.SetName(line.substr(1, pos - 1));

			ReadAttributes(line.substr(pos + 1, line.find('>') - (pos + 1) - 1), &node);
		}
		else
		{
			node.SetName(line.substr(1, line.size() - 3));
		}
	}
	else
	{
		typename _StrType::size_type pos = line.find(' ');

		if (pos != _StrType::npos)
		{
			node.SetName(line.substr(1, pos - 1));

			ReadAttributes(line.substr(pos + 1, line.size() - (pos + 2)), &node);
		}
		else
		{
			node.SetName(line.substr(1, line.size() - 2));
		}
	}

	return node;
}

template <class _StrType>
void mrt::mrtInternal::XML_Tokenizer_Parser<_StrType>::ReadAttributes(const _StrType& line, XML_Node<_StrType>* const node)
{
	_StrType name, value, attrData;

	typename std::basic_istringstream<typename _StrType::value_type> is(line);

	while (is >> attrData)
	{
		if (attrData.find('=') != _StrType::npos)
		{
			name = attrData.substr(0, attrData.find('='));
			value = attrData.substr(attrData.find('=') + 2, attrData.size() - (attrData.find('=') + 2) - 1);

			node->EmplaceAttribute(name, value);
		}
	}
}