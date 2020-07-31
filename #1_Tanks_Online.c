//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::move(TObject *Sender, int i, int v)    //передвижение
{                                                              //нашего танка +
	if (sup == true && tank[0].speed > 0) {
		i *= 2;
		tank[0].speed -= 1;
	}
	else if(tank[0].speed != 25)
		sup = false;
	//-----------------------------------------------------//
	if (v == 0) {
		tank[0].img1->Top += i;
		tank[0].img2->Top += i;
		if (tank[0].img2->Top % 2 && sup == false && tank[0].speed != 25)
			tank[0].speed += 1;
	}
	else
	{
		tank[0].img1->Left += i;
		tank[0].img2->Left += i;
		if (tank[0].img2->Left % 2 && sup == false && tank[0].speed != 25)
			tank[0].speed += 1;
	}
   //-----------------------------------------------------//
	Panel1->Left  = tank[0].img1->Left;
	Panel1->Width = floor((tank[0].speed*68.0)/25.0);
	Panel2->Left  = Panel1->Left+Panel1->Width;
	Panel2->Width = 68-Panel1->Width;
	Panel1->Top   = tank[0].head == 4 ? (tank[0].img1->Top-16) : (tank[0].img1->Top+72);
	Panel2->Top   = Panel1->Top;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::randpos(TObject *Sender)
{
    int rx = rand() % (800) + 10,
		ry = rand() % (420) + 10;
		tank[0].img1->Top = ry;
		tank[0].img2->Top = ry;
		tank[0].img1->Left = rx;
		tank[0].img2->Left = rx;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::bulL(TObject *Sender, int h, int lx, int i)
{
	bool h3 = bullet[i].point->Top > tank[0].img1->Top-h,
		 h4 = bullet[i].point->Top < tank[0].img1->Top+h;
	//-----------------------------------------------------//
	int l1 = bullet[i].point->Left+bullet[i].point->Height/2,
		l2 = tank[0].img1->Left+tank[0].img1->Height/2;
	//-----------------------------------------------------//
	int l = (l1 < l2 ? l2 : l1) - (l1 < l2 ? l1 : l2);
	//-----------------------------------------------------//
	if (h3 && h4 && l < lx)
	{
		randpos(Sender);
		if (bullet[i].v == 2)
			bullet[i].point->Left = -40;
		if (bullet[i].v == 3)
			bullet[i].point->Left = Width+5;
		IdUDPServer1->Send(masip, StrToInt(portE->Text), "!"+IntToStr(i)+"="+IntToStr(bullet[i].v));
		pointl++;
        score(Sender);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::bulT(TObject *Sender, int h, int lx, int i)
{
	bool h1 = bullet[i].point->Left > tank[0].img1->Left-h,
		 h2 = bullet[i].point->Left < tank[0].img1->Left+h;
	//-----------------------------------------------------//
	int l1 = bullet[i].point->Top+bullet[i].point->Width/2,
		l2 = tank[0].img1->Top+tank[0].img1->Width/2;
	//-----------------------------------------------------//
	int l = (l1 < l2 ? l2 : l1) - (l1 < l2 ? l1 : l2);
	//-----------------------------------------------------//
	if (h1 && h2 && l < lx)
	{
		randpos(Sender);
		if (bullet[i].v == 4)
			bullet[i].point->Top = Height;
		if (bullet[i].v == 5)
			bullet[i].point->Top = -40;
		IdUDPServer1->Send(masip, StrToInt(portE->Text), "!"+IntToStr(i)+"="+IntToStr(bullet[i].v));
        pointl++;
        score(Sender);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::score(TObject *Sender)
{
	Label4->Caption = IntToStr(pointl) + " : " + IntToStr(pointr);
	Label4->Left = ClientWidth/2-Label4->Width/2;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::bind(TObject *Sender, int i, AnsiString ip, int port)
{
	IdUDPServer1->Bindings->operator[](i)->Port = port;       //бинд серва +
	IdUDPServer1->Bindings->operator[](i)->IP = ip;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::doo(TObject *Sender) //переход в меню   +
{
	Button1->Visible = conval == -1 ? false : true;
	Button2->Visible = conval == -1 ? false : true;
	BCreate->Visible = conval == -1 ? true : false;
	Label1->Visible  = conval == -1 ? true : false;
	Label2->Visible  = conval == -1 ? true : false;
	portE->Visible   = conval == -1 ? true : false;
	ipE->Visible     = conval == -1 ? true : false;
	back->Visible    = conval == -1 ? true : false;
	Color            = conval == -1 ? clYellow : clLime;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::stat(TObject *Sender, int i)//рандомизация положения танка,
{                                                   //          тела и башни  +
    srand(time(0));
	//-----------------------------------------------------//
	int rx = rand() % (800) + 10,
		ry = rand() % (420) + 10,
		r = rand() % 2;
	//-----------------------------------------------------//
		tank[i].img1 = new TImage(Form1);
		tank[i].img1->Parent = Form1;
		tank[i].img1->Width  = 68;
		tank[i].img1->Height = 68;
		tank[i].img1->Top    = ry;
		tank[i].img1->Left   = rx;
	//-----------------------------------------------------//
		tank[i].img2 = new TImage(Form1);
		tank[i].img2->Parent = Form1;
		tank[i].img2->Width  = 68;
		tank[i].img2->Height = 68;
		tank[i].img2->Top    = ry;
		tank[i].img2->Left   = rx;
	//-----------------------------------------------------//
	tank[i].img1->Transparent = true;
	tank[i].img2->Transparent = true;
	//-----------------------------------------------------//
	ImageList1->Draw(tank[i].img1->Canvas,0,0,r);
	//-----------------------------------------------------//
	if (r == 0)
		r = rand() % (2)+2;
	else
		r = rand() % (2)+4;
    //-----------------------------------------------------//
	ImageList1->Draw(tank[i].img2->Canvas,0,0,r);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::clear1(TObject *Sender, int i)   //очистка корпуса   +
{
	Form1->tank[i].img1->Canvas->Brush->Color = clWhite;
	Form1->tank[i].img1->Canvas->FillRect(Canvas->ClipRect);
	Form1->tank[i].img1->Canvas->MoveTo(0,0);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::clear2(TObject *Sender, int i)   //очистка башни     +
{
	Form1->tank[i].img2->Canvas->Brush->Color = clWhite;
	Form1->tank[i].img2->Canvas->FillRect(Canvas->ClipRect);
	Form1->tank[i].img2->Canvas->MoveTo(0,0);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{                                                   //кнопки в меню +
	Button1->Width = Width-23;
	Button2->Width = Width-23;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender) //клик создать комнату
{
	doo(Sender);
	conval = 0;
	ipE->Text = IdIPWatch1->LocalIP();
	ipE->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender)  //клик присоедениться
{
	doo(Sender);
	conval = 1;
	AnsiString a = IdIPWatch1->LocalIP().SubString(1,IdIPWatch1->LocalIP().Pos("."));
	a += IdIPWatch1->LocalIP().SubString(IdIPWatch1->LocalIP().Pos(".")+1,IdIPWatch1->LocalIP().Pos("."));
    ipE->Text = a;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::backClick(TObject *Sender)
{
	doo(Sender);                         //выход из присоединения
	conval = -1;
    ipE->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
	if (portE->Text != "" && masip != "") 
		IdUDPServer1->Send(masip, StrToInt(portE->Text), "-");
	//-----------------------------------------------------//
	IdUDPServer1->Active = false;
    //-----------------------------------------------------//
	free(bg);    free(bullet);
	free(tank);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::BCreateClick(TObject *Sender)  //начинаем игру
{
	try
	{
		//-------------------------------------------------------// бинд сервака
		bind(Sender, 0, IdIPWatch1->LocalIP(), StrToInt(portE->Text));
		IdUDPServer1->Active = true;
		//-------------------------------------------------------// если мы коннектимся
		if (conval == 1)                                         // отправляем айпи
		{
			masip = ipE->Text;
			IdUDPServer1->Send(ipE->Text, StrToInt(portE->Text), "#"+IdIPWatch1->LocalIP());
		}
		//-------------------------------------------------------// прописываем свойства
		AnsiString fh = IdIPWatch1->LocalIP(), fx = ipE->Text;   // комнаты
				   fh = fh.c_str() == fx ? fh : fx;
		Label3->Visible = true;
		Label3->Caption = "ip: " + fh + "  port: " + portE->Text;
		//-------------------------------------------------------// обработка вида
		BCreate->Visible = false;   Label1->Visible = false;     // формы
		Label2->Visible = false;    Button3->Visible = true;
		ipE->Visible = false;       portE->Visible = false;
		Color = clWhite;            back->Visible = false;
		Width = 890;                Height = 520;
		bullet = nullptr;           Panel1->Visible = true;
		Form1->Position = poDesktopCenter;   Panel2->Visible = true;
		//-------------------------------------------------------// прорисовка бэкграуна
		bg = new TImage(Form1);   	bg->Parent = Form1;
		bg->Width = 890;          	bg->Height = 520;
		bg->SendToBack();         	Label3->BringToFront();
		Label4->BringToFront();   	score(Sender);
		bg->Picture->Bitmap->LoadFromResourceName(0,"Bitmap_1");
		//-------------------------------------------------------// инструкция
		Form2->Top = Form1->Top-Form2->ClientHeight;
        Form2->Left = Form1->Left+3;
		Form2->Visible = true;  Form2->Timer1->Enabled = true;
		Form1->SetFocus();
		//----------------------// создаем танкu
		stat(Sender, 0);            stat(Sender, 1);
		tank[1].img1->Top = -60;    tank[1].img1->Left = -60;
		tank[1].img2->Top = -60;    tank[1].img2->Left = -60;
        Panel1->Left = tank[0].img1->Left;
		Panel1->Width = floor((tank[0].speed*68.0)/25.0);
		Panel2->Left = Panel1->Left+Panel1->Width;
		Panel2->Width = 68-Panel1->Width;
		Panel1->Top = tank[0].head == 4 ? (tank[0].img1->Top-16) : (tank[0].img1->Top+72);
		Panel2->Top = Panel1->Top;
		//----------------------// включаем управление И передачу инфы
		Timer1->Enabled = true;
		if (conval == 1)
			Timer3->Enabled = true;
	}
	catch(...)
	{
		ShowMessage("Не удалось поиграть");
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer1Timer(TObject *Sender)                 //  управление
{                                                                    //  танком
	if (GetAsyncKeyState(VK_UP) && tank[0].img1->Top > 5)
		move(Sender, -1, 0);
	//---------------------------------------------------------------//
	if (GetAsyncKeyState(VK_DOWN) && tank[0].img1->Top != Height-96)
		move(Sender, 1, 0);
	//---------------------------------------------------------------//
	if (GetAsyncKeyState(VK_LEFT) && tank[0].img1->Left != 0)
		move(Sender, -1, 1);
	//---------------------------------------------------------------//
	if (GetAsyncKeyState(VK_RIGHT) && tank[0].img1->Left != Width-75)
		move(Sender, 1, 1);
	//---------------------------------------------------------------//
	if (GetKeyState(67))
	{
        keybd_event(67,0,0,0);
		keybd_event(67,0,KEYEVENTF_KEYUP,0);
    }
}
//---------------------------------------------------------------------------
int __fastcall TForm1::KEY(TObject *Sender, int Key, int num)
{
    int check;
	if (Key == 67)
		check = 0;
	else
		check = 1;

    clear2(Sender, 0);
	ImageList1->Draw(tank[0].img2->Canvas,0,0,num);
	tank[0].head = num;

	return check;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	static int check = 0; // Фиксация башни
	//-----------------------------------------------------//
	if ((Key == VK_LEFT || Key == VK_RIGHT))
	{
		if (tank[0].body != 0)
		{
			clear1(Sender, 0);                             // перерисовка танка
			ImageList1->Draw(tank[0].img1->Canvas,0,0,0);  // при изменении
			tank[0].body = 0;                              // напраления
        }
		if (check == 0)
		{
			clear2(Sender, 0);
			if (Key == VK_LEFT)
			{
				ImageList1->Draw(tank[0].img2->Canvas,0,0,2);
				tank[0].head = 2;
			}
			if (Key == VK_RIGHT)
			{
				ImageList1->Draw(tank[0].img2->Canvas,0,0,3);
                tank[0].head = 3;
			}
		}
        if (Key == VK_LEFT)
			pos = 1;
		else
			pos = 2;
	}
	if (Key == VK_DOWN || Key == VK_UP)
	{
		if (tank[0].body != 1)
		{
			clear1(Sender, 0);                             // перерисовка танка
			ImageList1->Draw(tank[0].img1->Canvas,0,0,1);  // при изменении
			tank[0].body = 1;                              // напраления
		}
		if (check == 0)
		{
			clear2(Sender, 0);
			if (Key == VK_DOWN)
			{
				ImageList1->Draw(tank[0].img2->Canvas,0,0,4);
                tank[0].head = 4;
			}
			if (Key == VK_UP)
			{
				ImageList1->Draw(tank[0].img2->Canvas,0,0,5);
				tank[0].head = 5;
			}
		}
		if (Key == VK_DOWN)
			pos1 = 1;
		else
            pos1 = 2;
	}
	switch (Key)
	{
		case 16:                                               //shift
			sup = true;
        break;
		//-----------------------------------------------------//  w
		case 87:
			check = KEY(Sender, 87, 5);
		break;
		//-----------------------------------------------------//  a
		case 65:
			check = KEY(Sender, 65, 2);
		break;
		//-----------------------------------------------------//  s
		case 83:
			check = KEY(Sender, 83, 4);
		break;
		//-----------------------------------------------------//  d
		case 68:
			check = KEY(Sender, 68, 3);
		break;
		//-----------------------------------------------------//  c
		case 67:
            if (pos == 1)
				check = KEY(Sender, 67, 2);
			else
			if (pos == 2)
				check = KEY(Sender, 67, 3);
			else
			if (pos1 == 1)
				check = KEY(Sender, 67, 4);
			else
			if (pos1 == 2)
				check = KEY(Sender, 67, 5);
		break;
		//-----------------------------------------------------//space
		case 32:
		if (cooldown == 0)
		{
			int temp = sizeb;

			sizeb++;
			bullet = (bul*)realloc(bullet,sizeof(bul)*sizeb);
			bullet[sizeb-1].point = new TImage(Form1);
			bullet[sizeb-1].point->Parent = Form1;
			bullet[sizeb-1].point->Width = 68;
			bullet[sizeb-1].point->Height = 68;
			bullet[sizeb-1].point->Transparent = true;

			if (tank[0].head == 5 || tank[0].head == 4) {
				ImageList1->Draw(bullet[sizeb-1].point->Canvas,0,0,7);
				if (tank[0].head == 5) {
					bullet[sizeb-1].v = 5;
					bullet[sizeb-1].point->Top = tank[0].img2->Top-34;
                    bullet[sizeb-1].point->Left = tank[0].img2->Left;
				}
                if (tank[0].head == 4) {
					bullet[sizeb-1].v = 4;
					bullet[sizeb-1].point->Top = tank[0].img2->Top+34;
                    bullet[sizeb-1].point->Left = tank[0].img2->Left;
				}
			}
			if (tank[0].head == 2 || tank[0].head == 3) {
				ImageList1->Draw(bullet[sizeb-1].point->Canvas,0,0,6);
				if (tank[0].head == 2) {
					bullet[sizeb-1].v = 2;
					bullet[sizeb-1].point->Top = tank[0].img2->Top;
					bullet[sizeb-1].point->Left = tank[0].img2->Left-34;
				}
				if (tank[0].head == 3) {
					bullet[sizeb-1].v = 3;
					bullet[sizeb-1].point->Top = tank[0].img2->Top;
					bullet[sizeb-1].point->Left = tank[0].img2->Left+34;
				}
			}

			if (temp == 0)
				Timer2->Enabled = true;

            numi = 0;
			cooldown = 1;
			Timer4->Enabled = true;
            Timer5->Enabled = true;

			if (!masip.IsEmpty())
			{
				AnsiString mes = "*";

				mes += IntToStr(bullet[sizeb-1].v);
				mes += "=";
				mes += IntToStr(bullet[sizeb-1].point->Left);
				mes += "=";
				mes += IntToStr(bullet[sizeb-1].point->Top);
                mes += "=";

				IdUDPServer1->Send(masip, StrToInt(portE->Text), mes);
			}
        }
		break;
        //-----------------------------------------------------//
		case 13:
			if (BCreate->Visible == true && portE->Text != "" && ipE->Text != "")
				BCreate->Click();
        break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer2Timer(TObject *Sender)
{
	for (int i = 0; i < sizeb; i++)
	{
		if (bullet[i].v == 2)
			bullet[i].point->Left -= 3;
		if (bullet[i].v == 3)
			bullet[i].point->Left += 3;
		if (bullet[i].v == 4)
			bullet[i].point->Top += 3;
		if (bullet[i].v == 5)
			bullet[i].point->Top -= 3;

		if (bullet[i].point->Top < -50 || bullet[i].point->Top > Height+10 ||
			 bullet[i].point->Left < -50 || bullet[i].point->Left > Width+9)
		{
			if (sizeb != 1)
			{
				for (int j = i; j < sizeb-1; j++)
				{
					bullet[j].point = bullet[j+1].point;
					bullet[j].v = bullet[j+1].v;
				}
				sizeb--;
				bullet = (bul*)realloc(bullet,sizeof(bul)*sizeb);
			}
			else
			{
                sizeb--;
				bullet = (bul*)realloc(bullet,1);
			}
		}
		//===============================================================//
		if (tank[0].body == 0 && (bullet[i].v == 4 || bullet[i].v == 5)) // бочком
			bulT(Sender, 34, 23, i);
		//===============================================================//
		if (tank[0].body == 1 && (bullet[i].v == 4 || bullet[i].v == 5)) // в анфас
			bulT(Sender, 19, 34, i);
		//===============================================================//
		if (tank[0].body == 1 && (bullet[i].v == 2 || bullet[i].v == 3)) // бочком
			bulL(Sender, 34, 23, i);
		//===============================================================//
		if (tank[0].body == 0 && (bullet[i].v == 2 || bullet[i].v == 3)) // в анфас
			bulL(Sender, 19, 34, i);
		//===============================================================//
	}

	if (sizeb == 0)
		Timer2->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button3Click(TObject *Sender)
{
	IdUDPServer1->Send(masip, StrToInt(portE->Text), "-");
	IdUDPServer1->Active = false;
    pointl = 0, pointr = 0;
    Label4->Caption = "";
    free(bullet);
	sizeb = 0;

	keybd_event(67,0,0,0);
	keybd_event(67,0,KEYEVENTF_KEYUP,0);
	Form2->Visible = false;
	ipE->Enabled = false;
	Timer3->Enabled = false;

	for (int i = 0; i < 2; i++)
	{
		tank[i].img1->Width = 0;
		tank[i].img2->Width = 0;
		free(tank[i].img1);
		free(tank[i].img2);
	}

	bg->Width = 0;
    free(bg);

	Canvas->Brush->Color = clYellow;
	Canvas->FillRect(Canvas->ClipRect);
	Canvas->MoveTo(0,0);

	BCreate->Visible = true;  Label1->Visible = true;
	Label2->Visible = true;   portE->Visible = true;
	ipE->Visible = true;      back->Visible = true;
	Color = clYellow;  		  Button3->Visible = false;
	Width = 277;              Height = 164;
	Label3->Visible = false;  Panel1->Visible = false;
	portE->Enabled = true;    Panel2->Visible = false;
	Form1->Position = poDesktopCenter;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer4Timer(TObject *Sender)
{
	cooldown = 0;
    Timer4->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Key == VK_LEFT || Key == VK_RIGHT)
		pos = 0;

	if (Key == VK_DOWN || Key == VK_UP)
		pos1 = 0;

	if ((Key == VK_LEFT || Key == VK_RIGHT) && pos1 != 0)
		keybd_event(pos1 == 1 ? VK_DOWN : VK_UP,0,0,0);

	if ((Key == VK_DOWN || Key == VK_UP) && pos != 0)
		keybd_event(pos == 1 ? VK_LEFT : VK_RIGHT,0,0,0);

	if (Key == VK_SHIFT)
		sup = false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer3Timer(TObject *Sender)
{
	try {
		AnsiString mes = "";

		mes += IntToStr(tank[0].head);
		mes += "=";
		mes += IntToStr(tank[0].body);
		mes += "=";
		mes += IntToStr(tank[0].img1->Left);
		mes += "=";
		mes += IntToStr(tank[0].img1->Top);
		mes += "=";

		IdUDPServer1->Send(masip, StrToInt(portE->Text), mes);
	}
	catch (...)
	{
		Timer3->Enabled = false;
		Button3->Click();
		ShowMessage("Соединение прервано...");
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::IdUDPServer1UDPRead(TIdUDPListenerThread *AThread, const TIdBytes AData,
		  TIdSocketHandle *ABinding)
{
	static int posh = -1, posb = -1;
	AnsiString mes = BytesToString(AData);
    if (mes.Pos("#") == 1 && conval == 0)
	{
		mes.Delete(1,1);
		masip = mes;
		if (Timer3->Enabled == false)
			Timer3->Enabled = true;
	}
	else if (mes.Pos("-") == 1)
	{
		Timer3->Enabled = false;
		tank[1].img1->Left = -60;
		tank[1].img2->Left = -60;
        masip = "";
	}
	else if (mes.Pos("!") == 1)
	{
		pointr++;
        score(this);
		mes.Delete(1,1);
		int i = StrToInt(mes.SubString(0,mes.Pos("=")-1));
		bullet[i].point->Left = -100;
	}
	else if (mes.Pos("*") == 1)
	{
		Timer2->Enabled = false;
		sizeb++;
		bullet = (bul*)realloc(bullet,sizeof(bul)*sizeb);
		bullet[sizeb-1].point = new TImage(Form1);
		bullet[sizeb-1].point->Parent = Form1;
		bullet[sizeb-1].point->Width = 68;
		bullet[sizeb-1].point->Height = 68;
		bullet[sizeb-1].point->Transparent = true;

		mes.Delete(1,1);
		bullet[sizeb-1].v = StrToInt(mes.SubString(0,mes.Pos("=")-1));
		mes.Delete(1,mes.Pos("="));

		bullet[sizeb-1].point->Left = StrToInt(mes.SubString(0,mes.Pos("=")-1));
		mes.Delete(1,mes.Pos("="));

		bullet[sizeb-1].point->Top = StrToInt(mes.SubString(0,mes.Pos("=")-1));

		if (bullet[sizeb-1].v == 5 || bullet[sizeb-1].v == 4)
			ImageList1->Draw(bullet[sizeb-1].point->Canvas,0,0,7);
		else
			ImageList1->Draw(bullet[sizeb-1].point->Canvas,0,0,6);
		Timer2->Enabled = true;
        numi = 1;
        Timer5->Enabled = true;
	}
	else
	{
		tank[1].head = StrToInt(mes.SubString(0,mes.Pos("=")-1));
		mes.Delete(1,mes.Pos("="));

		tank[1].body = StrToInt(mes.SubString(0,mes.Pos("=")-1));
		mes.Delete(1,mes.Pos("="));

		tank[1].img1->Left = StrToInt(mes.SubString(0,mes.Pos("=")-1));
		tank[1].img2->Left = StrToInt(mes.SubString(0,mes.Pos("=")-1));
		mes.Delete(1,mes.Pos("="));

		tank[1].img1->Top = StrToInt(mes.SubString(0,mes.Pos("=")-1));
		tank[1].img2->Top = StrToInt(mes.SubString(0,mes.Pos("=")-1));
		if (posh != tank[1].head)
		{
			clear2(this, 1);
			posh = tank[1].head; 
			ImageList1->Draw(tank[1].img2->Canvas,0,0,posh);   
		}
		if (posb != tank[1].body) 
		{
			clear1(this, 1);
			posb = tank[1].body;
			ImageList1->Draw(tank[1].img1->Canvas,0,0,posb);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer5Timer(TObject *Sender)
{
/*	static int kl = 0, h = 0;

	if (kl == 0)
	{
		kl++;
		h = tank[numi].head;
		boom->Width = 68;
		boom->Height = 68;
		boom->Transparent = true;
		if (h == 2)
			ImageList1->Draw(boom->Canvas,0,0,14);
		else
		if (h == 3)
			ImageList1->Draw(boom->Canvas,0,0,12);
		else
		if (h == 4)
			ImageList1->Draw(boom->Canvas,0,0,10);
		else
		if (h == 5)
			ImageList1->Draw(boom->Canvas,0,0,8);
        boom->BringToFront();
	}
	if (kl == 1)
	{
		kl++;
		boom->Canvas->Brush->Color = clWhite;
		boom->Canvas->FillRect(Canvas->ClipRect);
		boom->Canvas->MoveTo(0,0);
        if (h == 2)
			ImageList1->Draw(boom->Canvas,0,0,15);
		else
		if (h == 3)
			ImageList1->Draw(boom->Canvas,0,0,13);
		else
		if (h == 4)
			ImageList1->Draw(boom->Canvas,0,0,11);
		else
		if (h == 5)
			ImageList1->Draw(boom->Canvas,0,0,9);
	}
	else
	{
		kl = 0; h = 0;
		boom->Width = 0;
		//delete boom;
		//boom = nullptr; */
        Timer5->Enabled = false;
   //	}
}
//---------------------------------------------------------------------------
