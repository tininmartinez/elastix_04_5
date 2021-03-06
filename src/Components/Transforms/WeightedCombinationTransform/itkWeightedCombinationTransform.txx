/*======================================================================

  This file is part of the elastix software.

  Copyright (c) University Medical Center Utrecht. All rights reserved.
  See src/CopyrightElastix.txt or http://elastix.isi.uu.nl/legal.php for
  details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE. See the above copyright notices for more information.

======================================================================*/

#ifndef _itkWeightedCombinationTransform_txx
#define _itkWeightedCombinationTransform_txx

#include "itkWeightedCombinationTransform.h"

namespace itk
{


/**
 * ********************* Constructor ****************************
 */

template < class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions >
WeightedCombinationTransform<TScalarType,NInputDimensions,NOutputDimensions>
::WeightedCombinationTransform(): Superclass(OutputSpaceDimension,0)
{
  this->m_SumOfWeights = 1.0;
  this->m_NormalizeWeights = false;
} // end Constructor


/**
 * ************************ SetParameters ***********************
 */

template < class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions >
void
WeightedCombinationTransform<TScalarType,NInputDimensions,NOutputDimensions>::SetParameters( const ParametersType & param )
{
  if ( param.GetSize() != this->m_TransformContainer.size() )
  {
    itkExceptionMacro( << "Number of parameters does not match the number of transforms set in the transform container." );
  }

  this->m_Jacobian.SetSize( OutputSpaceDimension, param.GetSize() );
  this->m_Parameters = param;
  this->m_SumOfWeights = param.sum();
  if ( this->m_SumOfWeights < 1e-10 && this->m_NormalizeWeights )
  {
    itkExceptionMacro( << "Weights sum to 0." );
  }
  this->Modified();
} // end SetParameters


/**
 * ********************* TransformPoint ****************************
 */

template < class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions >
typename WeightedCombinationTransform<TScalarType,NInputDimensions,NOutputDimensions>
::OutputPointType
WeightedCombinationTransform<TScalarType,NInputDimensions,NOutputDimensions>
::TransformPoint(const InputPointType & ipp ) const
{
  OutputPointType opp;
  opp.Fill(0.0);
  OutputPointType tempopp;
  const TransformContainerType & tc = this->m_TransformContainer;
  const unsigned int N = tc.size();
  const ParametersType & param = this->m_Parameters;

  /** Calculate sum_i w_i T_i(x) */
  for (unsigned int i=0; i < N; ++i )
  {
    tempopp = tc[i]->TransformPoint( ipp );
    const double w = param[i];
    for ( unsigned int d = 0; d < OutputSpaceDimension; ++d )
    {
      opp[d] += w * tempopp[d];
    }
  }

  if ( this->m_NormalizeWeights )
  {
    /** T(x) = \sum_i w_i T_i(x) / sum_i w_i */
    for ( unsigned int d = 0; d < OutputSpaceDimension; ++d )
    {
      opp[d] /= this->m_SumOfWeights;
    }
  }
  else
  {
    /** T(x) = (1 - \sum_i w_i ) x + \sum_i w_i T_i(x) */
    const double factor = 1.0 - this->m_SumOfWeights;
    for ( unsigned int d = 0; d < OutputSpaceDimension; ++d )
    {
      opp[d] += factor * ipp[d];
    }
  }
  return opp;

} // end TransformPoint


/**
 * ********************* GetJacobian ****************************
 */

template < class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions >
const typename WeightedCombinationTransform<TScalarType,NInputDimensions,NOutputDimensions>
::JacobianType &
WeightedCombinationTransform<TScalarType,NInputDimensions,NOutputDimensions>
::GetJacobian( const InputPointType & ipp ) const
{
  NonZeroJacobianIndicesType dummy(0);
  this->GetJacobian( ipp, this->m_Jacobian, dummy );
  return this->m_Jacobian;

} // end GetJacobian()


/**
 * ********************* GetJacobian ****************************
 */

template < class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions >
void
WeightedCombinationTransform<TScalarType,NInputDimensions,NOutputDimensions>
::GetJacobian(
  const InputPointType & ipp,
  JacobianType & jac,
  NonZeroJacobianIndicesType & nzji ) const
{
  /** This transform has no nonzerojacobianindices */

  OutputPointType tempopp;
  const TransformContainerType & tc = this->m_TransformContainer;
  const unsigned int N = tc.size();
  const ParametersType & param = this->m_Parameters;

  if ( this->m_NormalizeWeights )
  {	
    /** dT/dmu_i = ( T_i(x) - T(x) ) / ( \sum_i w_i ) */
    OutputPointType opp;
    opp.Fill(0.0);
    for (unsigned int i=0; i < N; ++i )
    {
      tempopp = tc[i]->TransformPoint( ipp );
      const double w = param[i];
      for ( unsigned int d = 0; d < OutputSpaceDimension; ++d )
      {
        opp[d] += w * tempopp[d];
        jac(d,i) = tempopp[d];
      }
    }
    for ( unsigned int d = 0; d < OutputSpaceDimension; ++d )
    {
      opp[d] /= this->m_SumOfWeights;
    }
    for (unsigned int i=0; i < N; ++i )
    {
      for ( unsigned int d = 0; d < OutputSpaceDimension; ++d )
      {
        jac(d,i) = ( jac(d,i) - opp[d] ) / this->m_SumOfWeights;
      }
    }
  }
  else
  {
    /** dT/dmu_i = T_i(x) - x */
    for (unsigned int i=0; i < N; ++i )
    {
      tempopp = tc[i]->TransformPoint( ipp );
      for ( unsigned int d = 0; d < OutputSpaceDimension; ++d )
      {
        jac(d,i) = tempopp[d] - ipp[d];
      }
    }

  }

} // end GetJacobian()


} // end namespace itk


#endif
