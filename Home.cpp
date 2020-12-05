/*1. Используя функции SDL (SDL_RenderDraw... и SDL_RenderFill...) нарисовать:
 *         - поверхность земли
 *         - траву
 *         - дом с крышей, окном, дверью и трубой
 *         - на заднем плане - забор (опционально)
 * 2. Реализовать рисование окружности с использованием алгоритма Брезенхэма
 * 3. Нарисовать у домика круглое окно на чердаке.
 */
#include <iostream>
#include <memory>
/*
 * std::shared_ptr - разделяемый указатель
 * std::unique_ptr - уникальный указатель
 * std::weak_ptr - "слабый" указатель
 */
#include <SDL2/SDL_main.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
constexpr int WIDTH = 1000, HEIGHT = 720, R = 28;

std::shared_ptr<SDL_Texture> icon;
std::shared_ptr<SDL_Texture> icon2;

std::shared_ptr<SDL_Texture> Make_icon(SDL_Renderer *renderer, const char *name)
{

	std::shared_ptr<SDL_Texture> picture = std::shared_ptr<SDL_Texture>(
			IMG_LoadTexture(renderer, name),
			SDL_DestroyTexture);
	if(picture == nullptr)
	{
		std::cerr << "Не получается загрузить картинку" <<
		SDL_GetError()<<std::endl;
		return nullptr;
	}
	return picture;
}


void DrawCircle(SDL_Renderer * ren, int x, int y, int r, int X, int Y)
{
            int delta = 1 - 2 * r;
            int error = 0;
            SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);

            while(y >= 0)
            {
                SDL_RenderDrawPoint(ren, X + x, Y + y);
                SDL_RenderDrawPoint(ren, X + x, Y - y);
                SDL_RenderDrawPoint(ren, X - x, Y + y);
                SDL_RenderDrawPoint(ren, X - x, Y - y);
                error = 2 * (delta + y) - 1;
                if (delta < 0 and error <= 0)
                {
                    ++x;
                    delta += 2 * x + 1;
                    continue;
                }
                if (delta > 0 and error > 0)
                {
                    --y;
                    delta -= 2 * y + 1;
                    continue;
                }
                ++x;
                --y;
                delta += 2 * (x - y);
            }
}
int main(int, char **)
{
    std::cout << "Начало работы программы" << std::endl;
    SDL_Init(SDL_INIT_EVERYTHING);
    auto win = std::shared_ptr<SDL_Window>(
            SDL_CreateWindow("MY HOME",
                    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                    WIDTH, HEIGHT,
                    SDL_WINDOW_SHOWN),
            SDL_DestroyWindow);
    if (win == nullptr) {
        std::cerr << "Ошибка при создании окна: " <<
                SDL_GetError() << std::endl;
        return 1;
    }
    auto ren = std::shared_ptr<SDL_Renderer>(
            SDL_CreateRenderer(win.get(), -1,
                    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC),
            SDL_DestroyRenderer);
    if (ren == nullptr) {
        std::cerr << "Ошибка при создании рендерера: " <<
                SDL_GetError() << std::endl;
        return 1;
    }


	bool f = true;
	int icon_x = 800, icon_y = 50;// координаты, где появиться летающий
	bool f2 = true;
	int icon_x2 = 500, icon_y2 = 610;// координаты, где появится бегающий

	auto keys = SDL_GetKeyboardState(nullptr);

    SDL_Event event;
    bool finish = false;
    // Основной цикл программы
    while (not finish)
    {
        // Цикл обработки событий
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) // Если окно закрыли...
                finish = true;
        }
        //Фон
        SDL_SetRenderDrawColor(ren.get(), 200, 255, 250, 255);//Цвет в формате rgba
        SDL_RenderClear(ren.get());
        //Земля
        SDL_Rect r { 0, 525, WIDTH, 220 };
        SDL_SetRenderDrawColor(ren.get(), 28, 120, 19, 1);//Цвет в формате rgba
        SDL_RenderFillRect(ren.get(), &r);
        //забор
        for (int i = 3; i < WIDTH; i += 87) {
        	SDL_SetRenderDrawColor(ren.get(), 0, 26, 255, 1);// палки на заборе
            SDL_Rect r1 { i, 450, 50, 180 };
            SDL_RenderFillRect(ren.get(), &r1);
           SDL_Rect r2 { i + 50, 490, 50, 20 };
           SDL_RenderFillRect(ren.get(), &r2);
           SDL_SetRenderDrawColor(ren.get(), 245, 15, 65, 1);// треугольник на заборе
           for (double x1 = i + 25, x2 = i + 25,  y = 425; y < 450; --x1, ++x2, y++)
            {
            	//SDL_RenderDrawLine(ren.get(), x1, y, x2, y);
            }
        }
        //дом
        SDL_Rect r1 { WIDTH / 2 - 150, 200, 430, 430 };
        SDL_SetRenderDrawColor(ren.get(), 254, 155, 0, 255);//Цвет в формате rgba
        SDL_RenderFillRect(ren.get(), &r1);
        //Окно
        SDL_Rect r2 { WIDTH / 2 + 25, 200 + 150, 100, 100 };
        SDL_SetRenderDrawColor(ren.get(), 255, 238, 0, 1);//внутри //Цвет в формате rgba
        SDL_RenderFillRect(ren.get(), &r2);

        SDL_SetRenderDrawColor(ren.get(), 107, 75, 26, 1);// палочки //Цвет в формате rgba
        SDL_RenderDrawLine(ren.get(), WIDTH / 2 + 75, 350, WIDTH / 2 + 75, 450);

        SDL_SetRenderDrawColor(ren.get(), 107, 75, 26, 1);// палочки //Цвет в формате rgba
        SDL_RenderDrawLine(ren.get(), WIDTH / 2 + 25, 400, WIDTH / 2 + 125, 400);

        //Труба
        SDL_Rect r3 { WIDTH / 2 + 185, 130, 50, 60 };
        SDL_SetRenderDrawColor(ren.get(), 94, 79, 79, 1);//Цвет в формате rgba
        SDL_RenderFillRect(ren.get(), &r3);
        //Крыша
        SDL_SetRenderDrawColor(ren.get(), 255, 12, 0, 255);//Цвет в формате rgba
        for (double x1 = WIDTH / 2 + 65, x2 =WIDTH / 2 + 65,  y = 100; y < 200; x1 -= 2.15, x2 += 2.15, y++) {
            SDL_RenderDrawLine(ren.get(), x1, y, x2, y);
        }
        //дверь
        SDL_Rect r4 { WIDTH / 2 + 150 , 500, 75, 130 };
        SDL_SetRenderDrawColor(ren.get(), 94, 79, 79, 1);//Цвет в формате rgba
        SDL_RenderFillRect(ren.get(), &r4);
        //Трава
        SDL_SetRenderDrawColor(ren.get(), 71, 255, 27, 255);//Цвет в формате rgba
        for(int i = 0; i <= 15; i += 15)
            for (int x = 10 + i,  y1 = HEIGHT - i, y2 = HEIGHT - (i+50) ; x < WIDTH - 15; x += 50 )
                SDL_RenderDrawLine(ren.get(), x, y1, x, y2);

        //окружность
        DrawCircle(ren.get(), 0, R, R, WIDTH / 2 + 65, 150);


		//летающий орёл
		if (f)
			icon = Make_icon(ren.get(), "left.png");
		else
			icon = Make_icon(ren.get(), "right.png");

		if(keys[SDL_SCANCODE_RIGHT] and icon_x < WIDTH - 100){ icon_x += 8; f = true;}//тут и далее цифра 8 - это скорость полета
		if(keys[SDL_SCANCODE_LEFT] and icon_x > 0 ){ icon_x -= 8; f = false;}
		if(keys[SDL_SCANCODE_UP] and icon_y > 0){ icon_y -= 8; }
		if(keys[SDL_SCANCODE_DOWN] and icon_y < HEIGHT - 50){ icon_y += 8; }

		if(icon == nullptr) return 1;
		SDL_Rect icon_rect {icon_x, icon_y, 200, 200};//тут предпоследние 2 числа это  ширина и высота картинки твоей
		SDL_RenderCopy(ren.get(), icon.get(), nullptr, &icon_rect);

		//бегающая мышка
		if(icon_x2 < WIDTH - 150 and f2){
			icon2 = Make_icon(ren.get(), "mouse_r.png");
			icon_x2 += 8;//скорость бега вправо
		}
		else f2 = false;
		if(icon_x2 > 0 and !f2){
			icon2 = Make_icon(ren.get(), "mouse_l.png");
			icon_x2 -= 8;//скорость бега влево
		}
		else f2 = true;

		if(icon2 == nullptr) return 1;
		SDL_Rect icon_rect2 {icon_x2, icon_y2, 150, 150};//тут предпоследние 2 числа это  ширина и высота картинки твоей
		SDL_RenderCopy(ren.get(), icon2.get(), nullptr, &icon_rect2);

        SDL_RenderPresent(ren.get());
        }
        /*
         * Двойная буферизация:
         * 1 буфер (передний) - буфер, содержимое которого отображается.
         * 2 буфер (задний) - буфер, в котором осуществляется построение изображения.
         *
         */
        std::cout << "Конец работы программы" << std::endl;
        return 0;
}
