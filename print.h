#ifndef CONSOLE_H
#define CONSOLE_H

#include <QObject>
#include <QElapsedTimer>
#include <QString>

class ConsolePrn : public QObject
{
	Q_OBJECT
public:
    QElapsedTimer timer;
    int   nHooks;
    ConsolePrn();
    ConsolePrn(const ConsolePrn& cam):QObject()    { cam;    }

		void prn(char* str);
        void prn_skip(char* str);
        void prn2(QString str);
signals:
	void print_sig(const QString & str);
};

void init_prn();
void map_to_prn(QObject* w);
void prn(const char* fmt,...);
void prn_skip(const char* fmt,...);
#define mu_printf prn
void prn2(QString str);
void print(QString str);
void unmap_from_prn(QObject* w);

#endif
