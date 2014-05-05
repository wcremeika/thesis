#include "version.h"

#include <QDebug>
#include <QRegExp>

bool checkVersion (const QString &v)
{
    // returns true, if vym is able to read file regarding 
    // the version set with setVersion
    return checkVersion (v,__VYM_VERSION);
}


bool checkVersion (const QString &v, const QString &vstatic)
{
    bool ok=false;
    int v1=0;
    int v2=0;
    int v3=0;
    int vs1=0;
    int vs2=0;
    int vs3=0;

    QRegExp rx("(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})");
    int pos=rx.indexIn (v);
    if (pos>-1)
    {
	v1=rx.cap(1).toInt(&ok);
	if (ok) v2=rx.cap(2).toInt(&ok);
	if (ok) v3=rx.cap(3).toInt(&ok);
    } 

    pos=rx.indexIn (vstatic);
    if (ok && pos>-1)
    {
	vs1=rx.cap(1).toInt(&ok);
	if (ok) vs2=rx.cap(2).toInt(&ok);
	if (ok) vs3=rx.cap(3).toInt(&ok);
    } 

    if (!ok) 
    {
	qWarning ()<<QString ("Warning: Checking version failed: v=%1").arg(v);
	return false;
    }	
    
    if (vs1 > v1)
	return true;
    if (vs1 < v1)
	return false;
    if (vs2 > v2)
	return true;
    if (vs2 < v2)
	return false;
    if (vs3 > v3)
	return true;
    if (vs3 < v3)
	return false;
    return true;    

}
