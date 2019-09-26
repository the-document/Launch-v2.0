

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 10:14:07 2038
 */
/* Compiler settings for Launchv2.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0622 
    protocol : all , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */


#ifndef __Launchv2_h_h__
#define __Launchv2_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ILaunchv2_FWD_DEFINED__
#define __ILaunchv2_FWD_DEFINED__
typedef interface ILaunchv2 ILaunchv2;

#endif 	/* __ILaunchv2_FWD_DEFINED__ */


#ifndef __Launchv2_FWD_DEFINED__
#define __Launchv2_FWD_DEFINED__

#ifdef __cplusplus
typedef class Launchv2 Launchv2;
#else
typedef struct Launchv2 Launchv2;
#endif /* __cplusplus */

#endif 	/* __Launchv2_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __Launchv2_LIBRARY_DEFINED__
#define __Launchv2_LIBRARY_DEFINED__

/* library Launchv2 */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_Launchv2;

#ifndef __ILaunchv2_DISPINTERFACE_DEFINED__
#define __ILaunchv2_DISPINTERFACE_DEFINED__

/* dispinterface ILaunchv2 */
/* [uuid] */ 


EXTERN_C const IID DIID_ILaunchv2;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("3726fdb8-acc8-4026-a117-6cbb131a39da")
    ILaunchv2 : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct ILaunchv2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ILaunchv2 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ILaunchv2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ILaunchv2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ILaunchv2 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ILaunchv2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ILaunchv2 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ILaunchv2 * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } ILaunchv2Vtbl;

    interface ILaunchv2
    {
        CONST_VTBL struct ILaunchv2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILaunchv2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ILaunchv2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ILaunchv2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ILaunchv2_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ILaunchv2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ILaunchv2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ILaunchv2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __ILaunchv2_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_Launchv2;

#ifdef __cplusplus

class DECLSPEC_UUID("38b7922c-a23a-4b85-8ecf-29ef0cbd4369")
Launchv2;
#endif
#endif /* __Launchv2_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


