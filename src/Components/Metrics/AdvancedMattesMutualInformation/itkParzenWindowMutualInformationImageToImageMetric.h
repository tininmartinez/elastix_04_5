/*======================================================================

  This file is part of the elastix software.

  Copyright (c) University Medical Center Utrecht. All rights reserved.
  See src/CopyrightElastix.txt or http://elastix.isi.uu.nl/legal.php for
  details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE. See the above copyright notices for more information.

======================================================================*/

#ifndef __itkParzenWindowMutualInformationImageToImageMetric_H__
#define __itkParzenWindowMutualInformationImageToImageMetric_H__

#include "itkParzenWindowHistogramImageToImageMetric.h"

#include "itkArray2D.h"

namespace itk
{

  /**
   * \class ParzenWindowMutualInformationImageToImageMetric
   * \brief Computes the mutual information between two images to be
   * registered using the method of Mattes et al.
   *
   * ParzenWindowMutualInformationImageToImageMetric computes the mutual
   * information between a fixed and moving image to be registered.
   *
   * The calculations are based on the method of Mattes et al. [1,2],
   * where the probability density distribution are estimated using
   * Parzen histograms. Once the PDFs have been constructed, the
   * mutual information is obtained by double summing over the
   * discrete PDF values.
   *
   * Construction of the PDFs is implemented in the superclass
   * ParzenWindowHistogramImageToImageMetric.
   *
   * This implementation of the MattesMutualInformation is based on the
   * AdvancedImageToImageMetric, which means that:
   * \li It uses the ImageSampler-framework
   * \li It makes use of the compact support of B-splines, in case of B-spline transforms.
   * \li Image derivatives are computed using either the B-spline interpolator's implementation
   * or by nearest neighbor interpolation of a precomputed central difference image.
   * \li A minimum number of samples that should map within the moving image (mask) can be specified.
   *
   * Notes:\n
   * 1. This class returns the negative mutual information value.\n
   * 2. This class in not thread safe due the private data structures
   *     used to the store the marginal and joint pdfs.
   *
   * References:\n
   * [1] "Nonrigid multimodality image registration"\n
   *      D. Mattes, D. R. Haynor, H. Vesselle, T. Lewellen and W. Eubank\n
   *      Medical Imaging 2001: Image Processing, 2001, pp. 1609-1620.\n
   * [2] "PET-CT Image Registration in the Chest Using Free-form Deformations"\n
   *      D. Mattes, D. R. Haynor, H. Vesselle, T. Lewellen and W. Eubank\n
   *      IEEE Transactions in Medical Imaging. To Appear.\n
   * [3] "Optimization of Mutual Information for MultiResolution Image
   *      Registration"\n
   *      P. Thevenaz and M. Unser\n
   *      IEEE Transactions in Image Processing, 9(12) December 2000.\n
   *
   * \ingroup Metrics
   * \sa ParzenWindowHistogramImageToImageMetric
   */

  template <class TFixedImage,class TMovingImage >
    class ParzenWindowMutualInformationImageToImageMetric :
  public ParzenWindowHistogramImageToImageMetric< TFixedImage, TMovingImage >
  {
  public:

    /** Standard class typedefs. */
    typedef ParzenWindowMutualInformationImageToImageMetric             Self;
    typedef ParzenWindowHistogramImageToImageMetric<
      TFixedImage, TMovingImage >                                       Superclass;
    typedef SmartPointer<Self>                                          Pointer;
    typedef SmartPointer<const Self>                                    ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro( Self );

    /** Run-time type information (and related methods). */
    itkTypeMacro(
      ParzenWindowMutualInformationImageToImageMetric,
      ParzenWindowHistogramImageToImageMetric );

    /** Typedefs from the superclass. */
    typedef typename
      Superclass::CoordinateRepresentationType              CoordinateRepresentationType;
    typedef typename Superclass::MovingImageType            MovingImageType;
    typedef typename Superclass::MovingImagePixelType       MovingImagePixelType;
    typedef typename Superclass::MovingImageConstPointer    MovingImageConstPointer;
    typedef typename Superclass::FixedImageType             FixedImageType;
    typedef typename Superclass::FixedImageConstPointer     FixedImageConstPointer;
    typedef typename Superclass::FixedImageRegionType       FixedImageRegionType;
    typedef typename Superclass::TransformType              TransformType;
    typedef typename Superclass::TransformPointer           TransformPointer;
    typedef typename Superclass::InputPointType             InputPointType;
    typedef typename Superclass::OutputPointType            OutputPointType;
    typedef typename Superclass::TransformParametersType    TransformParametersType;
    typedef typename Superclass::TransformJacobianType      TransformJacobianType;
    typedef typename Superclass::InterpolatorType           InterpolatorType;
    typedef typename Superclass::InterpolatorPointer        InterpolatorPointer;
    typedef typename Superclass::RealType                   RealType;
    typedef typename Superclass::GradientPixelType          GradientPixelType;
    typedef typename Superclass::GradientImageType          GradientImageType;
    typedef typename Superclass::GradientImagePointer       GradientImagePointer;
    typedef typename Superclass::GradientImageFilterType    GradientImageFilterType;
    typedef typename Superclass::GradientImageFilterPointer GradientImageFilterPointer;
    typedef typename Superclass::FixedImageMaskType         FixedImageMaskType;
    typedef typename Superclass::FixedImageMaskPointer      FixedImageMaskPointer;
    typedef typename Superclass::MovingImageMaskType        MovingImageMaskType;
    typedef typename Superclass::MovingImageMaskPointer     MovingImageMaskPointer;
    typedef typename Superclass::MeasureType                MeasureType;
    typedef typename Superclass::DerivativeType             DerivativeType;
    typedef typename Superclass::DerivativeValueType        DerivativeValueType;
    typedef typename Superclass::ParametersType             ParametersType;
    typedef typename Superclass::FixedImagePixelType        FixedImagePixelType;
    typedef typename Superclass::MovingImageRegionType      MovingImageRegionType;
    typedef typename Superclass::ImageSamplerType           ImageSamplerType;
    typedef typename Superclass::ImageSamplerPointer        ImageSamplerPointer;
    typedef typename Superclass::ImageSampleContainerType   ImageSampleContainerType;
    typedef typename
      Superclass::ImageSampleContainerPointer               ImageSampleContainerPointer;
    typedef typename Superclass::FixedImageLimiterType      FixedImageLimiterType;
    typedef typename Superclass::MovingImageLimiterType     MovingImageLimiterType;
    typedef typename
      Superclass::FixedImageLimiterOutputType               FixedImageLimiterOutputType;
    typedef typename
      Superclass::MovingImageLimiterOutputType              MovingImageLimiterOutputType;
    typedef typename
      Superclass::MovingImageDerivativeScalesType           MovingImageDerivativeScalesType;

    /** The fixed image dimension. */
    itkStaticConstMacro( FixedImageDimension, unsigned int,
      FixedImageType::ImageDimension );

    /** The moving image dimension. */
    itkStaticConstMacro( MovingImageDimension, unsigned int,
      MovingImageType::ImageDimension );

    /**  Get the value. */
    MeasureType GetValue( const ParametersType& parameters ) const;

    /** Set/get whether to apply the technique introduced by Nicholas Tustison; default: false */
    itkGetConstMacro( UseJacobianPreconditioning, bool );
    itkSetMacro( UseJacobianPreconditioning, bool );

  protected:

    /** The constructor. */
    ParzenWindowMutualInformationImageToImageMetric();

    /** The destructor. */
    virtual ~ParzenWindowMutualInformationImageToImageMetric() {};

    /** Protected Typedefs ******************/

    /** Typedefs inherited from superclass */
    typedef typename Superclass::FixedImageIndexType                FixedImageIndexType;
    typedef typename Superclass::FixedImageIndexValueType           FixedImageIndexValueType;
    typedef typename Superclass::MovingImageIndexType               MovingImageIndexType;
    typedef typename Superclass::FixedImagePointType                FixedImagePointType;
    typedef typename Superclass::MovingImagePointType               MovingImagePointType;
    typedef typename Superclass::MovingImageContinuousIndexType     MovingImageContinuousIndexType;
    typedef typename Superclass::BSplineInterpolatorType            BSplineInterpolatorType;
    typedef typename Superclass::CentralDifferenceGradientFilterType        CentralDifferenceGradientFilterType;
    typedef typename Superclass::MovingImageDerivativeType          MovingImageDerivativeType;
    typedef typename Superclass::PDFValueType                       PDFValueType;
    typedef typename Superclass::MarginalPDFType                    MarginalPDFType;
    typedef typename Superclass::JointPDFType                       JointPDFType;
    typedef typename Superclass::JointPDFDerivativesType            JointPDFDerivativesType;
    typedef typename Superclass::IncrementalMarginalPDFType         IncrementalMarginalPDFType;
    typedef typename Superclass::JointPDFIndexType                  JointPDFIndexType;
    typedef typename Superclass::JointPDFRegionType                 JointPDFRegionType;
    typedef typename Superclass::JointPDFSizeType                   JointPDFSizeType;
    typedef typename Superclass::JointPDFDerivativesIndexType       JointPDFDerivativesIndexType;
    typedef typename Superclass::JointPDFDerivativesRegionType      JointPDFDerivativesRegionType;
    typedef typename Superclass::JointPDFDerivativesSizeType        JointPDFDerivativesSizeType;
    typedef typename Superclass::ParzenValueContainerType           ParzenValueContainerType;
    typedef typename Superclass::KernelFunctionType                 KernelFunctionType;
    typedef typename Superclass::NonZeroJacobianIndicesType         NonZeroJacobianIndicesType;

    /**  Get the value and analytic derivatives for single valued optimizers.
     * Called by GetValueAndDerivative if UseFiniteDifferenceDerivative == false.
     */
    virtual void GetValueAndAnalyticDerivative(
      const ParametersType& parameters,
      MeasureType& value, DerivativeType& derivative ) const;

    /** Get the value and analytic derivatives for single valued optimizers.
     * Called by GetValueAndDerivative if UseFiniteDifferenceDerivative == false
     * and UseExplicitPDFDerivatives == false.
     */
    virtual void GetValueAndAnalyticDerivativeLowMemory(
      const ParametersType& parameters,
      MeasureType& value, DerivativeType& derivative ) const;

    /**  Get the value and finite difference derivatives for single valued optimizers.
     * Called by GetValueAndDerivative if UseFiniteDifferenceDerivative == true.
     */
    virtual void GetValueAndFiniteDifferenceDerivative( const ParametersType& parameters,
      MeasureType& value, DerivativeType& derivative ) const;

    /** Compute terms to implement preconditioning as proposed by Tustison et al. */
    virtual void ComputeJacobianPreconditioner(
      const TransformJacobianType & jac,
      const NonZeroJacobianIndicesType & nzji,
      DerivativeType & preconditioner,
      DerivativeType & divisor ) const;

    /** Some initialization functions, called by Initialize. */
    virtual void InitializeHistograms( void );

  private:

    /** The private constructor. */
    ParzenWindowMutualInformationImageToImageMetric( const Self& ); // purposely not implemented
    /** The private copy constructor. */
    void operator=( const Self& );                              // purposely not implemented

    /** Helper array for storing the values of the JointPDF ratios. */
    typedef double                      PRatioType;
    typedef Array2D< PRatioType >       PRatioArrayType;
    mutable PRatioArrayType             m_PRatioArray;

    /** Setting */
    bool  m_UseJacobianPreconditioning;

    /** Helper function to update the derivative in case of low memory consumption. */
    void UpdateDerivativeLowMemory(
      const RealType & fixedImageValue,
      const RealType & movingImageValue,
      const DerivativeType & imageJacobian,
      const NonZeroJacobianIndicesType & nzji,
      DerivativeType & derivative ) const;

    /** Helper function to compute m_PRatioArray in case of low memory consumption. */
    void ComputeValueAndPRatioArray( double & MI ) const;

  }; // end class ParzenWindowMutualInformationImageToImageMetric

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkParzenWindowMutualInformationImageToImageMetric.hxx"
#endif

#endif // end #ifndef __itkParzenWindowMutualInformationImageToImageMetric_H__

