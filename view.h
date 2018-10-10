#include <SFML\Graphics.hpp>

sf::View view; // Камера

void getPlayerCoordinateForview(float x,float y) { //функция для считывания координат игрока


    view.setCenter(x, y); //следим за игроком, передавая его координаты камере. +100 - сместили камеру по иксу вправо. эксперементируйте


}

void changeview() {


    if (sf::Keyboard::isKeyPressed(sf::Keyboard::U)) {
        view.zoom(1.0100f); //масштабируем, уменьшение
        //view.zoom(1.0006f); //тоже самое помедленнее соответственно
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
        //view.setRotation(90);//сразу же задает поворот камере
        view.rotate(1);//постепенно поворачивает камеру (отрицательное значение - в обратную сторону)
    }


    if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) {
        view.setSize(966, 668);//устанавливает размер камеры (наш исходный)
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
        view.setSize(1366, 768);//например другой размер
    }


    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        view.setViewport(sf::FloatRect(0, 0, 0.5f,
                                       1));//таким образом делается раздельный экран для игры на двоих. нужно только создать ещё один объект View и привязывать к нему координаты игрока 2.
    }
}

