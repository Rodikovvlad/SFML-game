#include <SFML/Graphics.hpp>
#include <iostream>
#include "map.h"
#include "view.h"
#include <sstream>
using namespace std;


sf::RenderWindow window;
sf::RectangleShape rect2(sf::Vector2f(10, 100));
sf::RectangleShape rect3(sf::Vector2f(10, 100));
sf::CircleShape circle(30, 30);



class Sprites {
    private: float xcor, ycor;
    public:
        sf::Image img;
        sf::Texture text;
        sf::Sprite sprite;
        string file;
        float speed, CurrentFrame, width, height;
        int heroDir, points, health;
        bool life;
        Sprites(string f, int x, int y, float W, float H) {
            points = 0; speed = 0.1; CurrentFrame = 0;
            health = 100; life = true;
            file = f; // Название картинки
            img.loadFromFile(file);
            text.loadFromImage(img);
            sprite.setTexture(text);
            xcor = x;
            ycor = y;
            sprite.setPosition(x,y);
            width = W; // Картинка по умолчанию
            height = H;
            sprite.setTextureRect(sf::IntRect(0, 0, W, H));
        }


    void checkPress(float time) {
        if (life) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                if (xcor > 32) {
                    heroDir = 0;
                    CurrentFrame += 0.005 * time;
                    if (CurrentFrame > 4) CurrentFrame -= 4;
                    sprite.setTextureRect(sf::IntRect(int(CurrentFrame) * 64, 96, 64, 96));
                    sprite.move(-speed * time, 0);
                    xcor -= speed * time;
                    getPlayerCoordinateForview(getPlayerCoordX(),
                                               getPlayerCoordY());//передаем координаты игрока в функцию управления камерой
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                if (xcor < 900 - 32) {
                    heroDir = 1;
                    CurrentFrame += 0.005 * time;
                    if (CurrentFrame > 4) CurrentFrame -= 4;
                    sprite.setTextureRect(sf::IntRect(int(CurrentFrame) * 64, 192, 64, 96));
                    sprite.move(speed * time, 0);
                    xcor += speed * time;
                    getPlayerCoordinateForview(getPlayerCoordX(),
                                               getPlayerCoordY());//передаем координаты игрока в функцию управления камерой
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                if (ycor > 32) {
                    heroDir = 2;
                    CurrentFrame += 0.005 * time;
                    if (CurrentFrame > 4) CurrentFrame -= 4;
                    sprite.setTextureRect(sf::IntRect(int(CurrentFrame) * 64, 288, 64, 96));
                    sprite.move(0, -speed * time);
                    ycor -= speed * time;
                    getPlayerCoordinateForview(getPlayerCoordX(),
                                               getPlayerCoordY());//передаем координаты игрока в функцию управления камерой
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                if (ycor < 668) {
                    heroDir = 3;
                    CurrentFrame += 0.005 * time;
                    if (CurrentFrame > 4) CurrentFrame -= 4;
                    sprite.setTextureRect(sf::IntRect(int(CurrentFrame) * 64, 0, 64, 96));
                    sprite.move(0, speed * time);
                    ycor += speed * time;
                    getPlayerCoordinateForview(getPlayerCoordX(),
                                               getPlayerCoordY());//передаем координаты игрока в функцию управления камерой
                }
            }
        }
            if (health < 0) life = false;

    }


    void checkCollis(float time) {
        sf::FloatRect spriteBounds = sprite.getGlobalBounds(); // Изымаем данные персонажа
        sf::FloatRect rect2Bounds = rect2.getGlobalBounds();
        sf::FloatRect rect3Bounds = rect3.getGlobalBounds();
        sf::FloatRect circleBounds = circle.getGlobalBounds();

        if (spriteBounds.intersects(rect2Bounds)) {
            if (heroDir == 1) {
                sprite.setPosition(500, 100);
                xcor = 500.0;
                ycor = 100.0;
            }
        }
        if (spriteBounds.intersects(rect3Bounds)) {
            if (heroDir == 0) {
                sprite.setPosition(150, 100);
                xcor = 150.0;
                ycor = 100.0;
            }
        }
        if (spriteBounds.intersects(circleBounds)) {
            switch (heroDir) {
                case 0: {
                    circle.move(-speed * time, 0);
                    break;
                }
                case 1: {
                    circle.move(speed * time, 0);
                    break;
                }
                case 2: {
                    circle.move(0, -speed * time);
                    break;
                }
                case 3: {
                    circle.move(0, speed * time);
                    break;
                }


            }
        }
    }

    void interactionWithMap()//ф-ция взаимодействия с картой
    {

        for (int i = ycor / 32; i < (ycor + height) / 32; i++)//проходимся по тайликам, контактирующим с игроком, то есть по всем квадратикам размера 32*32, которые мы окрашивали в 9 уроке. про условия читайте ниже.
            for (int j = xcor / 32; j<(xcor + width) / 32; j++)//икс делим на 32, тем самым получаем левый квадратик, с которым персонаж соприкасается. (он ведь больше размера 32*32, поэтому может одновременно стоять на нескольких квадратах). А j<(x + w) / 32 - условие ограничения координат по иксу. то есть координата самого правого квадрата, который соприкасается с персонажем. таким образом идем в цикле слева направо по иксу, проходя по от левого квадрата (соприкасающегося с героем), до правого квадрата (соприкасающегося с героем)
            {

                if (TileMap[i][j] == 's') { //если символ равен 's' (камень)
                    points++; //какое то действие... например телепортация героя
                    TileMap[i][j] = ' ';//убираем камень, типа взяли бонус. можем и не убирать, кстати.
                }
                if (TileMap[i][j] == 'f') {
                    health-=40;//если взяли ядовитейший в мире цветок,то переменная health=health-40;
                    TileMap[i][j] = ' ';//убрали цветок
                }

                if (TileMap[i][j] == 'h') {
                    health += 20;//если взяли сердечко,то переменная health=health+20;
                    TileMap[i][j] = ' ';//убрали сердечко
                }

            }
    }



    float getPlayerCoordX() { // Взять координату X спрайта
        return xcor;
        }

    float getPlayerCoordY() { // Взять координату Y спрайта
        return ycor;
        }

};

void drawMap(sf::Sprite s_map) {
    for (int i = 0; i < HEIGHT_MAP; i++)
        for (int j = 0; j < WIDTH_MAP; j++)
        {
            if (TileMap[i][j] == ' ')  s_map.setTextureRect(sf::IntRect(0, 0, 32, 32)); //если встретили символ пробел, то рисуем 1й квадратик
            if (TileMap[i][j] == 's')  s_map.setTextureRect(sf::IntRect(32, 0, 32, 32));//если встретили символ s, то рисуем 2й квадратик
            if ((TileMap[i][j] == '0')) s_map.setTextureRect(sf::IntRect(64, 0, 32, 32));//если встретили символ 0, то рисуем 3й квадратик
            if ((TileMap[i][j] == 'f')) s_map.setTextureRect(sf::IntRect(96, 0, 32, 32));//добавили цветок
            if ((TileMap[i][j] == 'h')) s_map.setTextureRect(sf::IntRect(128, 0, 32, 32));//и сердечко
            s_map.setPosition(j * 32, i * 32);//по сути раскидывает квадратики, превращая в карту. то есть задает каждому из них позицию. если убрать, то вся карта нарисуется в одном квадрате 32*32 и мы увидим один квадрат

            window.draw(s_map);//рисуем квадратики на экран
        }
}


int main()
{
    // Создаем окно
    window.create(sf::VideoMode(966, 668), "SFML works!");
    window.setFramerateLimit(60); // fps

    view.reset(sf::FloatRect(0, 0, 966, 668));//размер "вида" камеры при создании объекта вида камеры. (потом можем менять как хотим) Что то типа инициализации

    sf::Font font;//шрифт
    font.loadFromFile("CyrilicOld.TTF");//передаем нашему шрифту файл шрифта
    sf::Text sentens("", font, 30);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
    sf::Text point("", font, 30);
    sf::Text timer("", font, 40);
    sentens.setStyle(sf::Text::Bold | sf::Text::Underlined);//жирный и подчеркнутый текст. по умолчанию он "худой":)) и не подчеркнутый
    point.setStyle(sf::Text::Bold | sf::Text::Underlined);
    timer.setStyle(sf::Text::Bold | sf::Text::Underlined);

    sf::Image map_image;//объект изображения для карты
    map_image.loadFromFile("map.png");//загружаем файл для карты
    sf::Texture map;//текстура карты
    map.loadFromImage(map_image);//заряжаем текстуру картинкой
    sf::Sprite s_map;//создаём спрайт для карты
    s_map.setTexture(map);//заливаем текстуру спрайтом

    // Настройки прямоугольника
    rect2.setFillColor(sf::Color::Red);
    rect2.setPosition(200,100);
    rect3.setFillColor(sf::Color::Red);
    rect3.setPosition(500,100);

    // Настройки кргуа
    circle.setFillColor(sf::Color::Blue);
    circle.setPosition(250, 300);

    //Загрузка и настройка картинки
    Sprites sprt1("sprite.png", 100, 300, 64.0, 96.0);

    // Время для процессора
    sf::Clock clock;
    sf::Clock timing;
    getPlayerCoordinateForview(sprt1.getPlayerCoordX(), sprt1.getPlayerCoordY());

    // Пока окно открыто
    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds();
        int vremya = timing.getElapsedTime().asSeconds();
        clock.restart();
        time /= 800;

        sf::Event event;

        // Проверка событий
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Проверка нажатий
        sprt1.checkPress(time);
        // Проверка коллизий
        sprt1.checkCollis(time);
        // Взаимодействие с объектами карты
        sprt1.interactionWithMap();
        changeview();
        window.setView(view);
        window.clear();
        drawMap(s_map);
        window.draw(rect2);
        window.draw(rect3);
        window.draw(circle);
        ostringstream playerScoreString, playerTimeString;    // объявили переменную
        playerScoreString << sprt1.points;		//занесли в нее число очков, то есть формируем строку
        sentens.setString(L"Собрано камней:");//задаем строку тексту
        point.setString(playerScoreString.str());
        playerTimeString << vremya;
        timer.setString(playerTimeString.str());
        sentens.setPosition(view.getCenter().x - 350, view.getCenter().y - 300);//задаем позицию текста, отступая от центра камеры
        point.setPosition(view.getCenter().x - 130, view.getCenter().y - 300);
        timer.setPosition(200, 200);
        window.draw(sentens);//рисую этот текст
        window.draw(point);
        window.draw(timer);
        window.draw(sprt1.sprite);
        window.display();
    }

    return 0;
}