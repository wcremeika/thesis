#include "parser.h"

#include <QDebug>
#include <QRegExp>
#include <iostream>

#include "command.h"
#include "treeitem.h"

extern QList <Command*> modelCommands;

Parser::Parser()
{
    initParser();
}

void Parser::initParser()
{
    initAtom();
    current=-1;
}

void Parser::initAtom()
{
    atom="";
    com="";
    paramList.clear();
    errLevel=NoError;
    errDescription="";
    errMessage="";
}

void Parser::parseAtom (QString s)
{
    initAtom();
    atom=s;
    QRegExp re;
    int pos;

    // Strip WS at beginning
    re.setPattern ("\\w");
    re.setMinimal (true);
    pos=re.indexIn (atom);
    if (pos>=0)
	s=s.right(s.length()-pos);

    // Get command
    re.setPattern ("\\b(.*)(\\s|\\()");
    pos=re.indexIn (s);
    if (pos>=0)
	com=re.cap(1);

    // Get parameters
    paramList.clear();
    QString t;
    int leftParenthesis;
    int rightParenthesis;
    if (!nextParenthesisContents(s, leftParenthesis, rightParenthesis, t)) return;


    paramList=findParameters(t);
}

QString Parser::getAtom()
{
    return atom;
}

QString Parser::getCommand()
{
    return com;
}

QStringList Parser::getParameters()
{
    return paramList;
}

int Parser::parCount()
{
    return paramList.count();
}

QString Parser::errorMessage()
{
    QString l;
    switch (errLevel)
    {
	case NoError: l="No Error";
	case Warning: l="Warning";
	case Aborted: l="Aborted";
    }
    return QString ("Error Level: '%1'  Command: '%2' Description: '%3'")
	.arg(l).arg(com).arg(errDescription);
}

QString Parser::errorDescription()
{
    return errDescription;
}

ErrorLevel Parser::errorLevel()
{
    return errLevel;
}

void Parser::setError(ErrorLevel level, const QString &description)
{
    errDescription=description;
    errLevel=level;
}

bool Parser::checkParameters(TreeItem *selti)
{
    foreach (Command *c, modelCommands)
    {
	if (c->getName() == com)
	{
	    // Check type of selection
	    if (selti)
	    {
		bool ok;
		ok=false;
		TreeItem::Type st=selti->getType();
		Command::SelectionType ct=c->getSelectionType();
		if (ct==Command::TreeItem || ct==Command::BranchOrImage)
		{
		    if (st==TreeItem::MapCenter ||
			st==TreeItem::Branch ||
			st==TreeItem::XLink ||
			st==TreeItem::Image ) 
			ok=true;
		} else if ( ct==Command::BranchOrImage )
		{
		    if (st==TreeItem::MapCenter ||
			st==TreeItem::Branch ||
			st==TreeItem::Image ) 
			ok=true;
		} else if ( ct==Command::Branch || ct==Command::BranchLike)
		{

		    if (st == TreeItem::MapCenter ||
			st == TreeItem::Branch )
			ok=true;
		} else if ( ct==Command::Image )	    
		{
		    if (st==TreeItem::Image )
			ok=true;
		} else if ( ct==Command::Any)	    
		{
		    ok=true;
		} else if ( ct==Command::XLinkItem)	    
		{
		    if (st==TreeItem::XLink)
			ok=true;
		} else
		    qWarning()<<"Parser::checkParameters  Unknown selection type";
		if (!ok)
		{
		    setError (Aborted, "Selection does not match command");
		    return false;
	    	}
	    }

	    // Check for number of parameters
	    int optPars=0;
	    for (int i=0; i < c->parCount(); i++ )
		if (c->isParOptional(i) ) optPars++;
	    if (paramList.count() < (c->parCount() - optPars) ||
	        paramList.count() > c->parCount() )
	    {
		QString expected;
		if (optPars>0)
		    expected=QString("%1..%2").arg(c->parCount()-optPars).arg(c->parCount() );
		else 
		    expected=QString().setNum(c->parCount());
		setError (
		    Aborted,
		    QString("Wrong number of parameters: Expected %1, but found %2").arg(expected).arg(paramList.count()));
		return false;
	    }

	    // Check types of parameters
	    bool ok;
	    for (int i=0; i < paramList.count(); i++ )
	    {	
		switch (c->getParType(i) )
		{
		    case Command::String:
			parString (ok,i);
                        if (!ok) 
                        {
                            // Convert to string implicitly
                            paramList[i]='"' + paramList[i] + '"';
                            ok=true;
                        }
			break;
		    case Command::Int:	
			parInt (ok,i);
			break;
		    case Command::Double:	
			parDouble (ok,i);
			break;
		    case Command::Color:	
			parColor (ok,i);
			break;
		    case Command::Bool:	
			parBool (ok,i);
			break;
		    default: ok=false;	
		}
		if (!ok)
		{
		    setError (
			Aborted, 
			QString("Parameter %1 has wrong type").arg(i));
		    return false;
		}
	    }
	    resetError();
	    return true;
	}    
    } 
    setError (Aborted,"Unknown command");
    return false;
}

void Parser::resetError ()
{
    errMessage="";
    errDescription="";
    errLevel=NoError;
}

bool Parser::checkParCount (const int &expected)
{
    if (paramList.count()!=expected)
    {
	setError (
	    Aborted,
	    QString("Wrong number of parameters: Expected %1, but found %2").arg(expected).arg(paramList.count()));
	return false;
    } 
    return true;    
}

bool Parser::checkParIsInt(const int &index)
{
    bool ok;
    if (index > paramList.count())
    {
	setError (
	    Aborted,
	    QString("Parameter index %1 is outside of parameter list").arg(index));
	return false;
    } else
    {
	paramList[index].toInt (&ok, 10);
	if (!ok)
	{
	    setError (
		Aborted,
		QString("Parameter %1 is not an integer").arg(index));
	    return false;
	} 
    }	
    return true;
}

bool Parser::checkParIsDouble(const int &index)
{
    bool ok;
    if (index > paramList.count())
    {
	setError (
	    Aborted,
	    QString("Parameter index %1 is outside of parameter list").arg(index));
	return false;
    } else
    {
	paramList[index].toDouble (&ok);
	if (!ok)
	{
	    setError (
		Aborted,
		QString("Parameter %1 is not double").arg(index));
	    return false;
	} 
    }	
    return true;
}

int Parser::parInt (bool &ok,const uint &index)
{
    if (checkParIsInt (index))
	return paramList[index].toInt (&ok, 10);
    ok=false;
    return 0;
}

QString Parser::parString (bool &ok,const int &index)
{
    // return the string at index, this could be also stored in
    // a variable later
    QString r;
    QRegExp re("\"(.*)\"");
    int pos=re.indexIn (paramList[index]);
    if (pos>=0)
    {
	r=re.cap (1);
	ok=true;
    } else    
    {
	r="";
	ok=false;
    }
    return r;
}

bool Parser::parBool (bool &ok,const int &index)
{
    // return the bool at index, this could be also stored in
    // a variable later
    QString r;
    ok=true;
    QString p=paramList[index];
    if (p=="true" || p=="1")
	return true;
    else if (p=="false" || p=="0")
	return false;
    ok=false;
    return ok;
}

QColor Parser::parColor(bool &ok,const int &index)
{
    // return the QColor at index
    ok=false;
    QString r;
    QColor c;
    QRegExp re("\"(.*)\"");
    int pos=re.indexIn (paramList[index]);
    if (pos>=0)
    {
	r=re.cap (1);
	c.setNamedColor(r);
	ok=c.isValid();
    }	
    return c;
}

double Parser::parDouble (bool &ok,const int &index)
{
    if (checkParIsDouble (index))
	return paramList[index].toDouble (&ok);
    ok=false;
    return 0;
}

void Parser::setScript(const QString &s)
{
    script=s;
}   

QString Parser::getScript()
{
    return script;
}   

void Parser::execute()
{
    current=0;
}   

bool Parser::next() //FIXME-3 parser does not detect missing closing " or '("foo" ()'
{
    int start=current;
    if (current<0) execute();
    if (current+1>=script.length()) return false;

    bool inBracket=false;
    while (true)
    {
	//qDebug() <<"current="<<current<< "   start="<<start<<"  length="<<script.length();

	// Check if we are inside a string
	if (script.at(current)=='"')
	{
	    if (inBracket)
		inBracket=false;
	    else    
		inBracket=true;
	}

	// Check if we are in a comment
	if (!inBracket && script.at(current)=='#')
	{
	    while (script.at(current)!='\n')
	    {
		current++;
		if (current+1>=script.length()) 
		    return false;
	    }
	    start=current;
	}

	// Check for end of atom
	if (!inBracket && script.at(current)==';')
	{
	    atom=script.mid(start,current-start);
	    current++;
	    return true;
	}
	
	// Check for end of script
	if (current+1>=script.length() )
	{
	    if (inBracket)
	    {
		setError (Aborted,"Runaway string");
		return false;
	    } else
	    {
		atom=script.mid(start);
		return true;
	    }
	}
	current++;
    }
}   

QStringList Parser::getCommands() 
{
    QStringList list;
    foreach (Command *c, modelCommands)
	list.append (c->getName() );
    return list;	
}

QStringList Parser::findParameters(const QString &s)
{
    int pos=0;
    int left=0;
    bool inquote=false;
    QStringList ret;
    while (pos < s.length())
    {
        if (s.at(pos)=='\"') 
        {
            if (inquote)
                inquote=false;
            else    
                inquote=true;
        }
        if (s.at(pos)==',' && !inquote)
        {
            ret << s.mid(left, pos - left );
            left = pos + 1;
        }
        pos++;
    }
    if (left > 0) 
        ret << s.mid(left, pos - left );
    else
        if (!s.isEmpty()) ret << s;
    return ret;
}

bool Parser::nextParenthesisContents(
        const QString &s, 
        int &leftParenthesis, 
        int &rightParenthesis, 
        QString &contents)
{
    int pos=0;
    int leftP=-1;
    int rightP=-1;
    int openParenthesis=0;
    bool inquote=false;
    while (pos < s.length())
    {
        if (s.at(pos)=='\"') 
        {
            if (inquote)
                inquote=false;
            else    
                inquote=true;
        }
        if (s.at(pos)=='(' && !inquote) 
        {
            openParenthesis++;
            if (openParenthesis==1) leftP=pos;
        }

        if (s.at(pos)==')' && !inquote) 
        {
            openParenthesis--;
            if (openParenthesis==0) rightP=pos;
        }

        if (openParenthesis<0) 
        {
            setError(Aborted, "Error, too many closing parenthesis!");
            return false;
        }

        pos++;
    }
    if (leftP< 0) 
    {
        setError(Aborted, "Error: No left parenthesis found");
        return false;
    }

    if (rightP< 0) 
    {
        setError(Aborted, "Error: No right parenthesis found");
        return false;
    }

    contents=s.mid(leftP+1, rightP - leftP - 1);
    pos = leftParenthesis;
    leftParenthesis=leftP;
    rightParenthesis=rightP;
    return true;
}

