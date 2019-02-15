#pragma once
#include <Windows.h>
#include <vector>
#include <msclr/marshal_cppstd.h>
namespace app {

	using namespace System;
	using namespace System::IO;
	using namespace System::Xml;
	using namespace System::Text;
	using namespace System::Collections::Generic;			// container support
	enum class xml_pos
	{
		IP_POS, PORT_POS
	};
	public ref class Point2D {
	public: Point2D() : x(0), y(0) {};
	public: Point2D(int x1, int y1)
		: x(x1)
		, y(y1)
	{};
	public: double x;
			double y;
	};

	public ref class Point3D {
	public: Point3D() : x(0), y(0), z(0) {};
	public: Point3D(int x1, int y1, int z1)
		: x(x1)
		, y(y1)
		, z(z1)
	{};
	public: double x;
			double y;
			double z;
	};


	public ref class ConfigDevice
	{
	public: ConfigDevice()
		: _port(5005)
	{


	};
	public:
		msclr::interop::marshal_context context;
		String^ _IP_str;
		const char* _host;
	public: array<String^>^ _IP;
	public: int _port;	// Port number
	
	};

	public ref class AppConfig
	{
	public:
		AppConfig(void);
	public:
		bool Load(System::String^ filename);
		void WriteXML();
	protected:
		
		bool ParseSectionConfiguration(XmlReader^ reader);
		
		void GetNodeValue(String^ strVal, array<String^>^% IPval);
		void GetNodeValue(String^ strVal, Point2D^% pt);
		void GetNodeValue(String^ strVal, Point3D^% pt);
		void GetNodeValue(String^ strVal, bool% status);
		void GetNodeValue(String^ strVal, double% val);
		void GetNodeValue(String^ strVal, float% val);
		void GetNodeValue(String^ strVal, int% val);

		void WriteNodeValue(array<String^>^ IPval, String ^% strVal);
		template <class T>
		void WriteNodeValue(T val, String^% strVal) {
			strVal = "";
			strVal = val.ToString();
		};


	protected:
		static String^ XML_NODE_KEY = gcnew String("descendant::");

		static String^ XML_TAG_ROOT = gcnew String("HACKDFW");
		static String^ XML_TAG_SECTION_CONFIG = gcnew String("configuration");
		
				
		static String^ XML_TAG_IP = gcnew String("IP");
		static String^ XML_TAG_PORT = gcnew String("port");
	
	private:
		String^ _xml_path;

	public: unsigned int _configuration_modified;
	public: ConfigDevice^ _cfgDv;


	};

}

