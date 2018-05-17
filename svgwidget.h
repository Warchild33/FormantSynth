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
    QMap<QString, QDomElement> elementByID;
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

//----методы работающие с SVG как с DOM документом(медленнее)
    bool LoadDom(QString fn);
    bool SaveDom(QString fn);
    bool LoadRenderDOM();
    void setPathStyleDOM(QString id, QColor fill_color, float fill_opacity);
    void TraverseXmlNode(const QDomNode& node);

};

#endif // SVGWIDGET_H
