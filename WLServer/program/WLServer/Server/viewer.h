#pragma once

#include "extern.h"													//extern functions & vars


namespace Interface {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;


	//char definitions
	

	/// <summary>
	/// Resumen de viewer
	/// </summary>
	public ref class viewer : public System::Windows::Forms::Form
	{

	public:
		static void Iniciar(void)
		{

			ServerStart();

		}



	public:
		viewer(void)
		{			
			InitVars();											//inicializa las variables del sistema 
			InitLog();											//init log file
			Log("Sys: Program Start");								//write a msg log
			InitConfig();											//inicializa los valores de configuracion 
			InitDatabase();										//carga los valores desde la base de datos
			InitializeComponent();
			Iniciar();
		}

	public:
		static void View(RichTextBox^ sender,String^ str)
		{
			sender->AppendText(str);
		}

	protected:
		/// <summary>
		/// Limpiar los recursos que se estén utilizando.
		/// </summary>
		~viewer()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  button1;
	protected: 
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::RichTextBox^  ViewScales;
	private: System::Windows::Forms::RichTextBox^  ViewTickets;
	private: System::Windows::Forms::RichTextBox^  ViewLog;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::Timer^  timer2;

	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Variable del diseñador requerida.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Método necesario para admitir el Diseñador. No se puede modificar
		/// el contenido del método con el editor de código.
		/// </summary>
		void InitializeComponent(void)
		{
			char msg[TYPE_CHAR256];
			String^ str;

			sprintf(msg,"%s @ %s\n",App.Name,Config.IP);
			str = gcnew String(msg);									
			
			this->components = (gcnew System::ComponentModel::Container());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->ViewScales = (gcnew System::Windows::Forms::RichTextBox());
			this->ViewTickets = (gcnew System::Windows::Forms::RichTextBox());
			this->ViewLog = (gcnew System::Windows::Forms::RichTextBox());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->timer2 = (gcnew System::Windows::Forms::Timer(this->components));
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->button1->Location = System::Drawing::Point(680, 24);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(102, 33);
			this->button1->TabIndex = 0;
			this->button1->Text = L"&Detener/Stop";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &viewer::button1_Click);
			// 
			// button2
			// 
			this->button2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->button2->Location = System::Drawing::Point(680, 65);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(102, 33);
			this->button2->TabIndex = 1;
			this->button2->Text = L"&Salir/Exit";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &viewer::button2_Click);
			// 
			// button3
			// 
			this->button3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->button3->Location = System::Drawing::Point(680, 499);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(102, 33);
			this->button3->TabIndex = 2;
			this->button3->Text = L"&Borrar/Clear";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &viewer::button3_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(16, 9);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(58, 13);
			this->label1->TabIndex = 3;
			this->label1->Text = L"Basculas/Scales";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label2->Location = System::Drawing::Point(277, 9);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(46, 13);
			this->label2->TabIndex = 4;
			this->label2->Text = L"Ventas/Sales";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label3->Location = System::Drawing::Point(12, 483);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(28, 13);
			this->label3->TabIndex = 5;
			this->label3->Text = L"Log";
			// 
			// ViewScales
			// 
			this->ViewScales->BackColor = System::Drawing::Color::Black;
			this->ViewScales->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->ViewScales->ForeColor = System::Drawing::Color::White;
			this->ViewScales->Location = System::Drawing::Point(16, 25);
			this->ViewScales->Name = L"ViewScales";
			this->ViewScales->Size = System::Drawing::Size(258, 453);
			this->ViewScales->TabIndex = 6;
			this->ViewScales->Text = L"";
			// 
			// ViewTickets
			// 
			this->ViewTickets->BackColor = System::Drawing::Color::Black;
			this->ViewTickets->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->ViewTickets->ForeColor = System::Drawing::Color::White;
			this->ViewTickets->Location = System::Drawing::Point(280, 25);
			this->ViewTickets->Name = L"ViewTickets";
			this->ViewTickets->Size = System::Drawing::Size(390, 453);
			this->ViewTickets->TabIndex = 7;
			this->ViewTickets->Text = L"";
			// 
			// ViewLog
			// 
			this->ViewLog->BackColor = System::Drawing::Color::Black;
			this->ViewLog->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->ViewLog->ForeColor = System::Drawing::Color::White;
			this->ViewLog->Location = System::Drawing::Point(16, 499);
			this->ViewLog->Name = L"ViewLog";
			this->ViewLog->Size = System::Drawing::Size(663, 67);
			this->ViewLog->TabIndex = 8;
			this->ViewLog->Text = L"";
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Interval = 1000;
			this->timer1->Tick += gcnew System::EventHandler(this, &viewer::timer1_Tick);
			// 
			// timer2
			// 
			this->timer2->Enabled = true;
			this->timer2->Interval = 500;
			this->timer2->Tick += gcnew System::EventHandler(this, &viewer::timer2_Tick);
			// 
			// viewer
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(794, 484);
			this->Controls->Add(this->ViewLog);
			this->Controls->Add(this->ViewTickets);
			this->Controls->Add(this->ViewScales);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
			this->MaximumSize = System::Drawing::Size(800, 600);
			this->Name = L"viewer";
			this->Text = str; //L"WLS Server" ;
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion


	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
		
		if(button1->Text=="&Iniciar/Start")
		{
			if (ServerStartStop(1)==1)
			{
				button1->Text ="&Detener/Stop";
				timer1->Enabled = true;
			}
		}
		else 
		{
			if (ServerStartStop(0)==1)
			{
				button1->Text ="&Iniciar/Start";	
				timer1->Enabled = false;
			}
		}
	}


	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {

		ApplicationExit();
			
	}


	private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {

		ViewLog->Clear();
	}


	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
		
		int c;
		char msg[TYPE_CHAR256];
		String^ str;


		//refresh scales
		if(Config.RefresScales)	
		{
			ViewScales->Clear();
			c = 0;
			while(Scales[c].Id && c<MAX_SCALES)
			{			
				sprintf(msg,"%s :%s %02i \n",Scales[c].Time,Scales[c].Serie,Scales[c].Agent);
				str = gcnew String(msg);
				View(ViewScales,str);
				c++;
			}
			Config.RefresScales = 0;
		}

		//refresh ticket
		if(Config.RefresTickets)
		{			
			ViewTickets->Clear();
			c = 0;
			while(Detail[c].Id && c<MAX_DETAIL)
			{			
				sprintf(msg,"%s %02i %02i %s\n",Detail[c].Time,Detail[c].Id,Detail[c].Line,Detail[c].Content);
				str = gcnew String(msg);									
				View(ViewTickets,str);
				c++;
			}
			Config.RefresTickets = 0;
		}
	}


	private: System::Void timer2_Tick(System::Object^  sender, System::EventArgs^  e) {
		
		if(strlen(Msg))
		{
			if((ViewLog->Text->Length + ((int) strlen(Msg))) > ViewLog->MaxLength)ViewLog->Clear();
			String^ str = gcnew String(Msg);
			View(ViewLog,str);
			memset(Msg,0,SQL_MAX_DATA_LEN);
		}
	}

};
}
