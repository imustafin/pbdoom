// from https://github.com/pmartin/pocketbook-demo/blob/master/demo02-gdb/demo02.cpp
#include "inkview.h"

static const int kFontSize = 16;


static int test_debug(ifont *font, int a, int b)
{
    char buffer[1024];
    int result = 0;

    fprintf(stderr, "Hi!\n");

    snprintf(buffer, sizeof(buffer), "a=%d b=%d result=%d", a, b, result);
    DrawTextRect(0, 0, ScreenWidth(), kFontSize, buffer, ALIGN_LEFT);
    FullUpdate();

    a += 10;

    fprintf(stderr, "a=%d\n", a);

    snprintf(buffer, sizeof(buffer), "a=%d b=%d result=%d", a, b, result);
    DrawTextRect(0, 20, ScreenWidth(), kFontSize, buffer, ALIGN_LEFT);
    FullUpdate();

    b *= 2;

    fprintf(stderr, "b=%d\n", b);

    snprintf(buffer, sizeof(buffer), "a=%d b=%d result=%d", a, b, result);
    DrawTextRect(0, 40, ScreenWidth(), kFontSize, buffer, ALIGN_LEFT);
    FullUpdate();

    result = a * b;
    snprintf(buffer, sizeof(buffer), "a=%d b=%d result=%d", a, b, result);
    DrawTextRect(0, 60, ScreenWidth(), kFontSize, buffer, ALIGN_LEFT);
    FullUpdate();

    fprintf(stderr, "DONE!");

    return 0;
}


static int main_handler(int event_type, int param_one, int param_two)
{
    if (EVT_INIT == event_type) {
        ifont *font = OpenFont("LiberationSans", kFontSize, 0);

        ClearScreen();
        SetFont(font, BLACK);

        test_debug(font, 1000, 3);

        CloseFont(font);
    }
    else if (EVT_KEYPRESS == event_type) {
        CloseApp();
    }
    return 0;
}


int main (int argc, char* argv[])
{
    InkViewMain(main_handler);
    return 0;
}
