/*======================================================================

  This file is part of the elastix software.

  Copyright (c) University Medical Center Utrecht. All rights reserved.
  See src/CopyrightElastix.txt or http://elastix.isi.uu.nl/legal.php for
  details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE. See the above copyright notices for more information.

======================================================================*/

#ifndef __elxInstallAllComponents_h
#define __elxInstallAllComponents_h

#include "elxComponentDatabase.h"

/** The CMake generated files contain macros: */
#include "elxMacro.h"

/** The following file, generated by CMake, declares all
 * the InstallComponent functions implemented by the components. */
#include "elxInstallComponentFunctionDeclarations.h"


int InstallAllComponents( elx::ComponentDatabase * _cdb )
{
  int ret = 0;

  /** The following file, generated by CMake, calls the
   * InstallComponent functions of the components. */

  #include "elxInstallComponentFunctionCalls.h"

  /** ret!=0 when something failed. */
  return ret;

} // end InstallAllComponents

#endif // #ifndef __elxInstallAllComponents_h
