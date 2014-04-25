/*
    Copyright (C) 1995-1997
        Rainer Schnitker, Heeper Str. 283, 33607 Bielefeld
        email: rainer@mathematik.uni-bielefeld.de

    All rights reserved
*/

#ifdef __RSXNT__

#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>

#include <string.h>

#define FG2BG(i) (i<<4)
#define BG2FG(i) (i>>4)

#define BG_WHITE    (BACKGROUND_GREEN|BACKGROUND_BLUE|BACKGROUND_RED)
#define BG_RED      (BACKGROUND_RED)
#define BG_GREEN    (BACKGROUND_GREEN)
#define BG_YELLOW   (BACKGROUND_GREEN|BACKGROUND_RED)
#define BG_BLUE     (BACKGROUND_BLUE)
#define BG_MAGNETA  (BACKGROUND_BLUE|BACKGROUND_RED)
#define BG_CYAN     (BACKGROUND_GREEN|BACKGROUND_BLUE)
#define BG_BLACK    (0)
#define BG_DEFAULT  BG_BLACK
#define BG_INTENS   BACKGROUND_INTENSITY

#define FG_WHITE    (FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_RED)
#define FG_RED      (FOREGROUND_RED)
#define FG_GREEN    (FOREGROUND_GREEN)
#define FG_YELLOW   (FOREGROUND_GREEN|FOREGROUND_RED)
#define FG_BLUE     (FOREGROUND_BLUE)
#define FG_MAGNETA  (FOREGROUND_BLUE|FOREGROUND_RED)
#define FG_CYAN     (FOREGROUND_GREEN|FOREGROUND_BLUE)
#define FG_BLACK    (0)
#define FG_DEFAULT  FG_WHITE
#define FG_INTENS   FOREGROUND_INTENSITY

static short BgColor[8] =
{
    BG_BLACK,
    BG_RED,
    BG_GREEN,
    BG_YELLOW,
    BG_BLUE,
    BG_MAGNETA,
    BG_CYAN,
    BG_WHITE
};

static short FgColor[8] =
{
    FG_BLACK,
    FG_RED,
    FG_GREEN,
    FG_YELLOW,
    FG_BLUE,
    FG_MAGNETA,
    FG_CYAN,
    FG_WHITE
};

static void clear_line(int x, int y, int size, WORD ColorB)
{
    static COORD src_size;
    static COORD src_pos;
    CHAR_INFO ci[256];
    SMALL_RECT srect;
    int i;

    for (i = 0; i < size; ++i) {
        ci[i].Char.AsciiChar = ' ';
        ci[i].Attributes = (char) (ColorB | BG2FG(ColorB));
    }
    src_size.X = 256;
    src_size.Y = 1;
    src_pos.X = 0;
    src_pos.Y = 0;
    srect.Left = x;
    srect.Top = y;
    srect.Right = x + size - 1;
    srect.Bottom = y;
    WriteConsoleOutput(GetStdHandle(STD_OUTPUT_HANDLE),
                        ci, src_size, src_pos, &srect);
}

static void clear_lines(int from, int to, int size, WORD ColorB)
{
    int y;

    for (y = from; y <= to; ++y)
        clear_line(0, y, size, ColorB);
}

static void GetConsoleCursorPosition(HANDLE handle, COORD *coord)
{
    CONSOLE_SCREEN_BUFFER_INFO scbi;

    GetConsoleScreenBufferInfo(handle, &scbi);
    coord->X = scbi.dwCursorPosition.X;
    coord->Y = scbi.dwCursorPosition.Y;
}

static void GetConsoleWindow(HANDLE handle, COORD *coord)
{
    CONSOLE_SCREEN_BUFFER_INFO scbi;

    GetConsoleScreenBufferInfo(handle, &scbi);
    coord->X = scbi.dwSize.X;
    coord->Y = scbi.dwSize.Y;
}

static int rsxio_ansi(const char *lpBuf, int size)
{
    static POINT CursorSave;            // Cursor ansi save
    static char  tBreakLine;            // Termcap mode
    static char  ColorMode;             // Color ansi mode
    static WORD  ColorF = FG_DEFAULT;   // Color ansi mode
    static WORD  ColorB = BG_DEFAULT;   // Color ansi mode

    int p[4];                           // parameter
    int pn;                             // no of parmameter
    int n,i;                            // offset buffer
    int bexit = 1;                      // exit flag
    char c;
    COORD cc;
    COORD cw;
    HANDLE console_handle;

    if (lpBuf[1] != '[')
        return 0;

    console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleWindow(console_handle, &cw);

    pn = p[0] = p[1] = p[2] = p[3] = 0;

    n = 2;
    while (bexit && n <= size) {
        c = lpBuf[n];

        switch (c) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                p[pn] *= 10;
                p[pn] += c - '0';
                break;

            case ';':
                ++pn;
                if (pn >= 4)
                    pn = 0;
                break;

            case 'A':               // cursor up
                if (!p[0])
                    p[0] = 1;
                GetConsoleCursorPosition(console_handle, &cc);
                cc.Y -= p[0];
                if (cc.Y < 0)
                    cc.Y = 0;
                SetConsoleCursorPosition(console_handle, cc);
                bexit = 0;
                break;

            case 'B':               // cursor down
                if (!p[0])
                    p[0] = 1;
                GetConsoleCursorPosition(console_handle, &cc);
                cc.Y += p[0];
                if (cc.Y >= cw.Y)
                    cc.Y = cw.Y - 1;
                SetConsoleCursorPosition(console_handle, cc);
                bexit = 0;
                break;

            case 'C':               // cursor right
                if (!p[0])
                    p[0] = 1;
                GetConsoleCursorPosition(console_handle, &cc);
                cc.X += p[0];
                if (cc.X >= cw.X)
                    cc.X = cw.X - 1;
                SetConsoleCursorPosition(console_handle, cc);
                bexit = 0;
                break;

            case 'D':               // cursor left
                if (!p[0])
                    p[0] = 1;
                GetConsoleCursorPosition(console_handle, &cc);
                cc.X -= p[0];
                if (cc.X < 0)
                    cc.X = 0;
                SetConsoleCursorPosition(console_handle, cc);
                bexit = 0;
                break;

            case 'f':
            case 'H':               // cursor position
                if (pn != 1) {
                    p[0] = 0;
                    p[1] = 0;
                } else {    // home = 1,1 ; screen = 0,0
                    p[1]--;
                    p[0]--;
                }
                cc.Y = p[0];
                cc.X = p[1];
                SetConsoleCursorPosition(console_handle, cc);
                bexit = 0;
                break;

            case 'n':               // cursor pos in keyb buf
                if (p[0] == 6) {
                    static char keyin[8];      // ESC [ y y ; x x R
                    int t;
                    int bytes = 0;

                    GetConsoleCursorPosition(console_handle, &cc);
                    keyin[bytes++] = 27;
                    keyin[bytes++] = '[';
                    t = cc.Y + 1;
                    if ((keyin[bytes] = t / 10 + '0') != '0')
                        bytes++;
                    keyin[bytes++] = t % 10 + '0';
                    keyin[bytes++] = ';';
                    t = cc.X + 1;
                    if ((keyin[bytes] = t / 10 + '0') != '0')
                        bytes++;
                    keyin[bytes++] = t % 10 + '0';
                    keyin[bytes++] = 'R';
                }
                bexit = 0;
                break;

            case 's':               // save cursor position
                GetConsoleCursorPosition(console_handle, &cc);
                CursorSave.y = cc.Y;
                CursorSave.x = cc.X;
                bexit = 0;
                break;

            case 'u':               // restore cursor position
                cc.Y = CursorSave.y;
                cc.X = CursorSave.x;
                SetConsoleCursorPosition(console_handle, cc);
                bexit = 0;
                break;

            case 'J':               // erase in display
                switch (p[0]) {
                case 0:             // erase from cursor to end of screen
                    GetConsoleCursorPosition(console_handle, &cc);
                    clear_lines(cc.Y, cw.Y - 1, cw.X, ColorB);
                    break;
                case 1:             // erase from start to cursor
                    clear_lines(0, cc.Y, cw.X, ColorB);
                    break;
                case 2:             // erase screen
                    clear_lines(0, cw.Y - 1, cw.X, ColorB);
                    break;
                default:
                    break;
                }
                bexit = 0;
                break;

            case 'K':               // erase in line
                GetConsoleCursorPosition(console_handle, &cc);
                switch (p[0]) {
                case 0:             // erase to end of line
                    clear_line(cc.X, cc.Y, cw.X - cc.X, ColorB);
                    break;
                case 1:             // erase to beginning of line
                    clear_line(0, cc.Y, cc.X + 1, ColorB);
                    break;
                case 2:             // erase whole line
                    clear_line(0, cc.Y, cw.X, ColorB);
                    break;
                default:
                    break;
                }
                bexit = 0;
                break;

            case 'L':               // insert n lines preceding current line
                bexit = 0;
                break;

            case 'M':               // delete n lines from current position down
                bexit = 0;
                break;

            case 'P':               // delete n chars from cursor to the left
                bexit = 0;
                break;

            case 'l':               // reset mode (begin termcap)
                if (p[0] == 7)
                    tBreakLine = FALSE;
                bexit = 0;
                break;

            case 'h':               // set mode (end termcap)
                if (p[0] == 7)
                    tBreakLine = TRUE;
                bexit = 0;
                break;

            case 'm':               // colors, attributes
                for (i = 0; i <= pn; i++) {
                    if (p[i] == 0) { // reset mode
                        ColorF = FG_DEFAULT;
                        ColorB = BG_DEFAULT;
                        ColorMode = 0;
                        SetConsoleTextAttribute(console_handle, ColorF | ColorB);
                    }
                    else if (p[i] <= 8)
                        ColorMode = p[i];
                    else if (p[i] >= 30 && p[i] <= 37) {
                        ColorF = FgColor[p[i] - 30];
                        if (ColorMode == 1)
                            ColorF |= FG_INTENS;
                    }
                    else if (p[i] >= 40 && p[i] <= 47) {
                        ColorB = BgColor[p[i] - 40];
                        //if (ColorMode == 1)
                        //    ColorB |= BG_INTENS;
                    }
                }
                if (ColorMode == 7) {
                    WORD FG, BG;
                    FG = ColorB >> 4;
                    BG = ColorF << 4;
                    SetConsoleTextAttribute(console_handle, FG | BG);
                }
                else
                    SetConsoleTextAttribute(console_handle, ColorF | ColorB);
                bexit = 0;
                break;

            case '=':    // termcap
                break;

            default:
                bexit = 0;
                break;
        } // switch (c)

        n++;

    } // while(bexit)

    return n;
}

int _rsxnt_write_with_ansi(const char *buf, size_t size)
{
    int i, n;
    char c;
    DWORD ret;
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

    for (i = 0; i < size; i++) {
        c = buf[i];
        if (c == 27) {  // ESC
            n = rsxio_ansi(buf + i, size - i);
            if (n > 0) {
                i += n - 1;
                continue;
            }
        }
        else
            WriteFile(console_handle, &c, 1, &ret, NULL);
    }
    return size;
}

#endif

