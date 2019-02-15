#include "AppConfig.h"



namespace app {

	AppConfig::AppConfig(void) : _configuration_modified(0)
	{
	}

	bool AppConfig::Load(System::String^ filename) {
		_xml_path = filename;
		XmlReader^ reader;
		FileStream^ stream;
		try {
			stream = gcnew FileStream(_xml_path, FileMode::Open);
			reader = XmlReader::Create(stream);
		}
		catch (System::IO::FileNotFoundException^) {
			return false;
		}
		catch (Exception^) {
			return false;
		}

		while (reader->Read()) {
			switch (reader->NodeType) {
			case XmlNodeType::Element:
				if (0 == reader->Name->CompareTo(XML_TAG_SECTION_CONFIG)) {
					// parse "devices" section
					ParseSectionConfiguration(reader);
				}
				
				else {}
				break;
			}
		}
		stream->Close();
		return true;
	}

	void AppConfig::WriteXML() {
		XmlDocument^ xml_doc = gcnew XmlDocument();
		xml_doc->Load(_xml_path);
		XmlNode^ root = xml_doc->DocumentElement;
		XmlNode^ cur_node;
		String^ temp_str;
		
		if (_configuration_modified >> (Byte)xml_pos::IP_POS & 1) {
			cur_node = root->SelectSingleNode(XML_NODE_KEY + XML_TAG_IP);
			WriteNodeValue(_cfgDv->_IP, temp_str);
			cur_node->InnerText = temp_str;
		}
		if (_configuration_modified >> (Byte)xml_pos::PORT_POS & 1) {
			cur_node = root->SelectSingleNode(XML_NODE_KEY + XML_TAG_PORT);
			WriteNodeValue(_cfgDv->_port, temp_str);
			cur_node->InnerText = temp_str;
		}
		
		xml_doc->Save(_xml_path);
	}

	bool AppConfig::ParseSectionConfiguration(XmlReader^ reader) {
		bool done = false;
		_cfgDv = gcnew ConfigDevice();
		while (!done && reader->Read()) {
			switch (reader->NodeType) {
			case XmlNodeType::Element:
				if (0 == reader->Name->CompareTo(XML_TAG_IP)) {
					if (reader->Read()) {
						GetNodeValue(reader->Value, _cfgDv->_IP);
					}

				}
				else if (0 == reader->Name->CompareTo(XML_TAG_PORT)) {
					if (reader->Read())
						GetNodeValue(reader->Value, _cfgDv->_port);
				}
				
				else {}
				break;
			case XmlNodeType::EndElement:
				// end of "devices" section
				if (0 == reader->Name->CompareTo(XML_TAG_SECTION_CONFIG)) {
					done = true;
				}
				break;
			default:
				break;
			}
		}
		return true;
	}

	

	void AppConfig::GetNodeValue(String^ strVal, bool% status) {
		if (strVal->Contains("True")) {
			status = true;
		}
		else {
			status = false;
		}
	}



	void AppConfig::GetNodeValue(String^ strVal, double% val) {
		val = 0.0;
		if (strVal->Length > 0) {
			val = Double::Parse(strVal);
		}
	}
	void AppConfig::GetNodeValue(String^ strVal, float% val) {
		val = 0.0;
		if (strVal->Length > 0) {
			val = Single::Parse(strVal);
		}
	}
	void AppConfig::GetNodeValue(String^ strVal, int% val) {
		val = 0;
		if (strVal->Length > 0) {
			val = Int32::Parse(strVal);
		}
	}

	void AppConfig::GetNodeValue(String ^ strVal, array<String^>^% IPval)
	{
		if (strVal->Length > 0) {
			_cfgDv->_IP_str = strVal;
			String^ strDelimiters = ".";
			IPval = strVal->Split(strDelimiters->ToCharArray());
			_cfgDv->_host = _cfgDv->context.marshal_as<const char*>(_cfgDv->_IP_str);
		}
	}

	void AppConfig::GetNodeValue(String^ strVal, Point2D^% pt) {
		String^ strDelimiters = " ,\t";
		//array<wchar_t>^ delimiters = strDelimiters->ToCharArray();
		array<String^>^ subStrs;
		subStrs = strVal->Split(strDelimiters->ToCharArray());
		if (subStrs->Length < 2) {
			return;
		}
		if (nullptr == pt) {
			pt = gcnew Point2D();
		}

		pt->x = Double::Parse(subStrs[0]);
		pt->y = Double::Parse(subStrs[1]);
	}

	void AppConfig::GetNodeValue(String^ strVal, Point3D^% pt) {
		String^ strDelimiters = " ,\t";
		//array<wchar_t>^ delimiters = strDelimiters->ToCharArray();
		array<String^>^ subStrs;
		subStrs = strVal->Split(strDelimiters->ToCharArray());
		if (subStrs->Length < 3) {
			return;
		}
		if (nullptr == pt) {
			pt = gcnew Point3D();
		}

		pt->x = Double::Parse(subStrs[0]);
		pt->y = Double::Parse(subStrs[1]);
		pt->z = Double::Parse(subStrs[2]);
	}

	void AppConfig::WriteNodeValue(array<String^>^ IPval, String ^% strVal)
	{
		strVal = "";
		strVal = IPval[0] + "." + IPval[1] + "." + IPval[2] + "." + IPval[3];
	}


}
