/*
 *
 * Copyright (c) 1997
 * Moscow Center for SPARC Technology
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Moscow Center for SPARC Technology makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 */

#ifndef __SGI_STL_STLCONF_H
# define __SGI_STL_STLCONF_H

# undef __AUTO_CONFIGURED

//==========================================================
// Getting proper values of autoconf flags
// if you ran 'configure', __AUTO_CONFIGURED is set to 1 and
// specific compiler fetures will be used.
// Otherwise, the <stlcomp.h> header will be included for per-version
// features recognition.
//==========================================================
// per-version compiler features recognition


// include the stlcomp.h.vc50 that comes with the sgi stl
#  include "vcl_config_stlcomp.h.vc50"
// define no exceptions because we don't want them yet
# undef    __STL_USE_EXCEPTIONS
//# define __STL_NO_EXCEPTIONS
# define __STL_CONST_CONSTRUCTOR_BUG
// check that __STL_USE_ABBREVS has not already been set (eg, in params.mk)
# ifndef __STL_USE_ABBREVS
#  define __STL_USE_ABBREVS 1
# endif
# define __STL_NO_NAMESPACES
//==========================================================

//==========================================================
// final workaround tuning based on given flags
//==========================================================

// some justification

# if !defined ( __STL_STATIC_TEMPLATE_DATA )
#   define __STL_STATIC_TEMPLATE_DATA 0
#  if !defined ( __STL_WEAK_ATTRIBUTE )
#   define __STL_WEAK_ATTRIBUTE 0
#  endif
# endif

# if defined (__STL_BASE_TYPEDEF_BUG)
#  undef  __STL_BASE_TYPEDEF_OUTSIDE_BUG
#  define __STL_BASE_TYPEDEF_OUTSIDE_BUG 1
# endif

// features tuning
# ifdef __STL_DEBUG
#  define __STL_ASSERTIONS 1
# endif

# ifdef __STL_ASSERTIONS
#  define __stl_assert(expr) \
    if (!(expr)) { fprintf(stderr, "%s:%d STL assertion failure: %s\n", \
                          __FILE__, __LINE__, # expr); abort(); }
# else
#  define __stl_assert(expr)
# endif

# ifdef __STL_DEBUG
#  define __stl_verbose_assert(expr,diagnostic) \
    if (!(expr)) { fprintf(stderr, "%s:%d STL error : %s\n%s:%d STL assertion failure: %s\n",\
                          __FILE__, __LINE__ , diagnostic, __FILE__, __LINE__ , # expr ); abort(); }

#  define __stl_debug_check(expr) __stl_assert(expr)
#  define __stl_debug_do(expr) expr
# else
#  define __stl_verbose_assert(expr,diagnostic) __stl_assert(expr)
#  define __stl_debug_check(expr)
#  define __stl_debug_do(expr)
# endif

# ifdef __STL_RAND48
#  define __rand lrand48
# else
#  define __rand rand
# endif

// tuning of static template data members workaround
# if ( __STL_STATIC_TEMPLATE_DATA < 1 )
// ignore __PUT directive in this case
#  if ( __STL_WEAK_ATTRIBUTE > 0 )
#   define __DECLARE_INSTANCE(type,item,init) type item __attribute__ (( weak )) = init
#  else
#   ifdef __PUT_STATIC_DATA_MEMBERS_HERE
#    define __DECLARE_INSTANCE(type,item,init) type item = init
#   else
#    define __DECLARE_INSTANCE(type,item,init)
#   endif /* __PUT_STATIC_DATA_MEMBERS_HERE */
#  endif /* __STL_WEAK_ATTRIBUTE */
# endif /* __STL_STATIC_TEMPLATE_DATA */

// default parameters as template types derived from arguments ( not always supported )
#  if ! defined (__STL_DEFAULT_TEMPLATE_PARAM)
#   define __DFL_TMPL_PARAM( classname, defval ) class classname
#   define __DFL_TMPL_ARG(classname) , classname
#  else
#   define __STL_DEFAULT_TYPE_PARAM 1
#   define __DFL_TMPL_PARAM( classname, defval ) class classname = defval
#   define __DFL_TMPL_ARG(classname)
#  endif

// default parameters as complete types
# if defined ( __STL_DEFAULT_TYPE_PARAM )
#   define __DFL_TYPE_PARAM( classname, defval ) class classname = defval
#   define __DFL_TYPE_ARG(classname)
# else
#  define __DFL_TYPE_PARAM( classname, defval ) class classname
#  define __DFL_TYPE_ARG(classname) , classname
# endif

// default parameters workaround tuning
#  if defined  ( __STL_DEFAULT_TYPE_PARAM ) || ( defined ( __STL_NAMESPACES )&&! defined (__STL_NO_NAMESPACES))
#    define __WORKAROUND_RENAME(X) X
#  else
#    define __WORKAROUND_RENAME(X) __##X
#  endif

// namespace selection
# if defined (__STL_NAMESPACES) && ! defined (__STL_NO_NAMESPACES)

// change this if don't think that is standard enough ;)
#  define __STL_NAMESPACE std
#  define __BEGIN_STL_NAMESPACE namespace __STL_NAMESPACE {
#  define __STL_USING_NAMESPACE  using namespace __STL_NAMESPACE ;

#  ifdef __STL_NO_USING_STD
#   define __USING_NAMESPACE
#  else
#   define __USING_NAMESPACE using namespace __STL_NAMESPACE ;
#  endif
#  ifdef __STL_DEFAULT_TYPE_PARAM
#    define __STL_FULL_NAMESPACE __STL_NAMESPACE
#    define __BEGIN_STL_FULL_NAMESPACE
#    define __END_STL_FULL_NAMESPACE
#  else
#    define __STL_FULL_NAMESPACE sgi_full
#    define __BEGIN_STL_FULL_NAMESPACE namespace __STL_FULL_NAMESPACE {
#    define __END_STL_FULL_NAMESPACE } ;
#  endif
#  define __END_STL_NAMESPACE }; __USING_NAMESPACE
    // workaround tuning
#  define __FULL_NAME(X) __STL_FULL_NAMESPACE::__WORKAROUND_RENAME(X)
# else /* __STL_NAMESPACES */
#  define __STL_NAMESPACE
#  define __STL_FULL_NAMESPACE
#  define __BEGIN_STL_NAMESPACE
#  define __END_STL_NAMESPACE
#  define __BEGIN_STL_FULL_NAMESPACE
#  define __END_STL_FULL_NAMESPACE
#  define __STL_USING_NAMESPACE
    // workaround tuning
#  define __FULL_NAME(X) __WORKAROUND_RENAME(X)
# endif  /* __STL_NAMESPACES */

#  define __STL_NAME(name) vcl_##name  // Lo Russo Graziano <Graziano.LoRusso@CSELT.IT>


// advanced keywords usage
#  ifdef  __STL_NEW_STYLE_CASTS
#   define __CONST_CAST(x,y) const_cast<x>(y)
#  else
#   define __CONST_CAST(x,y) ((x)y)
#  endif

#  ifndef __STL_TYPENAME
#   define typename
#  endif

#  ifndef __STL_EXPLICIT
#   define explicit
#  endif

#  ifdef __STL_MUTABLE
#   define __ASSIGN_MUTABLE(type,x,y) x=y
#  else
#   define __ASSIGN_MUTABLE(type,x,y) __CONST_CAST(type,x)=y
#   define mutable
#  endif

// exception-specific macros.
// most of them require double parens : (())
#  if defined ( __STL_NO_EXCEPTIONS) || ! defined (__STL_USE_EXCEPTIONS)
#  define IUEg__TRY
#  define IUEg__CATCH(x)      if (false)
#  define IUEg__THROW(arg)
#  define IUEg__RETHROW
#  else
#  define IUEg__CATCH(x)      catch x
#  define IUEg__TRY           try
#  define IUEg__THROW(arg)    throw arg
#  define IUEg__RETHROW       throw
#  endif

// throw specification ( used in inline constructors
// to improve efficiency some compilers )
// param count is variable, parens used.
#  if defined ( __STL_NO_EXCEPTIONS ) || ! defined ( __STL_EXCEPTION_SPEC )
#   define IUEg__THROWS(x)
#  else
#   define IUEg__THROWS(x) throw x
#  endif

#  if defined (__STL_LOOP_INLINE_PROBLEMS)
#   define INLINE_LOOP
#  else
#   define INLINE_LOOP inline
#  endif

#if defined ( __STL_UNINITIALIZABLE_PRIVATE )
#  define __PRIVATE public
   // Extra access restrictions prevent us from really making some things
   // private.
#else
#  define __PRIVATE private
#endif

#  ifdef __STL_FULL_SPEC_SYNTAX
#   define __STL_FULL_SPECIALIZATION template<>
#  else
#   define __STL_FULL_SPECIALIZATION
#  endif

#  define __IMPORT_CONTAINER_TYPEDEFS(super)                            \
    typedef typename super::value_type value_type;                               \
    typedef typename super::reference reference;                                 \
    typedef typename super::size_type size_type;                                 \
    typedef typename super::const_reference const_reference;                     \
    typedef typename super::difference_type difference_type;

#  define __IMPORT_ITERATORS(super)                                              \
    typedef typename super::iterator iterator;                                   \
    typedef typename super::const_iterator const_iterator;

#  define __IMPORT_REVERSE_ITERATORS(super)                                      \
    typedef typename super::const_reverse_iterator  const_reverse_iterator;      \
    typedef typename super::reverse_iterator reverse_iterator;

#define  __IMPORT_SUPER_COPY_ASSIGNMENT(__derived_name)         \
    __derived_name(const self& x) : super(x) {}                 \
    __derived_name(const super& x) : super(x) {}                \
    self& operator=(const self& x) {                            \
        super::operator=(x);                                    \
        return *this;                                           \
    }                                                           \
    self& operator=(const super& x) {                           \
        super::operator=(x);                                    \
        return *this;                                           \
    }

# if defined (__STL_BASE_TYPEDEF_OUTSIDE_BUG) || defined (__STL_NESTED_TYPE_PARAM_BUG)
#   define __CONTAINER_SUPER_TYPEDEFS \
 __IMPORT_CONTAINER_TYPEDEFS(super) __IMPORT_ITERATORS(super) __IMPORT_REVERSE_ITERATORS(super)
# else
#   define __CONTAINER_SUPER_TYPEDEFS
# endif

//==========================================================

#endif /* __STLCONF_H */
