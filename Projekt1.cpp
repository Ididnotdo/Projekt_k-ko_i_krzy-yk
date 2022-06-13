
#include <iostream>
#include <string>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>


using namespace std;

char Ruchy[3][3];

int ile_przegranych = 0;

typedef union ALLEGRO_EVENT ALLEGRO_EVENT;
typedef struct ALLEGRO_MOUSE_STATE ALLEGRO_MOUSE_STATE;
typedef struct ALLEGRO_FONT ALLEGRO_FONT;
ALLEGRO_MOUSE_STATE state;

ALLEGRO_FONT* font;
ALLEGRO_COLOR linie;
ALLEGRO_COLOR znaki;


void Kolko(int i, int j)
{
	al_draw_circle(i * 200 + 100, j * 200 + 100, 50, linie, 6);
	al_flip_display();
};

void Krzyzyk(int i, int j)
{
	al_draw_line((i * 200) + 65, (j * 200) + 65, i * 200 + 135, j * 200 + 135, linie, 6);
	al_draw_line(i * 200 + 65, j * 200 + 135, i * 200 + 135, j * 200 + 65, linie, 6);
	al_flip_display();
};

void Plansza()
{

	al_clear_to_color(al_map_rgb(255, 255, 255));
	al_draw_line(0, 200, 600, 200, linie, 6);
	al_draw_line(0, 400, 600, 400, linie, 6);
	al_draw_line(200, 0, 200, 600, linie, 6);
	al_draw_line(400, 0, 400, 600, linie, 6);
	al_flip_display();
};

void Plansza_wygrana_komp()
{
	al_clear_to_color(al_map_rgb(220, 172, 115));
	al_draw_text(font, znaki, 300, 300, ALLEGRO_ALIGN_CENTER, "Ha ha ha ha");
	al_flip_display();

}

void Plansza_wygrana_gracz()
{
	al_clear_to_color(al_map_rgb(220, 172, 115));
	al_draw_text(font, znaki, 300, 300, ALLEGRO_ALIGN_CENTER, "Musiales oszukac...");
	al_flip_display();

}
void Plansza_remis()
{
	al_clear_to_color(al_map_rgb(220, 172, 115));
	al_draw_text(font, znaki, 300, 300, ALLEGRO_ALIGN_CENTER, "Pfft... nie umiesz wygrac");
	al_flip_display();

}

bool Sprawdz_ruch_gracza(int x, int y)
{
	if (Ruchy[x][y] == 0)
		return true;
	else return false;
};

void Ruch_gracza(const ALLEGRO_EVENT& event)
{
	bool wykonany_ruch = false;
	int x, y, i = -1, j = -1;
	while (!wykonany_ruch)
	{
		if (event.mouse.button == 1)
		{
			x = event.mouse.x;
			y = event.mouse.y;
			if (x < 200)
				i = 0;
			else if (x < 400)
				i = 1;
			else if (x < 600)
				i = 2;
			if (y < 200)
				j = 0;
			else if (y < 400)
				j = 1;
			else if (y < 600)
				j = 2;
			if (Sprawdz_ruch_gracza(i, j))
			{
				Ruchy[i][j] = 'o';
				Kolko(i, j);
				wykonany_ruch = true;
			}
		}
		al_rest(0.02);
	};
};

void Zeruj()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Ruchy[i][j] = 0;
		}
	}
};

bool wygrana(char znak)
{
	int licznik = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (Ruchy[i][j] == znak)
				licznik++;
		}

		if (licznik == 3)
			return true;
		licznik = 0;
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (Ruchy[j][i] == znak)
				licznik++;
		}
		if (licznik == 3)
			return true;
		licznik = 0;
	}

	for (int i = 0; i < 3; i++)
	{
		if (Ruchy[i][i] == znak)
			licznik++;
	}

	if (licznik == 3)
		return true;
	licznik = 0;

	if (Ruchy[2][0] == znak) {
		licznik++;
		if (Ruchy[1][1] == znak) {
			licznik++;
			if (Ruchy[0][2] == znak)
				licznik++;
			if (licznik == 3)
				return true;
		}
	}
	return false;
};

bool Koniec_gry()
{
	if (wygrana('x') == true)
		return true;
	if (wygrana('o') == true)
		return true;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (Ruchy[i][j] == 0)
				return false;
		}
	}
	return true;
}

struct Wybrane_pole
{
	int x;
	int y;
};

int Ruch_komputera(char znak, int poziom)
{
	int rev, v;
	Wybrane_pole Pole;
	if (znak == 'x')
		rev = 2;
	else if (znak == 'o')
		rev = -2;
	char znak_przeciwny;
	if (znak == 'x')
		znak_przeciwny = 'o';
	else znak_przeciwny = 'x';
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (Ruchy[i][j] == 0)
			{
				Ruchy[i][j] = znak;
				if (wygrana(znak) == true)
				{
					if (znak == 'x')
						rev = -1;
					else if (znak == 'o')
						rev = 1;
					Pole.x = i;
					Pole.y = j;
					Ruchy[i][j] = 0;
					continue;
				}
				v = Ruch_komputera(znak_przeciwny, poziom + 1);
				Ruchy[i][j] = 0;
				if (znak == 'x' && v < rev)
				{
					rev = v;
					Pole.x = i;
					Pole.y = j;

				}
				else if (znak == 'o' && v > rev)
				{
					rev = v;
					Pole.x = i;
					Pole.y = j;

				}
			}
		}
	}
	if (rev == 2 || rev == -2)
		return 0;
	if (poziom == 0)
	{
		Krzyzyk(Pole.x, Pole.y);
		Ruchy[Pole.x][Pole.y] = znak;
	}
	return rev;
};


int main()
{

	if (!al_init())
		al_show_native_message_box(NULL, "Error", NULL, "Blad allegro", NULL, NULL);


	al_init_font_addon();
	al_init_ttf_addon();
	ALLEGRO_MOUSE_STATE state1;

	ALLEGRO_DISPLAY* display = al_create_display(600, 600);

	font = al_load_ttf_font("adler.ttf", 30, NULL);
	linie = al_map_rgb(217, 186, 146);
	znaki = al_map_rgb(0, 0, 0);

	al_set_window_title(display, "Kolko i krzyzyk");
	al_init_primitives_addon();
	al_install_mouse();
	al_init_font_addon();
	al_init_ttf_addon();


	ALLEGRO_EVENT events;
	ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_mouse_event_source());
	while (1)
	{
		Zeruj();
		Plansza();
		while (!Koniec_gry())
		{
			al_wait_for_event(event_queue, &events);
			switch (events.type)
			{
			case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
				Ruch_gracza(events);
				if (!Koniec_gry())
					Ruch_komputera('x', 0);
				break;

			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				al_destroy_font(font);
				al_destroy_display(display);
				al_destroy_event_queue(event_queue);
				return 0;
			}
		}

		if (wygrana('x'))
		{
			Plansza_wygrana_komp();
			al_rest(3.0);
			ile_przegranych++;
			ofstream file("ile_przegrales.txt");
			file << "Tyle razy przegrales: " << ile_przegranych << endl;
			file.close();
		}
		else if (wygrana('o'))
		{
			Plansza_wygrana_gracz();
			al_rest(2.0);
		}
		else {
			Plansza_remis();
			al_rest(2.0);
		}
	}
	al_destroy_font(font);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	return 0;
};