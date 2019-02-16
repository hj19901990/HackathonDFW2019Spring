#pragma once
#include "app\AppConfig.h"
#include "ContentRendering.h"
#include "GIFImg.h"
namespace face_detection {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			

		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::Windows::Forms::PictureBox^  pictureBox2;
	private: System::Windows::Forms::RichTextBox^  richTextBox1;
	private: System::Windows::Forms::ToolStrip^  toolStrip1;
	private: System::Windows::Forms::ToolStripButton^  toolStripStart;
	private: System::Windows::Forms::ToolStripButton^  toolStripStop;

	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MainForm::typeid));
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->toolStrip1 = (gcnew System::Windows::Forms::ToolStrip());
			this->toolStripStart = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripStop = (gcnew System::Windows::Forms::ToolStripButton());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->richTextBox1 = (gcnew System::Windows::Forms::RichTextBox());
			this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer1))->BeginInit();
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->toolStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->BeginInit();
			this->SuspendLayout();
			// 
			// splitContainer1
			// 
			this->splitContainer1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->Location = System::Drawing::Point(0, 0);
			this->splitContainer1->Margin = System::Windows::Forms::Padding(2);
			this->splitContainer1->Name = L"splitContainer1";
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->Controls->Add(this->toolStrip1);
			this->splitContainer1->Panel1->Controls->Add(this->pictureBox1);
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->Controls->Add(this->richTextBox1);
			this->splitContainer1->Panel2->Controls->Add(this->pictureBox2);
			this->splitContainer1->Size = System::Drawing::Size(1300, 600);
			this->splitContainer1->SplitterDistance = 640;
			this->splitContainer1->SplitterWidth = 3;
			this->splitContainer1->TabIndex = 0;
			// 
			// toolStrip1
			// 
			this->toolStrip1->ImageScalingSize = System::Drawing::Size(48, 48);
			this->toolStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) { this->toolStripStart, this->toolStripStop });
			this->toolStrip1->Location = System::Drawing::Point(0, 0);
			this->toolStrip1->Name = L"toolStrip1";
			this->toolStrip1->Size = System::Drawing::Size(609, 55);
			this->toolStrip1->TabIndex = 1;
			this->toolStrip1->Text = L"toolStrip1";
			// 
			// toolStripStart
			// 
			this->toolStripStart->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripStart->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolStripStart.Image")));
			this->toolStripStart->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripStart->Name = L"toolStripStart";
			this->toolStripStart->Size = System::Drawing::Size(52, 52);
			this->toolStripStart->Text = L"toolStripStart";
			this->toolStripStart->Click += gcnew System::EventHandler(this, &MainForm::toolStripStart_Click);
			// 
			// toolStripStop
			// 
			this->toolStripStop->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripStop->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolStripStop.Image")));
			this->toolStripStop->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripStop->Name = L"toolStripStop";
			this->toolStripStop->Size = System::Drawing::Size(52, 52);
			this->toolStripStop->Text = L"toolStripStop";
			this->toolStripStop->Click += gcnew System::EventHandler(this, &MainForm::toolStripStop_Click);
			// 
			// pictureBox1
			// 
			this->pictureBox1->BackColor = System::Drawing::SystemColors::Window;
			this->pictureBox1->Location = System::Drawing::Point(6, 67);
			this->pictureBox1->Margin = System::Windows::Forms::Padding(2);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(640, 480);
			this->pictureBox1->TabIndex = 0;
			this->pictureBox1->TabStop = false;
			this->pictureBox1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MainForm::pictureBox1_Paint);
			// 
			// richTextBox1
			// 
			this->richTextBox1->Location = System::Drawing::Point(10, 182);
			this->richTextBox1->Margin = System::Windows::Forms::Padding(2);
			this->richTextBox1->Name = L"richTextBox1";
			this->richTextBox1->Size = System::Drawing::Size(640, 400);
			this->richTextBox1->TabIndex = 2;
			this->richTextBox1->Text = L"";
			// 
			// pictureBox2
			// 
			this->pictureBox2->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox2.BackgroundImage")));
			this->pictureBox2->InitialImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox2.InitialImage")));
			this->pictureBox2->Location = System::Drawing::Point(10, 11);
			this->pictureBox2->Margin = System::Windows::Forms::Padding(2);
			this->pictureBox2->Name = L"pictureBox2";
			this->pictureBox2->Size = System::Drawing::Size(160, 160);
			this->pictureBox2->TabIndex = 0;
			this->pictureBox2->TabStop = false;
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1300, 600);
			this->Controls->Add(this->splitContainer1);
			this->Margin = System::Windows::Forms::Padding(2);
			this->Name = L"MainForm";
			this->Text = L"Face Recogination";
			//this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
			this->Load += gcnew System::EventHandler(this, &MainForm::MainForm_Load);
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel1->PerformLayout();
			this->splitContainer1->Panel2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer1))->EndInit();
			this->splitContainer1->ResumeLayout(false);
			this->toolStrip1->ResumeLayout(false);
			this->toolStrip1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
		public: app::AppConfig^		_appConfig;
		private: ContentRendering*	_contents;
		private: GIFImg^			_animatedGif;
	private: System::Void MainForm_Load(System::Object^  sender, System::EventArgs^  e) {
		this->MaximizeBox = false;
		this->MinimizeBox = false;
		_appConfig = gcnew app::AppConfig();
		_appConfig->Load("Config\\HACKDFW_cfg.xml");
		_animatedGif = gcnew GIFImg("Config//facescan.gif");
		_contents = new ContentRendering((HWND)this->Handle.ToPointer(), pictureBox1, _appConfig);

		//test
		richTextBox1->Multiline = true;
		richTextBox1->ForeColor = Color::Blue;
		richTextBox1->Font = gcnew System::Drawing::Font("Georgia", 16);
		richTextBox1->Clear();
		richTextBox1->Text += "Name:" + Environment::NewLine;
		richTextBox1->Text += "He looks upset!" + Environment::NewLine;
	}
private: System::Void toolStripStart_Click(System::Object^  sender, System::EventArgs^  e) {
	//_contents->_program_start = true;
	_contents->CaptureImage();
}
private: System::Void toolStripStop_Click(System::Object^  sender, System::EventArgs^  e) {
	_contents->_program_start = false;
}
private: System::Void pictureBox1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
	if (!_contents->_program_start) {
		//pictureBox1->Image = _animatedGif->GetNextFrame();
		//Sleep(20);
	}
	else {
		_contents->DrawBox(e);		
	}
}
};
}
