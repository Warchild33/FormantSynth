/**********************************************************************
 *  Project App6
 *
 *  print.cpp
 *
 *  Author: Vladimir Baranov
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
**********************************************************************/

#include <QWidget>
#include <QSignalMapper>
#include <stdio.h>
#include "print.h"

ConsolePrn cns;
char mu_printftmp[10000];

ConsolePrn::ConsolePrn()
{
    timer.restart();
}

void map_to_prn(QObject* w)
{
    QObject::connect(&cns, SIGNAL( print_sig(const QString &) ), w, SLOT(print(const QString &)) );
}


void unmap_from_prn(QObject* w)
{
    QObject::disconnect(&cns, SIGNAL( print_sig(const QString &) ), w, SLOT(print(const QString &)) );
}
void ConsolePrn::prn(char* str)
{ 
    QString s = QString(tr(str));
	emit print_sig(s); 
}

void ConsolePrn::prn_skip(char* str)
{
    if( timer.elapsed() > 500)
    {
        QString s = QString(tr(str));
        emit print_sig(s);
        timer.restart();
    }

}


void ConsolePrn::prn2(QString str)
{
    emit print_sig(str);
}

void prn2(QString str)
{
    cns.prn2(str);
}


void print(QString str)
{
    cns.prn2(str);
}

void prn(const char* fmt,...)
{
    va_list ap;

    va_start(ap, fmt);
    #ifdef WIN32
    _vsnprintf(mu_printftmp, 60000, fmt, ap);
    #else
    vsnprintf(mu_printftmp, 60000, fmt, ap);
    #endif
    va_end(ap);
    
	cns.prn(mu_printftmp);
}

void prn_skip(const char* fmt,...)
{
    va_list ap;

    va_start(ap, fmt);
    #ifdef WIN32
    _vsnprintf(mu_printftmp, 60000, fmt, ap);
    #else
    vsnprintf(mu_printftmp, 60000, fmt, ap);
    #endif
    va_end(ap);

    cns.prn_skip(mu_printftmp);


}
