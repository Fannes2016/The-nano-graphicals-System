#include <stdio.h>
#include <stdlib.h>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Text_Display.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Menu.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Input.H>

const char *Dir;
const char *Open_Name = "Open";
int            changed = 0;
char           filename[256] = "";
Fl_Text_Editor *Editor_Global;
const char *File_Name_Saved;
Fl_Input *File_Name_Input;
Fl_Text_Buffer *Text_Editor_Buffer;
bool Editor_Text_Have_Name = false;
char *newfile;
const char *Local_FileName_ToSave;
Fl_Window *Tell_Saved;
Fl_Window *Ask_Name_Window;
bool Using_Local_Filename = false;

static char *Exit_Xpm[] = {
	"21 27 2 1",
	"X   c #ffffff",
	"*   c #000000",
	"!   c #C0C0C0",
	"!!!!!!!!!!!!!!!!!!!!!",
	"!!!!!!!!!!!!!!!!!!!!!",
	"!!!!!!!!!!!!!!!!!!!!!",
	"!!!!!!!!!!!!!!!!!!!!!",
	"!!!!!!!!!!!!!!!!!!!!!",
	"!!!!!!!!!!!!!!!!!!!!!",
	"!!!!!!!!!!!!!!!!!!!!!",
	"!!!!!!!!!!!!!!!!!!!!!",
	"!!!!!!!!!!!!!!!!!!!!!",
	"!!!!!!!!!!!!!!!!!!!!!",
	"!!!!!!!!!!!!!!!!!!!!!",
	"!!!!!!!!!!!!!!!!!!!!!",
	"XXXXXXXXXXXXXXXXXXXXX",
	"X***X***********X***X",
	"X****X*********X****X",
	"X*****X*******X*****X",
	"X******X*****X******X",
	"X*******X***X*******X",
	"X********X*X********X",
	"X*********X*********X",
	"X********X*X********X",
	"X*******X***X*******X",
	"X******X*****X******X",
	"X*****X*******X*****X",
	"X****X*********X****X",
	"X***X***********X***X",
	"XXXXXXXXXXXXXXXXXXXXX"
};

class Editor_Class : public Fl_Text_Editor {
public:
	Editor_Class(int x, int y, int h, int w, char *l);
};

void Editor_Save_To_File(Fl_Input*, void*)
{
	Local_FileName_ToSave = File_Name_Input->value();
	printf("FileName:%s\n", Local_FileName_ToSave);
	Text_Editor_Buffer->savefile(Local_FileName_ToSave);
	Ask_Name_Window->hide();
}

void Open_And_Save(Fl_Widget*, void*)
{
	Text_Editor_Buffer->savefile(newfile);
	Tell_Saved->hide();
}

void Open_And_Save_Saved(Fl_Widget*, void*)
{
	Text_Editor_Buffer->savefile(Local_FileName_ToSave);
	Tell_Saved->hide();
}

void Open_And_Save_Saved_Not_Widget(void)
{
	Text_Editor_Buffer->savefile(Local_FileName_ToSave);
	//Tell_Saved->hide();
}

void Open_And_Save_Not_Widget(void)
{
	Text_Editor_Buffer->savefile(newfile);
	//Tell_Saved->hide();
}

void Editor_Save(Fl_Widget*, void*)
{
	if(Editor_Text_Have_Name == false && Using_Local_Filename == false)
	{
		Ask_Name_Window = new Fl_Window(150, 40, "Filename?");
		File_Name_Input = new Fl_Input(2, 2, 100, 20, "");
		File_Name_Input->when(FL_WHEN_ENTER_KEY|FL_WHEN_NOT_CHANGED);
		File_Name_Input->callback((Fl_Callback*)Editor_Save_To_File);
		Ask_Name_Window->show();
		Using_Local_Filename = true;
	}
	else if(Editor_Text_Have_Name == true)
	{
		//Tell_Saved = new Fl_Window(150, 100, "Saved!");
		//Fl_Button *Save_Button_Custom = new Fl_Button(0, 0, 150, 100, "Ok");     We don't need this because we don't need callbacks anymore.
		//Save_Button_Custom->callback(Open_And_Save);
		//Tell_Saved->show();
		Open_And_Save_Not_Widget();
	}
	else if(Using_Local_Filename == true)
	{
		//Tell_Saved = new Fl_Window(150, 100, "Saved!");
		//Fl_Button *Save_Button_Custom = new Fl_Button(0, 0, 150, 100, "Ok");
		//Save_Button_Custom->callback(Open_And_Save_Saved);
		//Tell_Saved->show();
		Open_And_Save_Saved_Not_Widget();
	}
}

void Editor_Load(Fl_Widget*, void*)
{
	newfile = fl_file_chooser("Open File?", "*", filename);
	Text_Editor_Buffer->loadfile(newfile);
	Editor_Text_Have_Name = true;
}

void Text_Editor_App(Fl_Widget*, void*)
{
	Editor_Text_Have_Name = false;
	Fl_Window *Text_Editor_Window = new Fl_Window(400, 400, "Text Editor");
	Fl_Text_Editor *Edit_Panel = new Fl_Text_Editor(0, 20, 400-20, 400-20, "");
	Fl_Button *Save_Button = new Fl_Button(0, 0, 40, 20, "Save");
	Fl_Button *Load_Button = new Fl_Button(40, 0, 40, 20, "Load");
	Save_Button->callback(Editor_Save);
	Load_Button->callback(Editor_Load);
	Text_Editor_Buffer = new Fl_Text_Buffer;
	Edit_Panel->textfont(FL_SYMBOL);
	Editor_Global = Edit_Panel;
	Edit_Panel->buffer(Text_Editor_Buffer);
	Save_Button->shortcut(FL_CONTROL + 's');
	Text_Editor_Window->show();
}

void Exit_App(Fl_Widget*, void*)
{
	exit(0);
}

void Open_Show_App(void)
{
	FILE *f = fopen(Dir, "r");
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);

	char *string = (char *)malloc(fsize + 1);
	fread(string, fsize, 1, f);
	fclose(f);

	string[fsize] = 0;
	Fl_Window *Dis = new Fl_Window(200, 200, 400, 500, "Text Display");
	Fl_Text_Display *disp = new Fl_Text_Display(20, 20, 400-20, 500-20, Dir);
    Fl_Text_Buffer *tbuff = new Fl_Text_Buffer();
    disp->buffer(tbuff);
    tbuff->text(string);
    Dis->resizable(disp);
    Dis->show();
}

void Open_App(Fl_Widget*, void*)
{

    // Create the file chooser, and show it
    Fl_File_Chooser chooser(".",                        // directory
                            "*",                        // filter
                            Fl_File_Chooser::MULTI,     // chooser type
                            "Choose?");        // title
    chooser.show();

    // Block until user picks something.
    //     (The other way to do this is to use a callback())
    //
    while(chooser.shown())
        { Fl::wait(); }

    // User hit cancel?
    if ( chooser.value() == NULL )
        { fprintf(stderr, "(User hit 'Cancel')\n"); return; }

    // Print what the user picked
    printf("File Chooser:\n");
    fprintf(stdout, "DIRECTORY: '%s'\n", chooser.directory());
    Dir = chooser.value();
    fprintf(stderr, "    VALUE: '%s'\n", chooser.value());
    fprintf(stderr, "    COUNT: %d files selected\n", chooser.count());

    // Multiple files? Show all of them
    if ( chooser.count() > 1 ) {
        for ( int t=1; t<=chooser.count(); t++ ) {
            fprintf(stderr, " VALUE[%d]: '%s'\n", t, chooser.value(t));
        }
    }
    Open_Show_App();
}

void Execute_App(Fl_Widget*, void*)
{
	Fl_File_Chooser chooser(".",                        // directory
                            "*",                        // filter
                            Fl_File_Chooser::MULTI,     // chooser type
                            "Choose?");        // title
    chooser.show();

    // Block until user picks something.
    //     (The other way to do this is to use a callback())
    //
    while(chooser.shown())
        { Fl::wait(); }

    // User hit cancel?
    if ( chooser.value() == NULL )
        { fprintf(stderr, "(User hit 'Cancel')\n"); return; }

    // Print what the user picked
    printf("File Chooser:\n");
    fprintf(stdout, "DIRECTORY: '%s'\n", chooser.directory());
    Dir = chooser.value();
    fprintf(stderr, "    VALUE: '%s'\n", chooser.value());
    fprintf(stderr, "    COUNT: %d files selected\n", chooser.count());

    // Multiple files? Show all of them
    if ( chooser.count() > 1 ) {
        for ( int t=1; t<=chooser.count(); t++ ) {
            fprintf(stderr, " VALUE[%d]: '%s'\n", t, chooser.value(t));
        }
    }
    char *s1 = "open ";
	char *result = (char *)malloc(strlen(s1)+strlen(strdup(Dir))+1);
	strcpy(result, s1);
    strcat(result, Dir);
    system(result);
}

int main()
{
	printf("Launching Nano Graphicals System (old)...\n");
	printf("\t\t$->Uox<-$\n");
	Fl_Window *Win = new Fl_Window(359, 250, "Uox 1.9");
	printf("Ready.\n");
	Fl_Pixmap Logo_Exit(Exit_Xpm);
	Fl_Button *Exit = new Fl_Button(0, 0, 30, 20, "");
	Fl_Button *Open = new Fl_Button(30, 0, strlen(Open_Name)*10, 20, "Open");
	Fl_Button *Execute = new Fl_Button(70, 0, 60, 20, "Execute");
	Fl_Button *Editor = new Fl_Button(130, 0, 60, 20, "Editor");
	Exit->callback(Exit_App);
	Open->callback(Open_App);
	Execute->callback(Execute_App);
	Editor->callback(Text_Editor_App);
	// shortcuts
	Exit->shortcut(FL_CONTROL + 'q');
	Open->shortcut(FL_CONTROL + 'o');
	Execute->shortcut(FL_CONTROL + 'e');
	Editor->shortcut(FL_CONTROL + 't');
	// images
	Exit->image(&Logo_Exit);
	Win->show();
	return(Fl::run());
}
