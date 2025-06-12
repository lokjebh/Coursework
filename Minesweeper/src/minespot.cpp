#include "minespot.h"
#include <QDebug>

MineSpot::MineSpot(QWidget *parent) : QPushButton(parent)
{
    setFixedSize(26, 26);
    setIcon(QIcon("./icons/isCovered.png"));
    setIconSize(QSize(24, 24));
    // connect(this, &QPushButton::clicked, this, &MineSpot::openSpot);
}

void MineSpot::mousePressEvent(QMouseEvent *event)
{
    // LeftButton was pressed
    if (event->button() == Qt::LeftButton) 
    {
        if (!isFlagged) {
            emit uncovered(this);
        } else return;
        // if (isFlagged) return;
        // openSpot();
        // emit uncovered(this);
    }
    else if (event->button() == Qt::RightButton)
    {
        isFlagged = !isFlagged;
        updateAppearance();
        emit flagged(this);
    }
}

void MineSpot::openSpot()
{
    if (!isCovered)
        return;    // if uncovered, do nothing

    isCovered = false;

    updateAppearance();

    // emit uncovered(this);
}

void MineSpot::updateAppearance()
{
    if (isCovered)
    {
        if (isFlagged) {
            // setText("F");
            setIcon(QIcon("./icons/flagged.png"));
        } else {
            // setText("");
            setIcon(QIcon("./icons/isCovered.png"));
        }
        // setText("");
        setEnabled(true);
    } 
    else
    {
        // setEnabled(false);
        setAttribute(Qt::WA_TransparentForMouseEvents, true);
        if (isMine) {
            // setText("M");
            setIcon(QIcon("./icons/isMine.png"));
        } else if (adjMines > 0) {
            switch (adjMines)
            {
                case 1: 
                    setIcon(QIcon("./icons/isOne.png"));
                    break;
                case 2:
                    setIcon(QIcon("./icons/isTwo.png"));
                    break;
                case 3:
                    setIcon(QIcon("./icons/isThree.png"));
                    break;
                case 4:
                    setIcon(QIcon("./icons/isFour.png"));
                    break;
                case 5:
                    setIcon(QIcon("./icons/isFive.png"));
                    break;
                case 6:
                    setIcon(QIcon("./icons/isSix.png"));
                    break;
                case 7:
                    setIcon(QIcon("./icons/isSeven.png"));
                    break;
                case 8:
                    setIcon(QIcon("./icons/isEight.png"));
                    break;
            }
            // setText(QString());
        } else {
            // setText("");
            setIcon(QIcon("./icons/isEmpty.png"));
        }
    }

    // setIconSize(QSize(24, 24));
} 
