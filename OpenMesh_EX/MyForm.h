#pragma once

#include "DotNetUtilities.h"
#include "Mesh/GUA_OM.h"
#include "Mesh/DP.h"

Tri_Mesh *mesh;

xform xf;
GLCamera camera;
float fov = 0.7f;

static const Mouse::button physical_to_logical_map[] = {
	Mouse::NONE, Mouse::ROTATE, Mouse::MOVEXY, Mouse::MOVEZ,
	Mouse::MOVEZ, Mouse::MOVEXY, Mouse::MOVEXY, Mouse::MOVEXY,
};
Mouse::button Mouse_State = Mouse::ROTATE;

namespace OpenMesh_EX {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// MyForm 的摘要
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO:  在此加入建構函式程式碼
			//
		}

	protected:
		/// <summary>
		/// 清除任何使用中的資源。
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  loadModelToolStripMenuItem;
	private: System::Windows::Forms::OpenFileDialog^  openModelDialog;
	private: System::Windows::Forms::SaveFileDialog^  saveModelDialog;
	private: System::Windows::Forms::ToolStripMenuItem^  saveModelToolStripMenuItem;
	private: HKOGLPanel::HKOGLPanelControl^  hkoglPanelControl1;
	private: System::Windows::Forms::Panel^  panel1;
	private: HKOGLPanel::HKOGLPanelControl^  hkoglPanelControl2;
	private: System::Windows::Forms::ToolStripMenuItem^  boundaryTypeToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  circleToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  squareToolStripMenuItem;
	protected:

	private:
		/// <summary>
		/// 設計工具所需的變數。
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改
		/// 這個方法的內容。
		/// </summary>
		void InitializeComponent(void)
		{
			HKOGLPanel::HKCOGLPanelCameraSetting^  hkcoglPanelCameraSetting1 = (gcnew HKOGLPanel::HKCOGLPanelCameraSetting());
			HKOGLPanel::HKCOGLPanelPixelFormat^  hkcoglPanelPixelFormat1 = (gcnew HKOGLPanel::HKCOGLPanelPixelFormat());
			HKOGLPanel::HKCOGLPanelCameraSetting^  hkcoglPanelCameraSetting2 = (gcnew HKOGLPanel::HKCOGLPanelCameraSetting());
			HKOGLPanel::HKCOGLPanelPixelFormat^  hkcoglPanelPixelFormat2 = (gcnew HKOGLPanel::HKCOGLPanelPixelFormat());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->loadModelToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveModelToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->boundaryTypeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->circleToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->squareToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openModelDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->saveModelDialog = (gcnew System::Windows::Forms::SaveFileDialog());
			this->hkoglPanelControl1 = (gcnew HKOGLPanel::HKOGLPanelControl());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->hkoglPanelControl2 = (gcnew HKOGLPanel::HKOGLPanelControl());
			this->menuStrip1->SuspendLayout();
			this->panel1->SuspendLayout();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->ImageScalingSize = System::Drawing::Size(20, 20);
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->fileToolStripMenuItem,
					this->boundaryTypeToolStripMenuItem
			});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Padding = System::Windows::Forms::Padding(8, 2, 0, 2);
			this->menuStrip1->Size = System::Drawing::Size(817, 28);
			this->menuStrip1->TabIndex = 1;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->loadModelToolStripMenuItem,
					this->saveModelToolStripMenuItem
			});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(45, 24);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// loadModelToolStripMenuItem
			// 
			this->loadModelToolStripMenuItem->Name = L"loadModelToolStripMenuItem";
			this->loadModelToolStripMenuItem->Size = System::Drawing::Size(168, 26);
			this->loadModelToolStripMenuItem->Text = L"Load Model";
			this->loadModelToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::loadModelToolStripMenuItem_Click);
			// 
			// saveModelToolStripMenuItem
			// 
			this->saveModelToolStripMenuItem->Name = L"saveModelToolStripMenuItem";
			this->saveModelToolStripMenuItem->Size = System::Drawing::Size(168, 26);
			this->saveModelToolStripMenuItem->Text = L"Save Model";
			this->saveModelToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::saveModelToolStripMenuItem_Click);
			// 
			// boundaryTypeToolStripMenuItem
			// 
			this->boundaryTypeToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->circleToolStripMenuItem,
					this->squareToolStripMenuItem
			});
			this->boundaryTypeToolStripMenuItem->Name = L"boundaryTypeToolStripMenuItem";
			this->boundaryTypeToolStripMenuItem->Size = System::Drawing::Size(122, 24);
			this->boundaryTypeToolStripMenuItem->Text = L"BoundaryType";
			// 
			// circleToolStripMenuItem
			// 
			this->circleToolStripMenuItem->Name = L"circleToolStripMenuItem";
			this->circleToolStripMenuItem->Size = System::Drawing::Size(181, 26);
			this->circleToolStripMenuItem->Text = L"Circle";
			this->circleToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::circleToolStripMenuItem_Click);
			// 
			// squareToolStripMenuItem
			// 
			this->squareToolStripMenuItem->Name = L"squareToolStripMenuItem";
			this->squareToolStripMenuItem->Size = System::Drawing::Size(181, 26);
			this->squareToolStripMenuItem->Text = L"Square";
			this->squareToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::squareToolStripMenuItem_Click);
			// 
			// openModelDialog
			// 
			this->openModelDialog->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &MyForm::openModelDialog_FileOk);
			// 
			// saveModelDialog
			// 
			this->saveModelDialog->DefaultExt = L"obj";
			this->saveModelDialog->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &MyForm::saveModelDialog_FileOk);
			// 
			// hkoglPanelControl1
			// 
			hkcoglPanelCameraSetting1->Far = 1000;
			hkcoglPanelCameraSetting1->Fov = 45;
			hkcoglPanelCameraSetting1->Near = -1000;
			hkcoglPanelCameraSetting1->Type = HKOGLPanel::HKCOGLPanelCameraSetting::CAMERATYPE::ORTHOGRAPHIC;
			this->hkoglPanelControl1->Camera_Setting = hkcoglPanelCameraSetting1;
			this->hkoglPanelControl1->Dock = System::Windows::Forms::DockStyle::Left;
			this->hkoglPanelControl1->Location = System::Drawing::Point(0, 28);
			this->hkoglPanelControl1->Margin = System::Windows::Forms::Padding(4);
			this->hkoglPanelControl1->Name = L"hkoglPanelControl1";
			hkcoglPanelPixelFormat1->Accumu_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			hkcoglPanelPixelFormat1->Alpha_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			hkcoglPanelPixelFormat1->Stencil_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			this->hkoglPanelControl1->Pixel_Format = hkcoglPanelPixelFormat1;
			this->hkoglPanelControl1->Size = System::Drawing::Size(559, 540);
			this->hkoglPanelControl1->TabIndex = 2;
			this->hkoglPanelControl1->Load += gcnew System::EventHandler(this, &MyForm::hkoglPanelControl1_Load);
			this->hkoglPanelControl1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::hkoglPanelControl1_Paint);
			this->hkoglPanelControl1->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::hkoglPanelControl1_KeyDown);
			this->hkoglPanelControl1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::hkoglPanelControl1_MouseDown);
			this->hkoglPanelControl1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::hkoglPanelControl1_MouseMove);
			this->hkoglPanelControl1->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::hkoglPanelControl1_MouseWheel);
			// 
			// panel1
			// 
			this->panel1->Controls->Add(this->hkoglPanelControl2);
			this->panel1->Dock = System::Windows::Forms::DockStyle::Right;
			this->panel1->Location = System::Drawing::Point(567, 28);
			this->panel1->Margin = System::Windows::Forms::Padding(4);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(250, 540);
			this->panel1->TabIndex = 3;
			// 
			// hkoglPanelControl2
			// 
			hkcoglPanelCameraSetting2->Far = 1000;
			hkcoglPanelCameraSetting2->Fov = 45;
			hkcoglPanelCameraSetting2->Near = -1000;
			hkcoglPanelCameraSetting2->Type = HKOGLPanel::HKCOGLPanelCameraSetting::CAMERATYPE::ORTHOGRAPHIC;
			this->hkoglPanelControl2->Camera_Setting = hkcoglPanelCameraSetting2;
			this->hkoglPanelControl2->Dock = System::Windows::Forms::DockStyle::Bottom;
			this->hkoglPanelControl2->Location = System::Drawing::Point(0, 290);
			this->hkoglPanelControl2->Margin = System::Windows::Forms::Padding(4);
			this->hkoglPanelControl2->Name = L"hkoglPanelControl2";
			hkcoglPanelPixelFormat2->Accumu_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			hkcoglPanelPixelFormat2->Alpha_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			hkcoglPanelPixelFormat2->Stencil_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			this->hkoglPanelControl2->Pixel_Format = hkcoglPanelPixelFormat2;
			this->hkoglPanelControl2->Size = System::Drawing::Size(250, 250);
			this->hkoglPanelControl2->TabIndex = 0;
			this->hkoglPanelControl2->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::hkoglPanelControl2_Paint);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 15);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(817, 568);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->hkoglPanelControl1);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Margin = System::Windows::Forms::Padding(4);
			this->Name = L"MyForm";
			this->Text = L"OpenMesh_EX";
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->panel1->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

//hkog panel control 2

private: System::Void hkoglPanelControl2_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e)
{
	glEnable(GL_COLOR_MATERIAL);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1, 2, -1 , 2, -1.0, 1.0);

	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);

	if (mesh != NULL)
		mesh->Render_UV();

	glPopMatrix();
}

//hkog panel control 1
private: System::Void hkoglPanelControl1_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
{
	if (e->KeyCode == Keys::U)
	{
		std::cout << "key down U\n";
		hkoglPanelControl2->Invalidate();
	}
}

private: System::Void hkoglPanelControl1_Load(System::Object^  sender, System::EventArgs^  e)
{

}
private: System::Void hkoglPanelControl1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e)
{
	glEnable(GL_COLOR_MATERIAL);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	point center;
	center[0] = 0.0;
	center[1] = 0.0;
	center[2] = 0.0;
	camera.setupGL(xf * center, 1.0);

	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glMultMatrixd((double *)xf);
	//Axis-------
	glPushMatrix();
	int length = 1000;
	glBegin(GL_LINES);
	glLineWidth(10.0);
	glColor4f(1.0, 0.0, 0.0, 1.0);
	glVertex3f(0, 0, 0);
	glVertex3f(length, 0, 0);

	glColor4f(0.0, 1.0, 0.0, 1.0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, length, 0);

	glColor4f(0.0, 0.0, 1.0, 1.0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, length);
	glEnd();
	glPopMatrix();
	//----------

	if (mesh != NULL)
		mesh->Render_SolidWireframe();
	glPopMatrix();
}

private: System::Void hkoglPanelControl1_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
{
	if (e->Button == System::Windows::Forms::MouseButtons::Left ||
		e->Button == System::Windows::Forms::MouseButtons::Middle)
	{
		point center;
		Mouse_State = Mouse::NONE;
		center[0] = 0.0;
		center[1] = 0.0;
		center[2] = 0.0;
		camera.mouse(e->X, e->Y, Mouse_State,
			xf * center,
			1.0, xf);
	}
	else if (e->Button == System::Windows::Forms::MouseButtons::Right)
	{
		GLdouble M[16] = { 0 }, P[16];
		GLint V[4];

		glPushMatrix();
		glMultMatrixd((double *)xf);
		glGetIntegerv(GL_VIEWPORT, V);
		glGetDoublev(GL_MODELVIEW_MATRIX, M);
		glGetDoublev(GL_PROJECTION_MATRIX, P);
		glPopMatrix();
		
		point mouse;
		mouse[0] = e->X;
		mouse[1] = V[3] - e->Y;

		glReadPixels(mouse[0], mouse[1], 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &mouse[2]);
		GLdouble obj[3];
		gluUnProject(mouse[0], mouse[1], mouse[2], M, P, V, &obj[0], &obj[1], &obj[2]);
		//std::cout << "world pos: " << std::to_string(obj[0]) << " " << std::to_string(obj[1]) << " " << std::to_string(obj[2]) << std::endl;
		mesh->FindNearFace(obj);

		hkoglPanelControl1->Invalidate();
		//hkoglPanelControl2->Invalidate();
	}
}
private: System::Void hkoglPanelControl1_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
{
	if (e->Button == System::Windows::Forms::MouseButtons::Left)
	{
		point center;
		Mouse_State = Mouse::ROTATE;
		center[0] = 0.0;
		center[1] = 0.0;
		center[2] = 0.0;
		camera.mouse(e->X, e->Y, Mouse_State,
			xf * center,
			1.0, xf);
		hkoglPanelControl1->Invalidate();
	}

	if (e->Button == System::Windows::Forms::MouseButtons::Middle)
	{
		point center;
		Mouse_State = Mouse::MOVEXY;
		center[0] = 0.0;
		center[1] = 0.0;
		center[2] = 0.0;
		camera.mouse(e->X, e->Y, Mouse_State,
			xf * center,
			1.0, xf);
		hkoglPanelControl1->Invalidate();
	}
}
private: System::Void hkoglPanelControl1_MouseWheel(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
{
	if (e->Delta < 0)
	{
		point center;
		Mouse_State = Mouse::WHEELDOWN;
		center[0] = 0.0;
		center[1] = 0.0;
		center[2] = 0.0;
		camera.mouse(e->X, e->Y, Mouse_State,
			xf * center,
			1.0, xf);
		hkoglPanelControl1->Invalidate();
	}
	else
	{
		point center;
		Mouse_State = Mouse::WHEELUP;
		center[0] = 0.0;
		center[1] = 0.0;
		center[2] = 0.0;
		camera.mouse(e->X, e->Y, Mouse_State,
			xf * center,
			1.0, xf);
		hkoglPanelControl1->Invalidate();
	}
}
private: System::Void loadModelToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
{
	openModelDialog->Filter = "Model(*.obj)|*obj";
	openModelDialog->Multiselect = false;
	openModelDialog->ShowDialog();
}
private: System::Void openModelDialog_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e)
{
	std::string filename;
	MarshalString(openModelDialog->FileName, filename);

	if (mesh != NULL)
		delete mesh;

	mesh = new Tri_Mesh;
	mesh->Clean();

	if (ReadFile(filename, mesh))
		std::cout << filename << std::endl;

	hkoglPanelControl1->Invalidate();
}
private: System::Void saveModelToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
{
	saveModelDialog->Filter = "Model(*.obj)|*obj";
	saveModelDialog->ShowDialog();
}
private: System::Void saveModelDialog_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e)
{
	std::string filename;
	MarshalString(saveModelDialog->FileName, filename);

	if (SaveFile(filename, mesh))
		std::cout << filename << std::endl;
}
private: System::Void circleToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	if (mesh != NULL)
	{
		mesh->Boundary_type = 1;
		hkoglPanelControl2->Invalidate();
	}
}
private: System::Void squareToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	if (mesh != NULL)
	{
		mesh->Boundary_type = 0;
		hkoglPanelControl2->Invalidate();
	}
}
};
}
