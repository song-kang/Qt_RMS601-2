#include "update_package.h"

update_package::update_package(QObject *parent)
	: QObject(parent)
{

}

update_package::~update_package()
{

}

bool update_package::md5Compare()
{
	QString localMd5;

	if (getPath() == "./")
	{
		localMd5 = CSkStaticClass::getFileMd5(CSkStaticClass::GetCurrentPath()+getName());
	}
	else
	{
		localMd5 = CSkStaticClass::getFileMd5(CSkStaticClass::GetCurrentPath()+getPath()+getName());
	}

	if (localMd5 != getMd5())
	{
		return false;
	}

	return true;
}
