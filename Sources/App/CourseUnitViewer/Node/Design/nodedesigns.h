#pragma once

#define NODE_RAD 50
#define NODE_SHADOW_SHIFT (NODE_RAD / 20 * 3)
#define MAX_SYMBOLS_PER_LINE 4
#define RECTS_PER_NAME 5

class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;
class Node;

typedef void (*nodeDesign)(Node * nd, QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

void designOlive(Node * nd, QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
void designFormal(Node * nd, QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
