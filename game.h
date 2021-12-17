#ifndef GAME_H
#define GAME_H
#include "QThread"
#include <QtWidgets>
#include <QVector>
#include <QPoint>
#include <QDebug>
#include <QKeyEvent>
#include <QMediaPlayer>
class Game:public QWidget
{
public:
    Game();

protected:
    void timerEvent(QTimerEvent* ) override; //Таймер
    void keyPressEvent(QKeyEvent *event) override;//Нажатия
    void paintEvent(QPaintEvent *event) override; //Отрисовка
    void pushstd(); // Стандартный режим
    void pushnofield(); // Выбран режим без полей
    void startmenu(); // В начальное меню
    void pushfast(); //Быстрая змейка
    void repeat(); //Кнопка повторной игры

private:
    static const int DOT_WIDTH=40; //Ширина точки
    static const int DOT_HEIGHT=40; // Высота точки
    static const int FIELD_WIDTH=15; //Количество точек
    static const int FIELD_HEIGHT=16; //Количество точек
    static const int DELAY=100; //Задержка

    QPushButton *m_firstdif; //Кнопка для стандартной сложности
    QPushButton *m_fastsnake; //Быстрая змея
    QPushButton *m_nofield; //Без полей
    QPushButton *m_overleft; //Левая кнопка в конце игры
    QPushButton *m_overright; //Правая кнопка в конце игры
    QLabel *m_itog; //Итоговая надпись
    enum Directions // Возможные варианты перемещения змейки
    {
        left,right,up,down
    };
    bool m_startflag=false; //чтобы змейка стояла афк в начале игры
    bool m_inGame;//Состояние игры
    bool addlenflag=false; //Добавлять как змейка была отрисована, иначе будет баг с отрисовкой хвоста
    bool m_fieldflag=true;//Фалс если выбран режим без полей

    int m_fixdelay=0; //Если выбрана сложность x2 скорость он принимает значение 50
    int m_timerID; //Таймер
    int m_score=0; //Очки
    int m_maxscore=0; //Максимальные очки
    int m_creatbuttonornot=0; //Если кнопки были созданы не пересоздавать их
    int m_choseflag=0; //Выбор вариантов игры
    int m_onestepincadr=1; // 1 отрисовка в 1 кадр, изабвляет от самоукуса

    Directions m_dir; //Что нажато

    QMediaPlayer *eatplayer; //Звуки еды
    QMediaPlayer *Death; //Звук смерти

    QPoint m_apple; //Координаты яблока
    QVector<QPoint> m_dots; //Массив точек
    void hideoffstart(); //Прячет и выключат кнопки
    void showstart(); //Показывает  стартовые кнопки
    void initGame(); //Запускает игру
    void gameOver(); //Игра закончена
    void startmenuphone(); //стартовое меню
    void gameoverback(); //играконец
    void doDrawing(); //Отрисовка
    void chosediff(); //Выбор режима игры
    void chosenextway(); //
    void move(); //Движение
    void localApple(); //Яблочко
    void check_Apple(); //Если скушали яблоко
    void check_field(); //Проверка где находится змея, чтобы не выйти за границы поля и чтобы змея не погла пройти через себя


};

#endif // GAME_H
