#include "game.h"
#include <QPainter>
#include <QTime>
#include <QPixmap>
#include <QMessageBox>
#include <QLabel>
#include <fstream>
#include <QFile>
#include <QRectF>
Game::Game()
{
    eatplayer= new QMediaPlayer(this); eatplayer->setVolume(75);eatplayer->setMedia(QUrl("qrc:/audio/audio/eat.mp3"));
    Death= new QMediaPlayer(this); Death->setVolume(75);Death->setMedia(QUrl("qrc:/audio/audio/death.mp3"));
    this->resize(DOT_WIDTH*FIELD_WIDTH,DOT_HEIGHT*FIELD_HEIGHT); //Задаю размер поля
    this->setWindowTitle("Змейка");
    this->setFixedHeight(FIELD_HEIGHT*DOT_HEIGHT);
    this->setFixedWidth(FIELD_WIDTH*DOT_WIDTH);
    initGame();

}

void Game::timerEvent(QTimerEvent * e)
{
    Q_UNUSED(e)//Не нужная переменная
    if(m_inGame and m_startflag)
    {   Death->stop();
        m_onestepincadr+=1;
        move();
        check_Apple();
        check_field();
    }
    this->repaint();

}

void Game::keyPressEvent(QKeyEvent *e)//Обработчик нажатий клавиш
{   int key=e->key();//Проверка на одновременное нажатие противоположных клавиш условие для того чтобы змейка не могла поползти за границам при выбранном режиме and m_dots[0].y()<=15 and m_dots[0].y()>=1
    if((key==Qt::Key_Left or key==Qt::Key_A) && m_dir!=Directions::right  && m_onestepincadr>=1 and m_dots[0].y()<=15 and m_dots[0].y()>=1 ){m_dir=Directions::left;m_startflag=true;m_onestepincadr=0;}
    if((key==Qt::Key_Right or key==Qt::Key_D) && m_dir!=Directions::left && m_onestepincadr>=1 and m_dots[0].y()<=15 and m_dots[0].y()>=1){m_dir=Directions::right;m_startflag=true;m_onestepincadr=0;}
    if((key==Qt::Key_Up or key==Qt::Key_W) && m_dir!=Directions::down && m_onestepincadr>=1 and m_dots[0].x()<=14 and m_dots[0].x()>=0){m_dir=Directions::up;m_startflag=true;m_onestepincadr=0;}
    if((key==Qt::Key_Down or key==Qt::Key_S) && m_dir!=Directions::up && m_onestepincadr>=1 and m_dots[0].x()<=14 and m_dots[0].x()>=0){m_dir=Directions::down;m_startflag=true;m_onestepincadr=0;}
    if(key==Qt::Key_P)
    {
        if(m_timerID!=0)
        {
            killTimer(m_timerID);
            m_timerID = 0;
        }
        else
        {
           m_timerID=startTimer(DELAY-m_fixdelay);
        }
    }



}

void Game::paintEvent(QPaintEvent *event)
{
    const QRect & rect = event->rect();
    QPainter painter(this);
    painter.eraseRect(rect);
    painter.setRenderHint(QPainter::Antialiasing);
    if(m_choseflag==0)
    {
        startmenuphone();
    }
    else if(m_choseflag==1)
    {
        doDrawing();
    }
    else if(m_choseflag==2)
    {
        gameoverback();
    }


}

void Game::initGame() //Иницирует игру
{
    if(m_choseflag==0)
    {
        chosediff();
    }
    else if(m_choseflag==1){
        //Отрисовка поля
        m_inGame = true; //Игра запущена
        m_dir=right; //Расположение змейки


        m_dots.resize(3); //Задаем изначальные р азмеры змейки
        for(int i=0; i<m_dots.size();i++){
            m_dots[i].setX(m_dots.size()-i-1); //Задаём координаты
            m_dots[i].setY(7);
        }
        localApple();
        m_timerID=startTimer(DELAY-m_fixdelay);
    }


}






void Game::doDrawing()
{

    QPainter qp(this);
    QImage back1(":/new/prefix1/png/back-1");
    QImage back2(":/new/prefix1/png/back-2");
    QImage Phone(":/new/prefix1/png/Phone");
    for(int i=0;i<width()/DOT_WIDTH+1;i++)
    {   for(int j=0;j<height()/DOT_HEIGHT+1;j++){
            if((i+j)%2==0){
                qp.drawImage(QRect(i*DOT_WIDTH,j*DOT_HEIGHT,DOT_WIDTH,DOT_HEIGHT),back1);

            }
            else{
                qp.drawImage(QRect(i*DOT_WIDTH,j*DOT_HEIGHT,DOT_WIDTH,DOT_HEIGHT),back2);

            }
        }
    }
    if(m_inGame)

    {
        QImage apple(":/new/prefix1/png/apple");
        qp.drawImage(QRect(m_apple.x()*DOT_WIDTH,m_apple.y()*DOT_HEIGHT,DOT_WIDTH,DOT_HEIGHT), apple);

        for(int i=0;i<m_dots.size();i++)
        {
            if(i==0)
            {  //Голова
                switch (m_dir) {
                    case right:{
                            QImage head(":/new/prefix1/png/head_right");
                            //QImage head(":/new/prefix1/png/3"); -5;-10;60;60
                            qp.drawImage(QRect(m_dots[i].x()*DOT_WIDTH,m_dots[i].y()*DOT_HEIGHT,DOT_WIDTH,DOT_HEIGHT),head);; //Координаты
                            break;
                         }
                    case left:{
                            QImage head(":/new/prefix1/png/head_left");
                            qp.drawImage(QRect(m_dots[i].x()*DOT_WIDTH,m_dots[i].y()*DOT_HEIGHT,DOT_WIDTH,DOT_HEIGHT),head);; //Координаты
                            break;
                        }
                    case up:{
                            QImage head(":/new/prefix1/png/head_up");
                            qp.drawImage(QRect(m_dots[i].x()*DOT_WIDTH,m_dots[i].y()*DOT_HEIGHT,DOT_WIDTH,DOT_HEIGHT),head);; //Координаты
                            break;
                        }
                    case down:{
                            QImage head(":/new/prefix1/png/head_down");
                            qp.drawImage(QRect(m_dots[i].x()*DOT_WIDTH,m_dots[i].y()*DOT_HEIGHT,DOT_WIDTH,DOT_HEIGHT),head);; //Координаты
                            break;

                        }
                }
            }
            else if(i==m_dots.size()-1)
            {   //конец змеи
                int lastindex=m_dots.size();
                if( (m_dots[lastindex-1].y()==m_dots[lastindex-2].y() and m_dots[lastindex-2].x()-m_dots[lastindex-1].x()>0) or m_dots[lastindex-1]==m_dots[lastindex-2])
                {
                            QImage tail(":/new/prefix1/png/tail_left");
                            qp.drawImage(QRect(m_dots[i].x()*DOT_WIDTH,m_dots[i].y()*DOT_HEIGHT,DOT_WIDTH,DOT_HEIGHT),tail);; //Координаты
                        }
                else if( m_dots[lastindex-1].y()==m_dots[lastindex-2].y() and m_dots[lastindex-2].x()-m_dots[lastindex-1].x()<0)
                {
                            QImage tail(":/new/prefix1/png/tail_right");
                            qp.drawImage(QRect(m_dots[i].x()*DOT_WIDTH,m_dots[i].y()*DOT_HEIGHT,DOT_WIDTH,DOT_HEIGHT),tail);; //Координаты
                        }
                else if( m_dots[lastindex-1].x()==m_dots[lastindex-2].x() and m_dots[lastindex-2].y()-m_dots[lastindex-1].y()<0)
                {
                            QImage tail(":/new/prefix1/png/tail_down");
                            qp.drawImage(QRect(m_dots[i].x()*DOT_WIDTH,m_dots[i].y()*DOT_HEIGHT,DOT_WIDTH,DOT_HEIGHT),tail);; //Координаты
                        }
                else if(m_dots[lastindex-1].x()==m_dots[lastindex-2].x() and m_dots[lastindex-2].y()-m_dots[lastindex-1].y()>0)
                {
                            QImage tail(":/new/prefix1/png/tail_up");
                            qp.drawImage(QRect(m_dots[i].x()*DOT_WIDTH,m_dots[i].y()*DOT_HEIGHT,DOT_WIDTH,DOT_HEIGHT),tail);; //Координаты

                        }

            }
            else if(i!=m_dots.size()-1)
            {   // тело змеи

                if (m_dots[i-1].x()==m_dots[i].x() and m_dots[i].x()==m_dots[i+1].x() )
                {
                            QImage body(":/new/prefix1/png/body_vertical");
                            qp.drawImage(QRect(m_dots[i].x()*DOT_WIDTH,m_dots[i].y()*DOT_HEIGHT,DOT_WIDTH,DOT_HEIGHT),body);; //Координаты
                         }
                else if(m_dots[i-1].y()==m_dots[i].y() and m_dots[i+1].y()==m_dots[i].y())
                {
                            QImage body(":/new/prefix1/png/body_horizontal");
                            qp.drawImage(QRect(m_dots[i].x()*DOT_WIDTH,m_dots[i].y()*DOT_HEIGHT,DOT_WIDTH,DOT_HEIGHT),body);; //Координаты
                        }
                else if((m_dots[i-1].x()!=m_dots[i+1].x() and (m_dots[i+1].x()-m_dots[i].x())<0 and m_dots[i].y()<m_dots[i-1].y()) or (m_dots[i-1].x()!=m_dots[i+1].x() and (m_dots[i-1].x()-m_dots[i].x())<0 and m_dots[i].y()<m_dots[i+1].y()))
                {
                            QImage body(":/new/prefix1/png/body_bottomleft");
                            qp.drawImage(QRect(m_dots[i].x()*DOT_WIDTH,m_dots[i].y()*DOT_HEIGHT,DOT_WIDTH,DOT_HEIGHT),body);; //Координаты
                        }
                else if((m_dots[i-1].x()!=m_dots[i+1].x() and (m_dots[i+1].x()-m_dots[i].x())<0 and m_dots[i-1].y()<m_dots[i].y()) or (m_dots[i-1].x()!=m_dots[i+1].x() and (m_dots[i-1].x()-m_dots[i].x())<0 and m_dots[i+1].y()<m_dots[i].y()) )
                {
                            QImage body(":/new/prefix1/png/body_topleft");
                            qp.drawImage(QRect(m_dots[i].x()*DOT_WIDTH,m_dots[i].y()*DOT_HEIGHT,DOT_WIDTH,DOT_HEIGHT),body);; //Координаты
                        }
                else if((m_dots[i-1].x()!=m_dots[i+1].x() and (m_dots[i+1].y()<m_dots[i].y()) and (m_dots[i-1].x()-m_dots[i].x())>0) or(m_dots[i-1].x()!=m_dots[i+1].x() and (m_dots[i-1].y()<m_dots[i].y()) and (m_dots[i+1].x()-m_dots[i].x())>0) )
                {
                            QImage body(":/new/prefix1/png/body_topright");
                            qp.drawImage(QRect(m_dots[i].x()*DOT_WIDTH,m_dots[i].y()*DOT_HEIGHT,DOT_WIDTH,DOT_HEIGHT),body);; //Координаты
                        }
                else if((m_dots[i-1].x()!=m_dots[i+1].x() and m_dots[i].y()<m_dots[i+1].y() and (m_dots[i-1].x()-m_dots[i].x()>0)) or (m_dots[i-1].x()!=m_dots[i+1].x() and m_dots[i].y()<m_dots[i-1].y() and (m_dots[i+1].x()-m_dots[i].x()>0)) )
                {
                            QImage body(":/new/prefix1/png/body_bottomright");
                            qp.drawImage(QRect(m_dots[i].x()*DOT_WIDTH,m_dots[i].y()*DOT_HEIGHT,DOT_WIDTH,DOT_HEIGHT),body);; //Координаты
                        }
                }



        }
        if(addlenflag)
        {
            m_dots.push_back(m_dots[m_dots.size()-1]);
            addlenflag=false;
        }
        //Цвет яблочка

        for(int i=0;i<width()/DOT_WIDTH+1;i++)
        {
            for(int j=0;j<height()/DOT_HEIGHT+1;j++){
                if(j==0)
                {
                    qp.drawImage(QRect(i*DOT_WIDTH,j*DOT_HEIGHT,DOT_WIDTH,DOT_HEIGHT),Phone);
                    break;
                }

            }
        }
        qp.drawImage(QRect(0*DOT_WIDTH,0*DOT_HEIGHT,DOT_WIDTH,DOT_HEIGHT), apple);
        QPainter painter(this);
        painter.setPen(Qt::gray);
        painter.setFont((QFont("Kameron",25)));
        painter.drawText(1*DOT_WIDTH, 0.9*DOT_HEIGHT, QString::number(m_score));
        if(m_maxscore>0){
            QImage cup(":/new/prefix1/png/cup");
            qp.drawImage(QRect(3*DOT_WIDTH,0*DOT_HEIGHT,36,37), cup);
            painter.drawText(4*DOT_WIDTH, 0.9*DOT_HEIGHT, QString::number(m_maxscore));


        }

    }
    else
    {
        gameOver();
    }
}




void Game::move()
{
    for(int i=m_dots.size()-1;i>0;--i)//Цикл для перемещения змейки
    {
        m_dots[i]=m_dots[i-1];
    }

    switch (m_dir) //Перемещение при нажатых клавишах
    {
    case left:{m_dots[0].rx()-=1;break;}
    case right:{m_dots[0].rx()+=1;break;}
    case up:{m_dots[0].ry()-=1;break;}
    case down:{m_dots[0].ry()+=1;break;}
    }


}

void Game::localApple()
{   QTime time= QTime::currentTime(); //Создаём таймер
    qsrand((uint) time.msec()); //Задаём мили секунды
    int x;
    int y;
    bool flag;
    while(true)
    {
      flag=true;
      x=(qrand()%(width()/DOT_WIDTH));
      y=((qrand()+1)%(height()/DOT_HEIGHT));
      for(int i=0;i<m_dots.size()-1;i++)
      {
        if(x==m_dots[i].x() and y==m_dots[i].y())
        {
            flag=false;
            break;
        }
      }
      if(flag)
      {
          break;
      }
    }
//    x=(qrand()%(width()/DOT_WIDTH));
//    y=(qrand()%(height()/DOT_HEIGHT));
    if(y<1){
        y=1;
    }
    m_apple.setX(x); //Ограничение точек по х
    m_apple.setY(y);//Ограничение точек по y
}
//Проверка яблока
void Game::check_Apple()
{
    for(int i=1;i<m_dots.size()-2;i++){
        if(m_apple==m_dots[i]){
            localApple();
        }
    }
    if(m_apple==m_dots[0])
    {
        m_score+=1;
        addlenflag=true;
        if(eatplayer->state()==QMediaPlayer::PlayingState){
            eatplayer->setPosition(0);
        }
        else if(eatplayer->state()==QMediaPlayer::StoppedState){
            eatplayer->play();
        }
        localApple();
    }

}
void Game::chosediff()
{
    if(m_creatbuttonornot==0)
    {
        QFile file(":/css/buttonstd.css");
        QFile file1(":/css/buttonspeed.css");
        QFile file2(":/css/buttonfield.css");
        m_firstdif = new QPushButton("Стандартный", this); m_firstdif->setGeometry(6.25*DOT_WIDTH,11*DOT_HEIGHT,100,70);  file1.open(QFile::ReadOnly); m_firstdif->setStyleSheet(file1.readAll()); file1.close();
        m_fastsnake = new QPushButton("Быстрый", this); m_fastsnake->setGeometry(9.5*DOT_WIDTH,11*DOT_HEIGHT,100,70);  file2.open(QFile::ReadOnly); m_fastsnake->setStyleSheet(file2.readAll()); file2.close();
        m_nofield = new QPushButton("Без полей", this); m_nofield->setGeometry(3*DOT_WIDTH,11*DOT_HEIGHT,100,70);  file.open(QFile::ReadOnly); m_nofield->setStyleSheet(file.readAll());file.close();
        connect(m_firstdif,  &QPushButton::released, this, &Game::pushstd);
        connect(m_fastsnake,  &QPushButton::released, this, &Game::pushfast);
        connect(m_nofield,  &QPushButton::released, this, &Game::pushnofield);
        m_creatbuttonornot+=1;

    }
    else
    {
        showstart();
    }



}

void Game::chosenextway()
{   QFont f( "Arial", 15);
    QString s;
    if(m_score>=222)
    {
      s=" и победили!";
    }
    else
    {
        s="";
    }
    if(m_creatbuttonornot==1)
    {
        QFile file(":/css/overleft.css");
        m_overleft = new QPushButton("В главное меню", this); m_overleft->setGeometry(0.5*DOT_WIDTH,13*DOT_HEIGHT,120,70);  file.open(QFile::ReadOnly); m_overleft->setStyleSheet(file.readAll());file.close();m_overleft->show();
        m_overright = new QPushButton("Повторить", this); m_overright->setGeometry(11.5*DOT_WIDTH,13*DOT_HEIGHT,120,70);  file.open(QFile::ReadOnly); m_overright->setStyleSheet(file.readAll());file.close();m_overright->show();
        m_itog= new QLabel("Вы набрали: "+QString::number(m_score)+s+"\n"+"Максимальный результат: "+QString::number(m_maxscore),this);m_itog->setFont(f);m_itog->setStyleSheet("QLabel { background-color : black; color : white;}");m_itog->setGeometry(4.2*DOT_WIDTH,12.5*DOT_HEIGHT,280,100);m_itog->show();
        connect(m_overleft,  &QPushButton::released, this, &Game::startmenu);
        connect(m_overright,  &QPushButton::released, this, &Game::repeat);
        m_creatbuttonornot+=1;


    }
    else
    {   delete m_itog;
        m_itog= new QLabel("Вы набрали: "+QString::number(m_score)+s+"\n"+"Максимальный результат: "+QString::number(m_maxscore),this);m_itog->setFont(f);m_itog->setStyleSheet("QLabel { background-color : black; color : white;}");m_itog->setGeometry(4.2*DOT_WIDTH,12.5*DOT_HEIGHT,280,100);m_itog->show();
        m_overleft->show(); m_overleft->setEnabled(true);
        m_overright->show(); m_overright->setEnabled(true);
    }


}

void Game::startmenuphone()
{

    QPainter back(this);
    QImage back3(":/new/prefix1/png/back-3.png");
    for(int i=0;i<FIELD_WIDTH;i++){
        for(int j=0;j<FIELD_HEIGHT;j++){
            back.drawImage(QRect(i*DOT_HEIGHT,j*DOT_WIDTH,61,55),back3);
        }
    }
    QImage p(":/back/background2.jpg");
    back.drawImage(QRect(0*DOT_WIDTH,0*DOT_HEIGHT,600,700),p);
    QImage d(":/new/prefix1/png/Dif4.png");
    back.drawImage(QRect(1*DOT_WIDTH,-0.5*DOT_HEIGHT,500,150),d);
}

void Game::gameoverback()
{
        QPainter back(this);
        QImage back4(":/new/prefix1/png/back-4.png");
        for(int i=0;i<FIELD_WIDTH;i++){
            for(int j=0;j<FIELD_HEIGHT;j++){
                back.drawImage(QRect(i*DOT_HEIGHT,j*DOT_WIDTH,63,65),back4);
            }
        }
        QImage back1(":/back/gameover.png");
        back.drawImage(QRect(0*DOT_WIDTH,1*DOT_HEIGHT,600,600),back1);
        QImage game(":/new/prefix1/png/Game1.png");
        back.drawImage(QRect(0.37*DOT_WIDTH,0.5*DOT_HEIGHT,280,115),game);
        QImage over(":/new/prefix1/png/Over2.png");
        back.drawImage(QRect(7.8*DOT_WIDTH,0.6*DOT_HEIGHT,280,115),over);

}
void Game::gameOver()
{   Death->play();
    if(m_maxscore<m_score)
    {
        m_maxscore=m_score;
    }
    m_onestepincadr=1;
    m_choseflag=2;
    m_startflag=false;
    gameoverback();
    chosenextway();
}
void Game::repeat()
{   m_choseflag=1;
    m_score=0;
    m_overleft->hide();m_overleft->setEnabled(false);
    m_overright->hide();m_overright->setEnabled(false);
    m_itog->hide();
    this->update();
    initGame();
}
void Game::startmenu()
{
    m_overleft->hide();m_overleft->setEnabled(false);
    m_overright->hide();m_overright->setEnabled(false);
    m_itog->hide();
    m_fieldflag=true;
    m_score=0;
    m_fixdelay=0;
    m_choseflag=0;
    m_maxscore=0;
    showstart();
    this->update();
    initGame();
}
void Game::pushstd()
{
    m_choseflag=1;
    hideoffstart();
    initGame();
    this->update();

}

void Game::pushnofield()
{
    m_choseflag=1;
    m_fieldflag=false;
    hideoffstart();
    initGame();
    this->update();

}



void Game::pushfast()
{
    m_choseflag=1;
    m_fixdelay=50;
    hideoffstart();
    initGame();
    this->update();

}


//Проверка поля
void Game::check_field()
{   //Змейка укусила себя
    if(m_dots.size()>=4)
    {
        for(int i=1;i<m_dots.size();++i)
        {
            if(m_dots[0]==m_dots[i])
            {
                m_inGame=false;
            }

        }
    }

    //Вышли за границы
    //qDebug()<<width()/DOT_WIDTH<<height()/DOT_HEIGHT;
    if(m_fieldflag)
    {
        if(m_dots[0].x() >=FIELD_WIDTH)  { m_inGame=false; }
        if(m_dots[0].x()<0)             { m_inGame=false; }
        if(m_dots[0].y()>=FIELD_HEIGHT) { m_inGame=false; }
        if(m_dots[0].y()<1)             { m_inGame=false; }
    }
    else
    {
        if(m_dots[0].x()>=16)
        {
            m_dots[0].rx()=-1;
        }
        else if(m_dots[0].x()<=-2)
        {
           m_dots[0].rx()=16;
        }
        else if(m_dots[0].y()<=-1)
        {
            m_dots[0].ry()=16;
        }
        else if(m_dots[0].y()>=17)
        {
            m_dots[0].ry()=0;
        }
    }


    if(!m_inGame)
    {
        killTimer(m_timerID); //Останавливаем игру
    }
}


void Game::showstart()
{
    m_firstdif->setEnabled(true);m_firstdif->show();
    m_fastsnake->setEnabled(true);m_fastsnake->show();
    m_nofield->setEnabled(true);m_nofield->show();
}
void Game::hideoffstart()
{
    m_firstdif->setEnabled(false);  m_firstdif->hide();
    m_fastsnake->setEnabled(false); m_fastsnake->hide();
    m_nofield->setEnabled(false); m_nofield->hide();
}
