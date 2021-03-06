/*======================================================================

  This file is part of the elastix software.

  Copyright (c) University Medical Center Utrecht. All rights reserved.
  See src/CopyrightElastix.txt or http://elastix.isi.uu.nl/legal.php for
  details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE. See the above copyright notices for more information.

======================================================================*/

#ifndef __elxRandomSamplerSparseMask_hxx
#define __elxRandomSamplerSparseMask_hxx

#include "elxRandomSamplerSparseMask.h"

namespace elastix
{
  using namespace itk;


  /**
  * ******************* BeforeEachResolution ******************
  */

  template <class TElastix>
    void RandomSamplerSparseMask<TElastix>
    ::BeforeEachResolution( void )
  {
    const unsigned int level =
      ( this->m_Registration->GetAsITKBaseType() )->GetCurrentLevel();

    /** Set the NumberOfSpatialSamples. */
    unsigned long numberOfSpatialSamples = 5000;
    this->GetConfiguration()->ReadParameter( numberOfSpatialSamples,
      "NumberOfSpatialSamples", this->GetComponentLabel(), level, 0 );

    this->SetNumberOfSamples( numberOfSpatialSamples );

  } // end BeforeEachResolution()


} // end namespace elastix

#endif // end #ifndef __elxRandomSamplerSparseMask_hxx

