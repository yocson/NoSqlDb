/////////////////////////////////////////////////////////////////////
// DbCore.cpp - Implements NoSql database prototype                //
// ver 1.0                                                         //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018       //
/////////////////////////////////////////////////////////////////////

#include "DbCore.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Utilities/TestUtilities/TestUtilities.h"
#include "../Query/Query.h"
#include <iostream>
#include <iomanip>
#include <functional>

using namespace NoSqlDb;

//----< test stub >----------------------------------------------------

#ifdef TEST_DBCORE

using namespace Utilities;

int main()
{
  NoSqlDb::DbCore<std::string> strDb;
  DbElement<std::string> demoElem;

  demoElem.name("Modern CPP");
  demoElem.descrip("Scott Meyers");
  demoElem.payLoad("O'Reilly Media");
  strDb["EMCPP"] = demoElem;
  strDb.addElem("TC", "A Tour of CPP", "Bjarne Stroustrup", "Addison-Wesley");
  demoElem.name("Comp Arch");
  demoElem.descrip("John L. Hennessy");
  demoElem.payLoad("Morgan Kaufmann");
  strDb.addElem("CA", demoElem);
  strDb.deleteElem("CA");
  strDb.editTextMata("TC", "name", "A Tour of CPP 2");
  strDb.editTextMata("TC", "descrip", "Bjarne Stroustrup 2");

  DateTime date("Wed Jan 31 22:52:40 2018");
  strDb.editDatetime("EMCPP", date);
  std::cout << "\n  Element CPPL's dateTime edited";
  showElem(strDb["EMCPP"]);

  std::cout << "\n  add child TC to EMCPP";
  strDb.addChild("EMCPP", "TC");
  showElem(strDb["EMCPP"]);
  std::cout << "\n  delete child TC from EMCPP";
  strDb.deleteChild("EMCPP", "TC");
  showElem(strDb["EMCPP"]);
  std::cout << "\n  Persistence functions:";
  strDb.SaveToXML("../testDB.xml");
  strDb.ReadFromXML("../testDB.xml");

  getchar();
  getchar();
  return 0;
}
#endif
