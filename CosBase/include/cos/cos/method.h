#ifndef COS_COS_METHOD_H
#define COS_COS_METHOD_H

/*
 o---------------------------------------------------------------------o
 |
 | COS method (macros)
 |
 o---------------------------------------------------------------------o
 |
 | C Object System
 |
 | Copyright (c) 2006+ Laurent Deniau, laurent.deniau@cern.ch
 |
 | For more information, see:
 | http://cern.ch/laurent.deniau/cos.html
 |
 o---------------------------------------------------------------------o
 |
 | This file is part of the C Object System framework.
 |
 | The C Object System is free software; you can redistribute it and/or
 | modify it under the terms of the GNU Lesser General Public License
 | as published by the Free Software Foundation; either version 3 of
 | the License, or (at your option) any later version.
 |
 | The C Object System is distributed in the hope that it will be
 | useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 | of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 |
 | See <http://www.gnu.org/licenses> for more details.
 |
 o---------------------------------------------------------------------o
 |
 | $Id: method.h,v 1.3 2008/08/13 09:28:12 ldeniau Exp $
 |
*/

#ifndef COS_COS_COS_H
#error "COS: use <cos/cos/cos.h> instead of <cos/cos/method.h>"
#endif

/* NOTE-USER: method declaration, definition and instantiation

   method-reference:
     methodref( generic-name , class-list )

   method-declaration:
     usemethod( method-decl );

   method-decl:
     generic-name , class-list
     ( generic-name , class-list ) local-name

   class-list:
     class-name
     class-list , class-name

   method-definition:
     defmethod( generic-specifier )
       method-statement
     endmethod

   method-statement:
     statement
     statement-with-contract

   next-method-declaration:
     defnext( generic-specifier );

   where in the context of methods:
     - the param-name_opt in param-decl can be omitted for unused argument.
     - the last param-decl must be (va_list)va for variadic generics.

   examples:
     usemethod(gfoo,A,B);      // declare method gfoo(A,B)
     usemethod((gfoo,A,B)foo); // declare method gfoo(A,B) as foo

     defmethod(OBJ, gclone   , Object)                        .. endmethod
     defmethod(OBJ, ggetAt   , Array, Range)                  .. endmethod
     defmethod(OBJ, ggetAtIdx, Array, (U32)idx)               .. endmethod
     defmethod(OBJ, gprint   , Stream, String)                .. endmethod
     defmethod(OBJ, vprintFmt, Stream, (STR)fmt, (va_list)va) .. endmethod
*/

/* method keywords:
 */
#ifdef  COS_DISABLE_ALL
#define COS_DISABLE_methodref
#define COS_DISABLE_usemethod
#define COS_DISABLE_defmethod
#define COS_DISABLE_endmethod
#define COS_DISABLE_retmethod
#define COS_DISABLE_defnext
#define COS_DISABLE_next_method
#define COS_DISABLE_next_method_p
#define COS_DISABLE_forward_message
#define COS_DISABLE_retval
#endif

#ifndef COS_DISABLE_methodref
#define methodref(...) COS_MTH_REF(__VA_ARGS__)
#endif

#ifndef COS_DISABLE_usemethod
#define usemethod(...) COS_MTH_USE(__VA_ARGS__)
#endif

#ifndef COS_DISABLE_defmethod
#define defmethod(...) COS_MTH_DEF(__VA_ARGS__)
#endif

#ifndef COS_DISABLE_endmethod
#define endmethod COS_MTH_END
#endif

#ifndef COS_DISABLE_retmethod
#define retmethod(...) COS_MTH_RET(__VA_ARGS__)
#endif

#ifndef COS_DISABLE_defnext
#define defnext(...) COS_MTH_DEFN(__VA_ARGS__)
#endif

#ifndef COS_DISABLE_next_method
#define next_method(...) COS_MTH_NXT(__VA_ARGS__)
#endif

#ifndef COS_DISABLE_next_method_p
#define next_method_p COS_MTH_NXT_P
#endif

#ifndef COS_DISABLE_forward_message
#define forward_message(...) COS_MTH_FWD(__VA_ARGS__)
#endif

#ifndef COS_DISABLE_retval
#define retval COS_MTH_RETVAL
#endif

/***********************************************************
 * Implementation
 */

/* method reference
 */
#define COS_MTH_REF(NAME,...) \
        ((OBJ)(void*)&COS_MTH_NAME(NAME,(__VA_ARGS__)))

/* method declaration
 */
#define COS_MTH_USE(...) \
        COS_PP_IF(COS_PP_ISTUPLE(__VA_ARGS__))( \
          COS_PP_PAIR(COS_MTH_USE_0,(COS_PP_PPART __VA_ARGS__)), \
          COS_MTH_USE_1(__VA_ARGS__))

#define COS_MTH_USE_0(DECL,LNAME) \
        COS_MTH_USE_2(COS_PP_ARG1 DECL,LNAME,(COS_PP_REST DECL))

#define COS_MTH_USE_1(NAME,...) \
        COS_MTH_USE_2(NAME,NAME,(__VA_ARGS__))

#define COS_MTH_USE_2(GNAME,LNAME,CS) \
        COS_MTH_USE_3(GNAME,LNAME,CS,COS_PP_LEN(CS))

#define COS_MTH_USE_3(GNAME,LNAME,CS,C) \
        extern struct COS_PP_CAT(Method,C) COS_MTH_NAME(GNAME,CS); \
        static OBJ const LNAME = (OBJ)(void*)&COS_MTH_NAME(GNAME,CS)

/* method definition and instantiation
 */
#define COS_MTH_DEF(  RET,NAME,...) \
        COS_MTH_DEF_0(RET,NAME,(__VA_ARGS__), \
          COS_PP_FILTER((__VA_ARGS__),COS_PP_ISNTUPLE), \
          COS_PP_FILTER((__VA_ARGS__),COS_PP_ISTUPLE ) )

#define COS_MTH_DEF_0(RET,NAME,PS,CS,AS) \
        COS_MTH_DEF_1(RET,NAME,PS,CS,AS, \
          COS_PP_LEN(CS), \
          COS_PP_ISTUPLE(COS_PP_SEQ(AS)), \
          COS_PP_NOT(COS_TOK_ISVOID(RET)) )

#define COS_MTH_DEF_1(RET,NAME,PS,CS,AS,C,A,R) \
COS_MTH_RANKCHK(    NAME,   CS,   C    ) \
COS_MTH_TYPECHK(RET,NAME,PS,CS         ) \
COS_MTH_COMPMAK(    NAME,   CS,   C    ) \
COS_MTH_FUNCDEF(RET,NAME,PS,CS,AS,C,A,R) \
COS_CTR_BEGCTR

#define COS_MTH_END   \
  COS_CTR_ENDCTR      \
  goto _cos_mth_fini; \
}

/* next_method definition
 */
#define COS_MTH_DEFN(  RET,NAME,...) \
        COS_MTH_DEFN_0(RET,NAME,(__VA_ARGS__), \
          COS_PP_FILTER((__VA_ARGS__),COS_PP_ISNTUPLE) )

#define COS_MTH_DEFN_0(RET,NAME,PS,CS) \
/* next_method definition */ \
COS_MTH_NEXTDEF(RET,NAME,PS,CS,COS_PP_LEN(CS)) \
/* next_method classes */ \
static struct Class* const _cos_mth_nxt_cls[] = { COS_PP_SEQ(COS_PP_MAP(CS,COS_MTH_CLS))}

/*
 * Low-level implementation
 */

// rank check
#define COS_MTH_RANKCHK(NAME,CS,C) \
COS_PP_IF(COS_PP_GE(C,5))( \
  COS_STATIC_ASSERT( \
    COS_PP_CAT3(COS_SYM_NAME(NAME,CS), \
                __method_rank_greater_than_,COS_MTH_RNKMAX), \
    COS_PP_SEPWITH(COS_PP_MAP(CS,COS_CLS_RANK),+) < COS_MTH_RNKMAX); \
,/* always true */)

// type check
#define COS_MTH_TYPECHK(RET,NAME,PS,CS) \
extern COS_GEN_TYPE(NAME) \
  COS_PP_CAT(COS_SYM_NAME(NAME,CS),__invalid_defmethod_vs_defgeneric); \
extern RET (* \
  COS_PP_CAT(COS_SYM_NAME(NAME,CS),__invalid_defmethod_vs_defgeneric) ) \
                                            COS_PP_MAP(PS,COS_SIG_GEN);

// method definition
#define COS_MTH_FUNCDEF(RET,NAME,PS,CS,AS,C,A,R) \
static void COS_FCT_NAME(NAME,CS) \
(SEL const _sel, COS_PP_SEQ(COS_SEL_DECL(C)), \
   void* const _arg, void* const _ret) \
{ \
  /* return type */ \
  typedef RET* const _cos_mth_ret; \
  /* arguments type */ \
  typedef COS_ARG_TYPE(NAME)* const _cos_mth_arg; \
  /* selfs types (i.e. _cos_mth_slfn) */ \
  COS_PP_SEP(COS_PP_MAP2(CS,COS_SLF_TYPE(C),COS_MTH_SLF)) \
  /* next_method definition */ \
  COS_MTH_NEXTDEF(RET,NAME,PS,CS,C) \
  /* next_method classes */ \
  static struct Class* const* const _cos_mth_nxt_cls = COS_MTH_NAME(NAME,CS).cls; \
  /* selfs variables */ \
  _cos_mth_slf1 self = (_cos_mth_slf1)_1; COS_PP_SEP(COS_SLF_DECL(C)) \
  /* arguments variables initialization (if any) */ \
  COS_PP_IF(A)(COS_PP_SEP(COS_PP_MAP(AS,COS_MTH_ARG)),/* no arg */) \
  goto _cos_mth_body; \
  /* exit point */ \
  _cos_mth_fini: \
  /* arguments variables deinitialization (if any) */ \
  COS_PP_IF(A)(COS_PP_SEP(COS_PP_MAP(AS,COS_MTH_DEARG)),/* no arg */) \
  return; \
  /* avoid compiler warning for unused identifiers */ \
  COS_PP_IF(R)(/* ret */,COS_UNUSED(_ret);) \
  COS_UNUSED(next_method,self); \
  COS_UNUSED(_sel,_arg,_cos_mth_nxt_sel,_cos_mth_nxt_rnk,_cos_mth_nxt_cls,_cos_mth_nxt_p); \
  _cos_mth_body:

// component intantiation (see cos/cos/coscls.h)
#define COS_MTH_COMPMAK(NAME,CS,C) \
static void COS_FCT_NAME(NAME,CS) \
(SEL _sel, COS_PP_SEQ(COS_SEL_TYPE(C)), void* _arg, void* _ret); \
 \
struct COS_PP_CAT(Method,C) COS_MTH_NAME(NAME,CS) = { \
   /* encode tag into rc */ \
  {{{ 0, cos_tag_method }}, \
   /* reference to generic */ \
   &COS_GEN_NAME(NAME), \
   /* cryptic information */ \
   COS_MTH_INFO(COS_PP_SEQ(COS_PP_MAP(CS,COS_CLS_RANK)),0,0,0,0,0) }, \
   /* reference to function */ \
   COS_FCT_NAME(NAME,CS), \
   /* references to classes of specialization */ \
   { COS_PP_SEQ(COS_PP_MAP(CS,COS_MTH_CLS)) } \
};

// next_method definition
#define COS_MTH_NEXTDEF(RET,NAME,PS,CS,C) \
  /* next_method type */ \
  typedef void (*const _cos_mth_nxt) COS_PP_MAP(PS,COS_SIG_NXTM); \
  /* next_method type (direct case) */ \
  typedef void (*const _cos_mth_nxt_d) \
    (SEL,COS_PP_SEQ(COS_PP_MAP(PS,COS_SIG_NXTM)),void*,void*); \
  /* next_method_p type */ \
  typedef COS_PP_CAT(IMP,C) _cos_mth_nxt_t; \
  /* direct or indirect call */ \
  enum { _cos_mth_nxt_ind = !COS_TOK_ISGUM(NAME) }; \
  /* next_method pointer */ \
  static _cos_mth_nxt_t _cos_mth_nxt_p = (_cos_mth_nxt_t)COS_YES; \
  /* next_method selector */ \
  static SEL const _cos_mth_nxt_sel = &COS_GEN_NAME(NAME); \
  /* next_method rank */ \
  static U32 const _cos_mth_nxt_rnk = \
    COS_MTH_INFO(COS_PP_SEQ(COS_PP_MAP(CS,COS_CLS_RANK)),0,0,0,0,0); \
  /* generic next_method pointer */ \
  void (*const next_method) (COS_PP_SEQ(COS_PP_MAP(PS,COS_SIG_NXT)), \
                             SEL, RET*, _cos_mth_nxt_t) = COS_NXT_NAME(NAME);

// self type
#define COS_MTH_SLF(C,T) \
  typedef struct C* const T;

// argument initialization
#define COS_MTH_ARG(a) \
  COS_PP_IF(COS_PP_ISBLANK(COS_PRM_NAME(a)))(/* unused */, \
  COS_PRM_TYPE(a) COS_PRM_NAME(a) /* declaration */ \
  COS_PP_IF(COS_TOK_ISVALIST(COS_PRM_TYPE(a)))( /* va_list */ \
    ; va_copy(COS_PRM_NAME(a),((_cos_mth_arg)_arg)->COS_PRM_NAME(a)), \
    =                         ((_cos_mth_arg)_arg)->COS_PRM_NAME(a) ); )

// argument deinitialization
#define COS_MTH_DEARG(a) \
  COS_PP_IF(COS_PP_ISBLANK(COS_PRM_NAME(a)))(/* unused */, \
  COS_PP_IF(COS_TOK_ISVALIST(COS_PRM_TYPE(a)))( \
    va_end(COS_PRM_NAME(a));, /* not va_list */) )

// class reference
#define COS_MTH_CLS(NAME) \
  (&COS_CLS_NAME(NAME))

/* retmethod
 */
#define COS_MTH_RET(...) \
  do { \
    COS_PP_IF(COS_PP_NOARG(__VA_ARGS__))(,COS_MTH_RETVAL = (__VA_ARGS__);) \
    goto _cos_ctr_end; \
  } while(0)

#define COS_MTH_RETVAL (*(_cos_mth_ret)_ret)

/* next_method
 */
#define COS_MTH_NXT(...) \
  /* compile-time check: next_method arguments */ \
  (0 ? ((_cos_mth_nxt)next_method)(__VA_ARGS__) : \
  ( COS_MTH_NXT_INIT, _cos_mth_nxt_ind ? \
  /* indirect invocation */ \
   next_method(__VA_ARGS__,_sel,_ret,_cos_mth_nxt_p) : \
  /* direct invocation */ \
   ((_cos_mth_nxt_d)_cos_mth_nxt_p)(_sel,__VA_ARGS__,_arg,_ret)))

#define COS_MTH_NXT_P \
  /* next_method check */ \
  ( COS_MTH_NXT_INIT, _cos_mth_nxt_p != 0 )

#define COS_MTH_NXT_INIT \
  /* next_method initialization */ \
  (_cos_mth_nxt_p != (_cos_mth_nxt_t)COS_YES ? (void)0 : \
   cos_method_nextInit((FUNC*)(void*)&_cos_mth_nxt_p, \
                       _cos_mth_nxt_sel,_cos_mth_nxt_rnk,_cos_mth_nxt_cls))

/* forward_message
*/
#define COS_MTH_FWD(...) ( \
  /* method lookup */ \
  COS_PP_CAT_NARG(cos_method_fastLookup,__VA_ARGS__) \
  /* lookup selectors */ \
  (_sel,COS_PP_SEQ(COS_PP_MAP((__VA_ARGS__),cos_any_id))) \
  /* method invocation */ \
  (_sel,__VA_ARGS__,_arg,_ret) )

/* method info encoding and decoding
 */
#define COS_MTH_INFO(...) \
  COS_MTH_INFO_0(__VA_ARGS__)

#define COS_MTH_INFO_0(C1,C2,C3,C4,C5,...) \
  ((U32)((C1)+(C2)+(C3)+(C4)+(C5)) << 25 | \
   (U32) (C1)                      << 20 | \
   (U32)      (C2)                 << 15 | \
   (U32)           (C3)            << 10 | \
   (U32)                (C4)       <<  5 | \
   (U32)                     (C5))

#define COS_MTH_RNK(mth) ((mth)->info >> 25 & 0x7F)
#define COS_MTH_RK1(mth) ((mth)->info >> 20 & 0x1F)
#define COS_MTH_RK2(mth) ((mth)->info >> 15 & 0x1F)
#define COS_MTH_RK3(mth) ((mth)->info >> 10 & 0x1F)
#define COS_MTH_RK4(mth) ((mth)->info >>  5 & 0x1F)
#define COS_MTH_RK5(mth) ((mth)->info       & 0x1F)

#endif // COS_COS_METHOD_H
