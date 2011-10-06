// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef DUNE_GENERICGEOMETRY_TRACEPROVIDER_HH
#define DUNE_GENERICGEOMETRY_TRACEPROVIDER_HH

#include <dune/common/forloop.hh>
#include <dune/common/typetraits.hh>
#include "subtopologies.hh"

namespace Dune
{

  namespace GenericGeometry
  {

    // External Forward Declarations
    // -----------------------------

    template< class Topology, class GeometryTraits >
    class CachedMapping;

    template< unsigned int dim, class GeometryTraits >
    class HybridMapping;

    template< class Topology, class GeometryTraits >
    class VirtualMapping;



    // TraceProvider
    // -------------

    template< class Topology, class GeometryTraits, unsigned int codim, bool forceHybrid >
    class TraceProvider
    {
      typedef TraceProvider< Topology, GeometryTraits, codim, forceHybrid > This;

    public:
      static const unsigned int dimension = Topology :: dimension;
      static const unsigned int codimension = codim;
      static const unsigned int mydimension = dimension - codimension;

      static const bool hybrid
        = (forceHybrid || IsCodimHybrid< Topology, codim > :: value);

      typedef typename CachedMapping< Topology, GeometryTraits > :: Mapping Mapping;

    private:
      static const unsigned int numSubTopologies
        = Mapping :: ReferenceElement :: template Codim< codimension > :: size;

      template< bool > class HybridFactory;
      template< bool > class NonHybridFactory;

      typedef typename SelectType< hybrid, HybridFactory<true>, NonHybridFactory<false> > :: Type Factory;

      template< int i > struct Builder;

    public:
      typedef typename Factory::Trace Trace;

      static Trace* construct ( const Mapping &mapping, unsigned int i, char *traceStorage )
      {
        return (*instance().construct_[ i ])( mapping, traceStorage );
      }

    private:
      typedef Trace* (*Construct)( const Mapping &mapping, char *traceStorage );

      TraceProvider ()
      {
        ForLoop< Builder, 0, numSubTopologies-1 >::apply( construct_ );
      }

      static const This &instance ()
      {
        static This theInstance;
        return theInstance;
      }

      Construct construct_[ numSubTopologies ];
    };



    template< class Topology, class GeometryTraits, unsigned int codim, bool forceHybrid >
    template< bool >
    class TraceProvider< Topology, GeometryTraits, codim, forceHybrid > :: HybridFactory
    {
      template< unsigned int i >
      struct VirtualTrace
      {
        typedef typename GenericGeometry :: SubTopology< Topology, codim, i > :: type
        SubTopology;
        typedef VirtualMapping< SubTopology, GeometryTraits > type;
      };

    public:
      typedef HybridMapping< mydimension, GeometryTraits > Trace;

      template< int i >
      static Trace* construct ( const Mapping &mapping, char *traceStorage )
      {
        typedef typename VirtualTrace< i >::type TraceImpl;
        return new( traceStorage ) TraceImpl( mapping.template trace< codim, i >() );
      }
    };



    template< class Topology, class GeometryTraits, unsigned int codim, bool forceHybrid >
    template< bool >
    class TraceProvider< Topology, GeometryTraits, codim, forceHybrid > :: NonHybridFactory
    {
      typedef typename GenericGeometry :: SubTopology< Topology, codim, 0 > :: type
      SubTopology;

    public:
      typedef CachedMapping< SubTopology, GeometryTraits > Trace;

      template< int i >
      static Trace* construct ( const Mapping &mapping, char *traceStorage )
      {
        return new( traceStorage ) Trace( mapping.template trace< codim, i >() );
      }
    };



    template< class Topology, class GeometryTraits, unsigned int codim, bool forceHybrid >
    template< int i >
    struct TraceProvider< Topology, GeometryTraits, codim, forceHybrid > :: Builder
    {
      static void apply ( Construct (&construct)[ numSubTopologies ] )
      {
        construct[ i ] = &(Factory::template construct< i >);
      }
    };

  }

}

#endif // #ifndef DUNE_GENERICGEOMETRY_TRACEPROVIDER_HH
