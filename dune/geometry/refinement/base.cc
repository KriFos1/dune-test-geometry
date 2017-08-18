// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef DUNE_GEOMETRY_REFINEMENT_BASE_CC
#define DUNE_GEOMETRY_REFINEMENT_BASE_CC

/*!
 * \file
 *
 * \brief This file contains the parts independent of a particular
 *        \ref Refinement implementation.
 */

#include <dune/geometry/type.hh>

namespace Dune
{
  /*!
   * \addtogroup Refinement Refinement
   * \{
   */

  /*!
   * \brief This namespace contains the implementation of \ref
   *        Refinement.
   */
  namespace RefinementImp
  {
    // /////////////////////////////////
    //
    // Declaration of RefinementImp::Traits
    //

#ifdef DOXYGEN
    // This is just for Doxygen
    /*!
     * \brief Mapping from \a geometryType, \a CoordType and \a coerceTo
     *        to a particular \ref Refinement implementation.
     *
     * \tparam topologyId The topology id of the element to refine
     * \tparam CoordType  The C++ type of the coordinates
     * \tparam coerceToId The topologyId of the subelements
     * \tparam dimension  The dimension of the refinement.
     * \tparam Dummy      Dummy parameter which can be used for SFINAE, should
     *                    always be void.
     *
     * Each \ref Refinement implementation has to define one or more
     * specialisations of this struct to declare what it implements.
     * Template class Refinement uses this struct to know which
     * implementation it should inherit from.  Since non-type template
     * arguments of specializations may not involve template parameters, it is
     * often impossible to specify the specialization for all cases directly.
     * As the workaround, the template parameter \a Dummy can be used for
     * SFINAE with \a enable_if.
     *
     * Each specialisation should contain a single member typedef Imp,
     * e.g.:
     * \code
     * template<class CoordType>
     * struct Traits<sphereTopologyId, CoordType, Impl::CubeToplogy<2>::id, 2>
     * {
     *   typedef SquaringTheCircle::Refinement Imp;
     * };
     * \endcode
     */
    template<unsigned topologyId, class CoordType,
        unsigned coerceToId, int dimension, class Dummy = void>
    struct Traits
    {
      //! The implementation this specialisation maps to
      typedef SquaringTheCircle::Refinement Imp;
    };

#else // !DOXYGEN

    // Doxygen won't see this

    template<unsigned topologyId, class CoordType,
        unsigned coerceToId, int dimension, class = void>
    struct Traits;

#endif // !DOXYGEN
  } // namespace RefinementImp


  class RefinementIntervals{
    int intervals_=1;

  public:
    explicit RefinementIntervals(int i) : intervals_(i) {}

    int intervals() const { return intervals_; }
  };

  inline RefinementIntervals refinementIntervals(int i)
  {
    return RefinementIntervals{i};
  }
  inline RefinementIntervals refinementLevels(int l)
  {
    return RefinementIntervals{1<<l};
  }

  // ///////////////
  //
  //  Static Refinement
  //

  /*!
   * \brief Wrap each \ref Refinement implementation to get a
   *        consistent interface
   *
   * \tparam topologyId The topology id of the element to refine
   * \tparam CoordType  The C++ type of the coordinates
   * \tparam coerceToId The topology id of the subelements
   * \tparam dimension  The dimension of the refinement.
   */
  template<unsigned topologyId, class CoordType,
      unsigned coerceToId, int dimension_>
  class StaticRefinement
    : public RefinementImp::Traits<topologyId, CoordType,
          coerceToId, dimension_ >::Imp
  {
  public:
#ifdef DOXYGEN
    /*!
     * \brief The Codim struct inherited from the \ref Refinement implementation
     *
     * \tparam codimension There is a different struct Codim for each codimension
     */
    template<int codimension>
    struct Codim
    {
      /*!
       * \brief The SubEntityIterator for each codim
       *
       * This is \em some sort of type, not necessarily a typedef
       */
      typedef SubEntityIterator;
    };

    //! The VertexIterator of the Refinement
    typedef Codim<dimension>::SubEntityIterator VertexIterator;
    //! The ElementIterator of the Refinement
    typedef Codim<0>::SubEntityIterator ElementIterator;

    /*!
     * \brief The CoordVector of the Refinement
     *
     * This is always a typedef to a FieldVector
     */
    typedef CoordVector;

    /*!
     * \brief The IndexVector of the Refinement
     *
     * This is always a typedef to a FieldVector
     */
    typedef IndexVector;
#endif

    typedef typename RefinementImp::Traits< topologyId, CoordType, coerceToId, dimension_>::Imp RefinementImp;

    using RefinementImp::dimension;

    using RefinementImp::Codim;

    using typename RefinementImp::VertexIterator;
    using typename RefinementImp::CoordVector;

    using typename RefinementImp::ElementIterator;
    using typename RefinementImp::IndexVector;

    //! Get the number of Vertices
    DUNE_DEPRECATED_MSG("nVertices(int) is deprecated, use nVertices(Dune::refinement{Intervals|Levels})")
    static int nVertices(int level)
    {
      return RefinementImp::nVertices(1<<level);
    }
    static int nVertices(Dune::RefinementIntervals tag)
    {
      return RefinementImp::nVertices(tag.intervals());
    }
    //! Get a VertexIterator
    DUNE_DEPRECATED_MSG("vBegin(int) is deprecated, use vBegin(Dune::refinement{Intervals|Levels})")
    static VertexIterator vBegin(int level)
    {
      return RefinementImp::vBegin(1<<level);
    }
    static VertexIterator vBegin(Dune::RefinementIntervals tag)
    {
      return RefinementImp::vBegin(tag.intervals());
    }
    //! Get a VertexIterator
    DUNE_DEPRECATED_MSG("nEnd(int) is deprecated, use vEnd(Dune::refinement{Intervals|Levels}(int))")
    static VertexIterator vEnd(int level)
    {
      return RefinementImp::vEnd(1<<level);
    }
    static VertexIterator vEnd(Dune::RefinementIntervals tag)
    {
      return RefinementImp::vEnd(tag.intervals());
    }

    //! Get the number of Elements
    DUNE_DEPRECATED_MSG("nElements(int) is deprecated, use nElements(Dune::refinement{Intervals|Levels}(int))")
    static int nElements(int level)
    {
      return RefinementImp::nElements(1<<level);
    }
    static int nElements(Dune::RefinementIntervals tag)
    {
      return RefinementImp::nElements(tag.intervals());
    }
    //! Get an ElementIterator
    DUNE_DEPRECATED_MSG("eBegin(int) is deprecated, use eBegin(Dune::refinement{Intervals|Levels}(int))")
    static ElementIterator eBegin(int level)
    {
      return RefinementImp::eBegin(1<<level);
    }
    static ElementIterator eBegin(Dune::RefinementIntervals tag)
    {
      return RefinementImp::eBegin(tag.intervals());
    }
    //! Get an ElementIterator
    DUNE_DEPRECATED_MSG("eEnd(int) is deprecated, use eEnd(Dune::refinement{Intervals|Levels}(int))")
    static ElementIterator eEnd(int level)
    {
      return RefinementImp::eEnd(1<<level);
    }
    static ElementIterator eEnd(Dune::RefinementIntervals tag)
    {
      return RefinementImp::eEnd(tag.intervals());
    }
  };

  /*! \} */
} // namespace Dune

#endif // DUNE_GEOMETRY_REFINEMENT_BASE_CC
