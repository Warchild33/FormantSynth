#ifndef SVGWIDGET_H
#define SVGWIDGET_H

#include <QWidget>
#include <QMap>
#include <QString>
#include <QColor>
#include <QSvgRenderer>
#include <QPaintEvent>
#include <QDomDocument>

class SvgWidget : public QWidget
{
public:
    QDomDocument domDocument;
    QRectF       viewbox;
    QMap<QString, QDomElement> elementByID;
    QMap<QString, QDomElement> elementByName;
    QMap<QString,QColor>  colors_map;
    QMap<QString, QString> replaceMap;  //таблица замен цветов
    QSvgRenderer svg_rend;
    QString      svg_original;         // оригинальное SVG описание
    QString      svg;                  // измененное SVG-описание
    QStringList  id_list;
    QMap<QString, QList<int> > knownStringIndexs; //строки в которых уже делались замены
    explicit SvgWidget(QWidget *parent = 0);

//----методы работающие с SVG как с текстом
    void SvgLoad(QString svgPath);
    void SvgSave(QString svgFileM);
    void LoadOriginal();
    void doReplace();
    bool LoadRender();
    void paintEvent(QPaintEvent*);
    QPointF findPathCenter(QString id);
    QPainterPath getPath(QString id);
    QRectF getRect(QString id);
    QRectF getEllipse(QString id);
    void rotateNode(QString id, float degree);
    void rotateNode(QString id, QPointF center, float degree);
    void setText(QString group_id, QString text);
    QPointF  mapToViewbox(QPoint p, QRectF viewbox);
    QRectF   mapToViewbox(QRectF p, QRectF viewbox);
    QTransform getViewBoxTransform();

//----методы работающие с SVG как с DOM документом(медленнее)
    bool LoadDom(QString fn);
    bool SaveDom(QString fn);
    bool LoadRenderDOM();
    void setViewBox(const QDomNode& root);
    void setFill(QString id, QColor fill_color, float fill_opacity);
    void setStroke(QString id, QColor stroke_color);
    void TraverseXmlNode(const QDomNode& node);
    QString getAttr(QString id, QString attr);
    QString getSubAttr(QString attr_string, QString attr_key);
    QString changeAttr(QString attr_string, QString attr_key, QString attr_val);

};



#endif // SVGWIDGET_H
