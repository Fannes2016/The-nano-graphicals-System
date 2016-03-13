#include <stdio.h>
#include <stdlib.h>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Text_Display.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Menu.H>

const char *Dir;
const char *Open_Name = "Open";

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

void Exit_App(Fl_Widget*, void*)
{
	exit(0);
}

void Open_Show_App(void)
{
	FILE *f = fopen(Dir, "rb");
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
int main()
{
	printf("Launching Nano Graphicals System...\n");
	printf("\t\t$->Nano<-$\n");
	Fl_Window *Win = new Fl_Window(359, 250, "Nano GUI System 0.27");
	printf("Ready.\n");
	Fl_Pixmap Logo_Exit(Exit_Xpm);
	Fl_Button *Exit = new Fl_Button(0, 0, 30, 20, "");
	Fl_Button *Open = new Fl_Button(30, 0, strlen(Open_Name)*10, 20, "Open");
	Exit->callback(Exit_App);
	Open->callback(Open_App);
	// shortcuts
	Exit->shortcut(FL_CONTROL + 'e');
	Open->shortcut(FL_CONTROL + 'o');
	// images
	Exit->image(&Logo_Exit);
	Win->show();
	return(Fl::run());
}