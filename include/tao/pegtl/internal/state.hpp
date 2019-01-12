// Copyright (c) 2014-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_INTERNAL_STATE_HPP
#define TAO_PEGTL_INTERNAL_STATE_HPP

#include "../config.hpp"

#include "duseltronik.hpp"
#include "seq.hpp"
#include "skip_control.hpp"

#include "../apply_mode.hpp"
#include "../rewind_mode.hpp"

#include "../analysis/generic.hpp"

namespace tao
{
   namespace TAO_PEGTL_NAMESPACE
   {
      namespace internal
      {
         template< typename State, typename... Rules >
         struct state
         {
            using analyze_t = analysis::generic< analysis::rule_type::seq, Rules... >;

            template< apply_mode A,
                      rewind_mode M,
                      template< typename... >
                      class Action,
                      template< typename... >
                      class Control,
                      typename Input,
                      typename... States >
            static auto success( State& s, const Input& in, States&&... st )
               -> decltype( s.template success< A, M, Action, Control >( in, st... ), void() )
            {
               s.template success< A, M, Action, Control >( in, st... );
            }

            template< apply_mode A,
                      rewind_mode M,
                      template< typename... >
                      class Action,
                      template< typename... >
                      class Control,
                      typename Input,
                      typename... States,
                      int = 0 >
            static auto success( State& s, const Input& in, States&&... st )
               -> decltype( s.success( in, st... ), void() )
            {
               s.success( in, st... );
            }

            template< apply_mode A,
                      rewind_mode M,
                      template< typename... >
                      class Action,
                      template< typename... >
                      class Control,
                      typename Input,
                      typename... States >
            [[nodiscard]] static bool match( Input& in, States&&... st )
            {
               State s( static_cast< const Input& >( in ), st... );

               if( duseltronik< seq< Rules... >, A, M, Action, Control >::match( in, s ) ) {
                  success< A, M, Action, Control >( s, in, st... );
                  return true;
               }
               return false;
            }
         };

         template< typename State, typename... Rules >
         inline constexpr bool skip_control< state< State, Rules... > > = true;

      }  // namespace internal

   }  // namespace TAO_PEGTL_NAMESPACE

}  // namespace tao

#endif
