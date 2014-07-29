

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Feb 03 17:48:00 2011
 */
/* Compiler settings for UltraMon.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __UltraMon_h__
#define __UltraMon_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IUltraMonSystem_FWD_DEFINED__
#define __IUltraMonSystem_FWD_DEFINED__
typedef interface IUltraMonSystem IUltraMonSystem;
#endif 	/* __IUltraMonSystem_FWD_DEFINED__ */


#ifndef __IUltraMonMonitors_FWD_DEFINED__
#define __IUltraMonMonitors_FWD_DEFINED__
typedef interface IUltraMonMonitors IUltraMonMonitors;
#endif 	/* __IUltraMonMonitors_FWD_DEFINED__ */


#ifndef __IUltraMonMonitor_FWD_DEFINED__
#define __IUltraMonMonitor_FWD_DEFINED__
typedef interface IUltraMonMonitor IUltraMonMonitor;
#endif 	/* __IUltraMonMonitor_FWD_DEFINED__ */


#ifndef __IUltraMonDisplayModes_FWD_DEFINED__
#define __IUltraMonDisplayModes_FWD_DEFINED__
typedef interface IUltraMonDisplayModes IUltraMonDisplayModes;
#endif 	/* __IUltraMonDisplayModes_FWD_DEFINED__ */


#ifndef __IUltraMonDisplayMode_FWD_DEFINED__
#define __IUltraMonDisplayMode_FWD_DEFINED__
typedef interface IUltraMonDisplayMode IUltraMonDisplayMode;
#endif 	/* __IUltraMonDisplayMode_FWD_DEFINED__ */


#ifndef __IUltraMonAppBars_FWD_DEFINED__
#define __IUltraMonAppBars_FWD_DEFINED__
typedef interface IUltraMonAppBars IUltraMonAppBars;
#endif 	/* __IUltraMonAppBars_FWD_DEFINED__ */


#ifndef __IUltraMonWindows_FWD_DEFINED__
#define __IUltraMonWindows_FWD_DEFINED__
typedef interface IUltraMonWindows IUltraMonWindows;
#endif 	/* __IUltraMonWindows_FWD_DEFINED__ */


#ifndef __IUltraMonSystem2_FWD_DEFINED__
#define __IUltraMonSystem2_FWD_DEFINED__
typedef interface IUltraMonSystem2 IUltraMonSystem2;
#endif 	/* __IUltraMonSystem2_FWD_DEFINED__ */


#ifndef __IUltraMonMonitor2_FWD_DEFINED__
#define __IUltraMonMonitor2_FWD_DEFINED__
typedef interface IUltraMonMonitor2 IUltraMonMonitor2;
#endif 	/* __IUltraMonMonitor2_FWD_DEFINED__ */


#ifndef __IUltraMonWindow_FWD_DEFINED__
#define __IUltraMonWindow_FWD_DEFINED__
typedef interface IUltraMonWindow IUltraMonWindow;
#endif 	/* __IUltraMonWindow_FWD_DEFINED__ */


#ifndef __IUltraMonWindow2_FWD_DEFINED__
#define __IUltraMonWindow2_FWD_DEFINED__
typedef interface IUltraMonWindow2 IUltraMonWindow2;
#endif 	/* __IUltraMonWindow2_FWD_DEFINED__ */


#ifndef __IUltraMonUtility_FWD_DEFINED__
#define __IUltraMonUtility_FWD_DEFINED__
typedef interface IUltraMonUtility IUltraMonUtility;
#endif 	/* __IUltraMonUtility_FWD_DEFINED__ */


#ifndef __IUltraMonUtility2_FWD_DEFINED__
#define __IUltraMonUtility2_FWD_DEFINED__
typedef interface IUltraMonUtility2 IUltraMonUtility2;
#endif 	/* __IUltraMonUtility2_FWD_DEFINED__ */


#ifndef __IUltraMonMonCtlEvents_FWD_DEFINED__
#define __IUltraMonMonCtlEvents_FWD_DEFINED__
typedef interface IUltraMonMonCtlEvents IUltraMonMonCtlEvents;
#endif 	/* __IUltraMonMonCtlEvents_FWD_DEFINED__ */


#ifndef __IUltraMonMonitorsMonCtl_FWD_DEFINED__
#define __IUltraMonMonitorsMonCtl_FWD_DEFINED__
typedef interface IUltraMonMonitorsMonCtl IUltraMonMonitorsMonCtl;
#endif 	/* __IUltraMonMonitorsMonCtl_FWD_DEFINED__ */


#ifndef __IUltraMonMonCtl_FWD_DEFINED__
#define __IUltraMonMonCtl_FWD_DEFINED__
typedef interface IUltraMonMonCtl IUltraMonMonCtl;
#endif 	/* __IUltraMonMonCtl_FWD_DEFINED__ */


#ifndef __IUltraMonMonCtl2_FWD_DEFINED__
#define __IUltraMonMonCtl2_FWD_DEFINED__
typedef interface IUltraMonMonCtl2 IUltraMonMonCtl2;
#endif 	/* __IUltraMonMonCtl2_FWD_DEFINED__ */


#ifndef __IUltraMonMonitorMonCtl_FWD_DEFINED__
#define __IUltraMonMonitorMonCtl_FWD_DEFINED__
typedef interface IUltraMonMonitorMonCtl IUltraMonMonitorMonCtl;
#endif 	/* __IUltraMonMonitorMonCtl_FWD_DEFINED__ */


#ifndef __IUltraMonMonCtlEvents2_FWD_DEFINED__
#define __IUltraMonMonCtlEvents2_FWD_DEFINED__
typedef interface IUltraMonMonCtlEvents2 IUltraMonMonCtlEvents2;
#endif 	/* __IUltraMonMonCtlEvents2_FWD_DEFINED__ */


#ifndef __IUltraMonDisplayMode2_FWD_DEFINED__
#define __IUltraMonDisplayMode2_FWD_DEFINED__
typedef interface IUltraMonDisplayMode2 IUltraMonDisplayMode2;
#endif 	/* __IUltraMonDisplayMode2_FWD_DEFINED__ */


#ifndef __IUltraMonAppBar_FWD_DEFINED__
#define __IUltraMonAppBar_FWD_DEFINED__
typedef interface IUltraMonAppBar IUltraMonAppBar;
#endif 	/* __IUltraMonAppBar_FWD_DEFINED__ */


#ifndef __UltraMonSystem_FWD_DEFINED__
#define __UltraMonSystem_FWD_DEFINED__

#ifdef __cplusplus
typedef class UltraMonSystem UltraMonSystem;
#else
typedef struct UltraMonSystem UltraMonSystem;
#endif /* __cplusplus */

#endif 	/* __UltraMonSystem_FWD_DEFINED__ */


#ifndef __UltraMonWindow_FWD_DEFINED__
#define __UltraMonWindow_FWD_DEFINED__

#ifdef __cplusplus
typedef class UltraMonWindow UltraMonWindow;
#else
typedef struct UltraMonWindow UltraMonWindow;
#endif /* __cplusplus */

#endif 	/* __UltraMonWindow_FWD_DEFINED__ */


#ifndef __UltraMonUtility_FWD_DEFINED__
#define __UltraMonUtility_FWD_DEFINED__

#ifdef __cplusplus
typedef class UltraMonUtility UltraMonUtility;
#else
typedef struct UltraMonUtility UltraMonUtility;
#endif /* __cplusplus */

#endif 	/* __UltraMonUtility_FWD_DEFINED__ */


#ifndef __UltraMonMonCtl_FWD_DEFINED__
#define __UltraMonMonCtl_FWD_DEFINED__

#ifdef __cplusplus
typedef class UltraMonMonCtl UltraMonMonCtl;
#else
typedef struct UltraMonMonCtl UltraMonMonCtl;
#endif /* __cplusplus */

#endif 	/* __UltraMonMonCtl_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __ULTRAMONLib_LIBRARY_DEFINED__
#define __ULTRAMONLib_LIBRARY_DEFINED__

/* library ULTRAMONLib */
/* [helpstring][version][uuid] */ 








typedef /* [public][v1_enum] */ 
enum __MIDL___MIDL_itf_UltraMon_0000_0000_0001
    {	POS_WINDOWS	= 0x1,
	POS_ICONS	= 0x2,
	POS_DESKTOPITEMS	= 0x4,
	POS_ALL	= ( ( POS_WINDOWS | POS_ICONS )  | POS_DESKTOPITEMS ) 
    } 	POS_FLAGS;

typedef /* [public][v1_enum] */ 
enum __MIDL___MIDL_itf_UltraMon_0000_0000_0002
    {	REFRATE_DEFAULT	= 0,
	REFRATE_OPTIMAL	= -1
    } 	REFRATE;

typedef /* [public][public][public][v1_enum] */ 
enum __MIDL___MIDL_itf_UltraMon_0000_0000_0003
    {	SHOWSTATE_HIDDEN	= 0,
	SHOWSTATE_MINIMIZED	= ( SHOWSTATE_HIDDEN + 1 ) ,
	SHOWSTATE_NORMAL	= ( SHOWSTATE_MINIMIZED + 1 ) ,
	SHOWSTATE_MAXIMIZED	= ( SHOWSTATE_NORMAL + 1 ) ,
	SHOWSTATE_MAXIMIZED_DESKTOP	= ( SHOWSTATE_MAXIMIZED + 1 ) 
    } 	SHOWSTATE;

typedef /* [public][v1_enum] */ 
enum __MIDL___MIDL_itf_UltraMon_0000_0000_0004
    {	WNDCHANGE_RESIZE_TO_FIT	= 0x1,
	WNDCHANGE_CLIP_TO_WORKSPACE	= 0x2,
	WNDCHANGE_DONT_VALIDATE_POSITION	= 0x4
    } 	WNDCHANGE;

typedef /* [public][v1_enum] */ 
enum __MIDL___MIDL_itf_UltraMon_0000_0000_0005
    {	FINDWND_TITLE	= 0x1,
	FINDWND_CLASS	= 0x2
    } 	FINDWND;

typedef /* [public][v1_enum] */ 
enum __MIDL___MIDL_itf_UltraMon_0000_0000_0006
    {	MOVEMONITOR_NEXT	= -1,
	MOVEMONITOR_PREV	= -2
    } 	MOVEMONITOR;

typedef /* [public][public][public][v1_enum] */ 
enum __MIDL___MIDL_itf_UltraMon_0000_0000_0007
    {	DRAWMODE_DEFAULT	= 0,
	DRAWMODE_CUSTOM	= 1,
	DRAWMODE_CUSTOM_ONLY_ENABLED	= 2
    } 	DRAWMODE;

typedef /* [public][public][v1_enum] */ 
enum __MIDL___MIDL_itf_UltraMon_0000_0000_0008
    {	REDRAW_ALL	= 0,
	REDRAW_MONITOR_INTERIOR	= 1
    } 	REDRAW;

typedef /* [public][v1_enum] */ 
enum __MIDL___MIDL_itf_UltraMon_0000_0000_0009
    {	MONCTL_EVENTS_DRAW	= 0x1,
	MONCTL_EVENTS_SELCHANGE	= 0x2,
	MONCTL_EVENTS_MOVE	= 0x4,
	MONCTL_EVENTS_MOUSE_CLICK	= 0x8
    } 	MONCTL_EVENTS;

typedef /* [public][v1_enum] */ 
enum __MIDL___MIDL_itf_UltraMon_0000_0000_0010
    {	STYLE_FLAT	= 0x1,
	STYLE_SUNKEN	= 0x2
    } 	STYLE;

typedef /* [public][public][public][v1_enum] */ 
enum __MIDL___MIDL_itf_UltraMon_0000_0000_0011
    {	HWACCEL_FULL	= 0,
	HWACCEL_NO_POINTER_BITMAP	= 1,
	HWACCEL_NO_ADVANCED	= 2,
	HWACCEL_NO_DIRECTX	= 3,
	HWACCEL_ONLY_BASIC	= 4,
	HWACCEL_NONE	= 5
    } 	HWACCEL;

typedef /* [public][public][public][v1_enum] */ 
enum __MIDL___MIDL_itf_UltraMon_0000_0000_0012
    {	EDGE_LEFT	= 0,
	EDGE_TOP	= 1,
	EDGE_RIGHT	= 2,
	EDGE_BOTTOM	= 3
    } 	SCREEN_EDGE;

typedef /* [public][public][v1_enum] */ 
enum __MIDL___MIDL_itf_UltraMon_0000_0000_0013
    {	MOUSE_BTN_LEFT	= 0,
	MOUSE_BTN_RIGHT	= 1,
	MOUSE_BTN_MIDDLE	= 2,
	MOUSE_BTN_X1	= 3,
	MOUSE_BTN_X2	= 4
    } 	MOUSE_BTN;

typedef /* [public][v1_enum] */ 
enum __MIDL___MIDL_itf_UltraMon_0000_0000_0014
    {	MOD_KEY_CTRL	= 0x1,
	MOD_KEY_SHIFT	= 0x2,
	MOD_KEY_ALT	= 0x4
    } 	MOD_KEY;

typedef /* [public][public][public][v1_enum] */ 
enum __MIDL___MIDL_itf_UltraMon_0000_0000_0015
    {	OR_LANDSCAPE	= 0,
	OR_PORTRAIT	= 1,
	OR_LANDSCAPE_FLIPPED	= 2,
	OR_PORTRAIT_FLIPPED	= 3
    } 	ORIENTATION;

typedef /* [public][public][v1_enum] */ 
enum __MIDL___MIDL_itf_UltraMon_0000_0000_0016
    {	CONNTYPE_UNKNOWN	= 0,
	CONNTYPE_VGA	= 1,
	CONNTYPE_DVI	= 2,
	CONNTYPE_HDMI	= 3,
	CONNTYPE_COMPOSITE	= 4,
	CONNTYPE_SVIDEO	= 5,
	CONNTYPE_COMPONENT	= 6,
	CONNTYPE_INTERNAL	= 7,
	CONNTYPE_DISPLAYPORT_EXTERNAL	= 8,
	CONNTYPE_DISPLAYPORT_EMBEDDED	= 9,
	CONNTYPE_UDI_EXTERNAL	= 10,
	CONNTYPE_UDI_EMBEDDED	= 11,
	CONNTYPE_SDI	= 12,
	CONNTYPE_LVDS	= 13,
	CONNTYPE_D_JPN	= 14,
	CONNTYPE_SDTV_DONGLE	= 15
    } 	CONNECTOR_TYPE;





EXTERN_C const IID LIBID_ULTRAMONLib;

#ifndef __IUltraMonSystem_INTERFACE_DEFINED__
#define __IUltraMonSystem_INTERFACE_DEFINED__

/* interface IUltraMonSystem */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IUltraMonSystem;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6030C057-9A97-4F89-8963-46E308481295")
    IUltraMonSystem : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ApplyMonitorChanges( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SavePositions( 
            /* [in] */ long flags) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RestorePositions( 
            /* [in] */ long flags) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SecondaryDisable( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SecondaryEnable( void) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_MultimonOS( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_MultimonSystem( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_NumActiveMonitors( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DesktopLeft( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DesktopTop( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DesktopWidth( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DesktopHeight( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Monitors( 
            /* [retval][out] */ IUltraMonMonitors **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DirectDrawAcceleration( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_DirectDrawAcceleration( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_IgnoredMonitors( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetIgnoredMonitors( 
            long ignoredMonitors,
            VARIANT_BOOL save) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DockedAppBars( 
            /* [retval][out] */ IUltraMonAppBars **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_WorkspaceLeft( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_WorkspaceTop( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_WorkspaceWidth( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_WorkspaceHeight( 
            /* [retval][out] */ long *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IUltraMonSystemVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IUltraMonSystem * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IUltraMonSystem * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IUltraMonSystem * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IUltraMonSystem * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IUltraMonSystem * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IUltraMonSystem * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IUltraMonSystem * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ApplyMonitorChanges )( 
            IUltraMonSystem * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SavePositions )( 
            IUltraMonSystem * This,
            /* [in] */ long flags);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RestorePositions )( 
            IUltraMonSystem * This,
            /* [in] */ long flags);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SecondaryDisable )( 
            IUltraMonSystem * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SecondaryEnable )( 
            IUltraMonSystem * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MultimonOS )( 
            IUltraMonSystem * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MultimonSystem )( 
            IUltraMonSystem * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_NumActiveMonitors )( 
            IUltraMonSystem * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DesktopLeft )( 
            IUltraMonSystem * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DesktopTop )( 
            IUltraMonSystem * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DesktopWidth )( 
            IUltraMonSystem * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DesktopHeight )( 
            IUltraMonSystem * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Monitors )( 
            IUltraMonSystem * This,
            /* [retval][out] */ IUltraMonMonitors **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DirectDrawAcceleration )( 
            IUltraMonSystem * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_DirectDrawAcceleration )( 
            IUltraMonSystem * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_IgnoredMonitors )( 
            IUltraMonSystem * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetIgnoredMonitors )( 
            IUltraMonSystem * This,
            long ignoredMonitors,
            VARIANT_BOOL save);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DockedAppBars )( 
            IUltraMonSystem * This,
            /* [retval][out] */ IUltraMonAppBars **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_WorkspaceLeft )( 
            IUltraMonSystem * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_WorkspaceTop )( 
            IUltraMonSystem * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_WorkspaceWidth )( 
            IUltraMonSystem * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_WorkspaceHeight )( 
            IUltraMonSystem * This,
            /* [retval][out] */ long *pVal);
        
        END_INTERFACE
    } IUltraMonSystemVtbl;

    interface IUltraMonSystem
    {
        CONST_VTBL struct IUltraMonSystemVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IUltraMonSystem_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IUltraMonSystem_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IUltraMonSystem_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IUltraMonSystem_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IUltraMonSystem_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IUltraMonSystem_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IUltraMonSystem_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IUltraMonSystem_ApplyMonitorChanges(This)	\
    ( (This)->lpVtbl -> ApplyMonitorChanges(This) ) 

#define IUltraMonSystem_SavePositions(This,flags)	\
    ( (This)->lpVtbl -> SavePositions(This,flags) ) 

#define IUltraMonSystem_RestorePositions(This,flags)	\
    ( (This)->lpVtbl -> RestorePositions(This,flags) ) 

#define IUltraMonSystem_SecondaryDisable(This)	\
    ( (This)->lpVtbl -> SecondaryDisable(This) ) 

#define IUltraMonSystem_SecondaryEnable(This)	\
    ( (This)->lpVtbl -> SecondaryEnable(This) ) 

#define IUltraMonSystem_get_MultimonOS(This,pVal)	\
    ( (This)->lpVtbl -> get_MultimonOS(This,pVal) ) 

#define IUltraMonSystem_get_MultimonSystem(This,pVal)	\
    ( (This)->lpVtbl -> get_MultimonSystem(This,pVal) ) 

#define IUltraMonSystem_get_NumActiveMonitors(This,pVal)	\
    ( (This)->lpVtbl -> get_NumActiveMonitors(This,pVal) ) 

#define IUltraMonSystem_get_DesktopLeft(This,pVal)	\
    ( (This)->lpVtbl -> get_DesktopLeft(This,pVal) ) 

#define IUltraMonSystem_get_DesktopTop(This,pVal)	\
    ( (This)->lpVtbl -> get_DesktopTop(This,pVal) ) 

#define IUltraMonSystem_get_DesktopWidth(This,pVal)	\
    ( (This)->lpVtbl -> get_DesktopWidth(This,pVal) ) 

#define IUltraMonSystem_get_DesktopHeight(This,pVal)	\
    ( (This)->lpVtbl -> get_DesktopHeight(This,pVal) ) 

#define IUltraMonSystem_get_Monitors(This,pVal)	\
    ( (This)->lpVtbl -> get_Monitors(This,pVal) ) 

#define IUltraMonSystem_get_DirectDrawAcceleration(This,pVal)	\
    ( (This)->lpVtbl -> get_DirectDrawAcceleration(This,pVal) ) 

#define IUltraMonSystem_put_DirectDrawAcceleration(This,newVal)	\
    ( (This)->lpVtbl -> put_DirectDrawAcceleration(This,newVal) ) 

#define IUltraMonSystem_get_IgnoredMonitors(This,pVal)	\
    ( (This)->lpVtbl -> get_IgnoredMonitors(This,pVal) ) 

#define IUltraMonSystem_SetIgnoredMonitors(This,ignoredMonitors,save)	\
    ( (This)->lpVtbl -> SetIgnoredMonitors(This,ignoredMonitors,save) ) 

#define IUltraMonSystem_get_DockedAppBars(This,pVal)	\
    ( (This)->lpVtbl -> get_DockedAppBars(This,pVal) ) 

#define IUltraMonSystem_get_WorkspaceLeft(This,pVal)	\
    ( (This)->lpVtbl -> get_WorkspaceLeft(This,pVal) ) 

#define IUltraMonSystem_get_WorkspaceTop(This,pVal)	\
    ( (This)->lpVtbl -> get_WorkspaceTop(This,pVal) ) 

#define IUltraMonSystem_get_WorkspaceWidth(This,pVal)	\
    ( (This)->lpVtbl -> get_WorkspaceWidth(This,pVal) ) 

#define IUltraMonSystem_get_WorkspaceHeight(This,pVal)	\
    ( (This)->lpVtbl -> get_WorkspaceHeight(This,pVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IUltraMonSystem_INTERFACE_DEFINED__ */


#ifndef __IUltraMonMonitors_INTERFACE_DEFINED__
#define __IUltraMonMonitors_INTERFACE_DEFINED__

/* interface IUltraMonMonitors */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IUltraMonMonitors;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("4357CF2F-C5A2-4777-A108-839CE399E676")
    IUltraMonMonitors : public IDispatch
    {
    public:
        virtual /* [custom][helpstring][id] */ HRESULT STDMETHODCALLTYPE Item( 
            /* [in] */ VARIANT index,
            /* [retval][out] */ IUltraMonMonitor **ppMonitor) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][restricted][propget] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown **pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IUltraMonMonitorsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IUltraMonMonitors * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IUltraMonMonitors * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IUltraMonMonitors * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IUltraMonMonitors * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IUltraMonMonitors * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IUltraMonMonitors * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IUltraMonMonitors * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [custom][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Item )( 
            IUltraMonMonitors * This,
            /* [in] */ VARIANT index,
            /* [retval][out] */ IUltraMonMonitor **ppMonitor);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IUltraMonMonitors * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][restricted][propget] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            IUltraMonMonitors * This,
            /* [retval][out] */ IUnknown **pVal);
        
        END_INTERFACE
    } IUltraMonMonitorsVtbl;

    interface IUltraMonMonitors
    {
        CONST_VTBL struct IUltraMonMonitorsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IUltraMonMonitors_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IUltraMonMonitors_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IUltraMonMonitors_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IUltraMonMonitors_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IUltraMonMonitors_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IUltraMonMonitors_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IUltraMonMonitors_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IUltraMonMonitors_Item(This,index,ppMonitor)	\
    ( (This)->lpVtbl -> Item(This,index,ppMonitor) ) 

#define IUltraMonMonitors_get_Count(This,pVal)	\
    ( (This)->lpVtbl -> get_Count(This,pVal) ) 

#define IUltraMonMonitors_get__NewEnum(This,pVal)	\
    ( (This)->lpVtbl -> get__NewEnum(This,pVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IUltraMonMonitors_INTERFACE_DEFINED__ */


#ifndef __IUltraMonMonitor_INTERFACE_DEFINED__
#define __IUltraMonMonitor_INTERFACE_DEFINED__

/* interface IUltraMonMonitor */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IUltraMonMonitor;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("697A1A9F-F7EE-45d2-B4A3-C87B7308F317")
    IUltraMonMonitor : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ID( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DeviceName( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_AdapterName( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_AdapterDeviceName( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Primary( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Primary( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Enabled( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Enabled( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Left( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Left( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Top( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Top( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Width( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Width( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Height( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Height( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_WorkLeft( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_WorkTop( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_WorkWidth( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_WorkHeight( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Colordepth( 
            /* [retval][out] */ short *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Colordepth( 
            /* [in] */ short newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_RefreshRate( 
            /* [retval][out] */ short *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_RefreshRate( 
            /* [in] */ short newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_HMonitor( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DeviceRegKey( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DisplayModes( 
            /* [retval][out] */ IUltraMonDisplayModes **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_HwAccel( 
            /* [retval][out] */ HWACCEL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_HwAccel( 
            /* [in] */ HWACCEL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Removable( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Detached( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_WindowsID( 
            /* [retval][out] */ long *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IUltraMonMonitorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IUltraMonMonitor * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IUltraMonMonitor * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IUltraMonMonitor * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IUltraMonMonitor * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IUltraMonMonitor * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IUltraMonMonitor * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IUltraMonMonitor * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ID )( 
            IUltraMonMonitor * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IUltraMonMonitor * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DeviceName )( 
            IUltraMonMonitor * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_AdapterName )( 
            IUltraMonMonitor * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_AdapterDeviceName )( 
            IUltraMonMonitor * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Primary )( 
            IUltraMonMonitor * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Primary )( 
            IUltraMonMonitor * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Enabled )( 
            IUltraMonMonitor * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Enabled )( 
            IUltraMonMonitor * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Left )( 
            IUltraMonMonitor * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Left )( 
            IUltraMonMonitor * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Top )( 
            IUltraMonMonitor * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Top )( 
            IUltraMonMonitor * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Width )( 
            IUltraMonMonitor * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Width )( 
            IUltraMonMonitor * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Height )( 
            IUltraMonMonitor * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Height )( 
            IUltraMonMonitor * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_WorkLeft )( 
            IUltraMonMonitor * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_WorkTop )( 
            IUltraMonMonitor * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_WorkWidth )( 
            IUltraMonMonitor * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_WorkHeight )( 
            IUltraMonMonitor * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Colordepth )( 
            IUltraMonMonitor * This,
            /* [retval][out] */ short *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Colordepth )( 
            IUltraMonMonitor * This,
            /* [in] */ short newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RefreshRate )( 
            IUltraMonMonitor * This,
            /* [retval][out] */ short *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_RefreshRate )( 
            IUltraMonMonitor * This,
            /* [in] */ short newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HMonitor )( 
            IUltraMonMonitor * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DeviceRegKey )( 
            IUltraMonMonitor * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DisplayModes )( 
            IUltraMonMonitor * This,
            /* [retval][out] */ IUltraMonDisplayModes **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HwAccel )( 
            IUltraMonMonitor * This,
            /* [retval][out] */ HWACCEL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_HwAccel )( 
            IUltraMonMonitor * This,
            /* [in] */ HWACCEL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Removable )( 
            IUltraMonMonitor * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Detached )( 
            IUltraMonMonitor * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_WindowsID )( 
            IUltraMonMonitor * This,
            /* [retval][out] */ long *pVal);
        
        END_INTERFACE
    } IUltraMonMonitorVtbl;

    interface IUltraMonMonitor
    {
        CONST_VTBL struct IUltraMonMonitorVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IUltraMonMonitor_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IUltraMonMonitor_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IUltraMonMonitor_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IUltraMonMonitor_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IUltraMonMonitor_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IUltraMonMonitor_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IUltraMonMonitor_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IUltraMonMonitor_get_ID(This,pVal)	\
    ( (This)->lpVtbl -> get_ID(This,pVal) ) 

#define IUltraMonMonitor_get_Name(This,pVal)	\
    ( (This)->lpVtbl -> get_Name(This,pVal) ) 

#define IUltraMonMonitor_get_DeviceName(This,pVal)	\
    ( (This)->lpVtbl -> get_DeviceName(This,pVal) ) 

#define IUltraMonMonitor_get_AdapterName(This,pVal)	\
    ( (This)->lpVtbl -> get_AdapterName(This,pVal) ) 

#define IUltraMonMonitor_get_AdapterDeviceName(This,pVal)	\
    ( (This)->lpVtbl -> get_AdapterDeviceName(This,pVal) ) 

#define IUltraMonMonitor_get_Primary(This,pVal)	\
    ( (This)->lpVtbl -> get_Primary(This,pVal) ) 

#define IUltraMonMonitor_put_Primary(This,newVal)	\
    ( (This)->lpVtbl -> put_Primary(This,newVal) ) 

#define IUltraMonMonitor_get_Enabled(This,pVal)	\
    ( (This)->lpVtbl -> get_Enabled(This,pVal) ) 

#define IUltraMonMonitor_put_Enabled(This,newVal)	\
    ( (This)->lpVtbl -> put_Enabled(This,newVal) ) 

#define IUltraMonMonitor_get_Left(This,pVal)	\
    ( (This)->lpVtbl -> get_Left(This,pVal) ) 

#define IUltraMonMonitor_put_Left(This,newVal)	\
    ( (This)->lpVtbl -> put_Left(This,newVal) ) 

#define IUltraMonMonitor_get_Top(This,pVal)	\
    ( (This)->lpVtbl -> get_Top(This,pVal) ) 

#define IUltraMonMonitor_put_Top(This,newVal)	\
    ( (This)->lpVtbl -> put_Top(This,newVal) ) 

#define IUltraMonMonitor_get_Width(This,pVal)	\
    ( (This)->lpVtbl -> get_Width(This,pVal) ) 

#define IUltraMonMonitor_put_Width(This,newVal)	\
    ( (This)->lpVtbl -> put_Width(This,newVal) ) 

#define IUltraMonMonitor_get_Height(This,pVal)	\
    ( (This)->lpVtbl -> get_Height(This,pVal) ) 

#define IUltraMonMonitor_put_Height(This,newVal)	\
    ( (This)->lpVtbl -> put_Height(This,newVal) ) 

#define IUltraMonMonitor_get_WorkLeft(This,pVal)	\
    ( (This)->lpVtbl -> get_WorkLeft(This,pVal) ) 

#define IUltraMonMonitor_get_WorkTop(This,pVal)	\
    ( (This)->lpVtbl -> get_WorkTop(This,pVal) ) 

#define IUltraMonMonitor_get_WorkWidth(This,pVal)	\
    ( (This)->lpVtbl -> get_WorkWidth(This,pVal) ) 

#define IUltraMonMonitor_get_WorkHeight(This,pVal)	\
    ( (This)->lpVtbl -> get_WorkHeight(This,pVal) ) 

#define IUltraMonMonitor_get_Colordepth(This,pVal)	\
    ( (This)->lpVtbl -> get_Colordepth(This,pVal) ) 

#define IUltraMonMonitor_put_Colordepth(This,newVal)	\
    ( (This)->lpVtbl -> put_Colordepth(This,newVal) ) 

#define IUltraMonMonitor_get_RefreshRate(This,pVal)	\
    ( (This)->lpVtbl -> get_RefreshRate(This,pVal) ) 

#define IUltraMonMonitor_put_RefreshRate(This,newVal)	\
    ( (This)->lpVtbl -> put_RefreshRate(This,newVal) ) 

#define IUltraMonMonitor_get_HMonitor(This,pVal)	\
    ( (This)->lpVtbl -> get_HMonitor(This,pVal) ) 

#define IUltraMonMonitor_get_DeviceRegKey(This,pVal)	\
    ( (This)->lpVtbl -> get_DeviceRegKey(This,pVal) ) 

#define IUltraMonMonitor_get_DisplayModes(This,pVal)	\
    ( (This)->lpVtbl -> get_DisplayModes(This,pVal) ) 

#define IUltraMonMonitor_get_HwAccel(This,pVal)	\
    ( (This)->lpVtbl -> get_HwAccel(This,pVal) ) 

#define IUltraMonMonitor_put_HwAccel(This,newVal)	\
    ( (This)->lpVtbl -> put_HwAccel(This,newVal) ) 

#define IUltraMonMonitor_get_Removable(This,pVal)	\
    ( (This)->lpVtbl -> get_Removable(This,pVal) ) 

#define IUltraMonMonitor_get_Detached(This,pVal)	\
    ( (This)->lpVtbl -> get_Detached(This,pVal) ) 

#define IUltraMonMonitor_get_WindowsID(This,pVal)	\
    ( (This)->lpVtbl -> get_WindowsID(This,pVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IUltraMonMonitor_INTERFACE_DEFINED__ */


#ifndef __IUltraMonDisplayModes_INTERFACE_DEFINED__
#define __IUltraMonDisplayModes_INTERFACE_DEFINED__

/* interface IUltraMonDisplayModes */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IUltraMonDisplayModes;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("E4582122-0593-4396-80CB-85BC4B3BB92D")
    IUltraMonDisplayModes : public IDispatch
    {
    public:
        virtual /* [custom][helpstring][id] */ HRESULT STDMETHODCALLTYPE Item( 
            /* [in] */ VARIANT index,
            /* [retval][out] */ IUltraMonDisplayMode **ppDisplayMode) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][restricted][propget] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown **pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IUltraMonDisplayModesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IUltraMonDisplayModes * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IUltraMonDisplayModes * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IUltraMonDisplayModes * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IUltraMonDisplayModes * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IUltraMonDisplayModes * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IUltraMonDisplayModes * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IUltraMonDisplayModes * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [custom][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Item )( 
            IUltraMonDisplayModes * This,
            /* [in] */ VARIANT index,
            /* [retval][out] */ IUltraMonDisplayMode **ppDisplayMode);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IUltraMonDisplayModes * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][restricted][propget] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            IUltraMonDisplayModes * This,
            /* [retval][out] */ IUnknown **pVal);
        
        END_INTERFACE
    } IUltraMonDisplayModesVtbl;

    interface IUltraMonDisplayModes
    {
        CONST_VTBL struct IUltraMonDisplayModesVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IUltraMonDisplayModes_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IUltraMonDisplayModes_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IUltraMonDisplayModes_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IUltraMonDisplayModes_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IUltraMonDisplayModes_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IUltraMonDisplayModes_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IUltraMonDisplayModes_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IUltraMonDisplayModes_Item(This,index,ppDisplayMode)	\
    ( (This)->lpVtbl -> Item(This,index,ppDisplayMode) ) 

#define IUltraMonDisplayModes_get_Count(This,pVal)	\
    ( (This)->lpVtbl -> get_Count(This,pVal) ) 

#define IUltraMonDisplayModes_get__NewEnum(This,pVal)	\
    ( (This)->lpVtbl -> get__NewEnum(This,pVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IUltraMonDisplayModes_INTERFACE_DEFINED__ */


#ifndef __IUltraMonDisplayMode_INTERFACE_DEFINED__
#define __IUltraMonDisplayMode_INTERFACE_DEFINED__

/* interface IUltraMonDisplayMode */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IUltraMonDisplayMode;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("466057DB-B9D5-4739-BEDB-E14145EE0F3C")
    IUltraMonDisplayMode : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Width( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Height( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ColorDepth( 
            /* [retval][out] */ VARIANT *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_RefreshRate( 
            /* [retval][out] */ VARIANT *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IUltraMonDisplayModeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IUltraMonDisplayMode * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IUltraMonDisplayMode * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IUltraMonDisplayMode * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IUltraMonDisplayMode * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IUltraMonDisplayMode * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IUltraMonDisplayMode * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IUltraMonDisplayMode * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Width )( 
            IUltraMonDisplayMode * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Height )( 
            IUltraMonDisplayMode * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ColorDepth )( 
            IUltraMonDisplayMode * This,
            /* [retval][out] */ VARIANT *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RefreshRate )( 
            IUltraMonDisplayMode * This,
            /* [retval][out] */ VARIANT *pVal);
        
        END_INTERFACE
    } IUltraMonDisplayModeVtbl;

    interface IUltraMonDisplayMode
    {
        CONST_VTBL struct IUltraMonDisplayModeVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IUltraMonDisplayMode_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IUltraMonDisplayMode_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IUltraMonDisplayMode_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IUltraMonDisplayMode_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IUltraMonDisplayMode_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IUltraMonDisplayMode_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IUltraMonDisplayMode_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IUltraMonDisplayMode_get_Width(This,pVal)	\
    ( (This)->lpVtbl -> get_Width(This,pVal) ) 

#define IUltraMonDisplayMode_get_Height(This,pVal)	\
    ( (This)->lpVtbl -> get_Height(This,pVal) ) 

#define IUltraMonDisplayMode_get_ColorDepth(This,pVal)	\
    ( (This)->lpVtbl -> get_ColorDepth(This,pVal) ) 

#define IUltraMonDisplayMode_get_RefreshRate(This,pVal)	\
    ( (This)->lpVtbl -> get_RefreshRate(This,pVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IUltraMonDisplayMode_INTERFACE_DEFINED__ */


#ifndef __IUltraMonAppBars_INTERFACE_DEFINED__
#define __IUltraMonAppBars_INTERFACE_DEFINED__

/* interface IUltraMonAppBars */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IUltraMonAppBars;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("B96517B6-9C8E-4722-B359-FB886E586C7A")
    IUltraMonAppBars : public IDispatch
    {
    public:
        virtual /* [custom][helpstring][id] */ HRESULT STDMETHODCALLTYPE Item( 
            /* [in] */ VARIANT index,
            /* [retval][out] */ IUltraMonAppBar **ppAppBar) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][restricted][propget] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown **pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Refresh( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IUltraMonAppBarsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IUltraMonAppBars * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IUltraMonAppBars * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IUltraMonAppBars * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IUltraMonAppBars * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IUltraMonAppBars * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IUltraMonAppBars * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IUltraMonAppBars * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [custom][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Item )( 
            IUltraMonAppBars * This,
            /* [in] */ VARIANT index,
            /* [retval][out] */ IUltraMonAppBar **ppAppBar);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IUltraMonAppBars * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][restricted][propget] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            IUltraMonAppBars * This,
            /* [retval][out] */ IUnknown **pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Refresh )( 
            IUltraMonAppBars * This);
        
        END_INTERFACE
    } IUltraMonAppBarsVtbl;

    interface IUltraMonAppBars
    {
        CONST_VTBL struct IUltraMonAppBarsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IUltraMonAppBars_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IUltraMonAppBars_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IUltraMonAppBars_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IUltraMonAppBars_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IUltraMonAppBars_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IUltraMonAppBars_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IUltraMonAppBars_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IUltraMonAppBars_Item(This,index,ppAppBar)	\
    ( (This)->lpVtbl -> Item(This,index,ppAppBar) ) 

#define IUltraMonAppBars_get_Count(This,pVal)	\
    ( (This)->lpVtbl -> get_Count(This,pVal) ) 

#define IUltraMonAppBars_get__NewEnum(This,pVal)	\
    ( (This)->lpVtbl -> get__NewEnum(This,pVal) ) 

#define IUltraMonAppBars_Refresh(This)	\
    ( (This)->lpVtbl -> Refresh(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IUltraMonAppBars_INTERFACE_DEFINED__ */


#ifndef __IUltraMonWindows_INTERFACE_DEFINED__
#define __IUltraMonWindows_INTERFACE_DEFINED__

/* interface IUltraMonWindows */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IUltraMonWindows;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("FD7A0C1A-9E42-44ae-AA83-2D244DA01EB8")
    IUltraMonWindows : public IDispatch
    {
    public:
        virtual /* [custom][helpstring][id] */ HRESULT STDMETHODCALLTYPE Item( 
            /* [in] */ VARIANT index,
            /* [retval][out] */ IUltraMonWindow2 **ppWnd) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][restricted][propget] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown **pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IUltraMonWindowsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IUltraMonWindows * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IUltraMonWindows * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IUltraMonWindows * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IUltraMonWindows * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IUltraMonWindows * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IUltraMonWindows * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IUltraMonWindows * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [custom][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Item )( 
            IUltraMonWindows * This,
            /* [in] */ VARIANT index,
            /* [retval][out] */ IUltraMonWindow2 **ppWnd);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IUltraMonWindows * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][restricted][propget] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            IUltraMonWindows * This,
            /* [retval][out] */ IUnknown **pVal);
        
        END_INTERFACE
    } IUltraMonWindowsVtbl;

    interface IUltraMonWindows
    {
        CONST_VTBL struct IUltraMonWindowsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IUltraMonWindows_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IUltraMonWindows_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IUltraMonWindows_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IUltraMonWindows_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IUltraMonWindows_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IUltraMonWindows_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IUltraMonWindows_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IUltraMonWindows_Item(This,index,ppWnd)	\
    ( (This)->lpVtbl -> Item(This,index,ppWnd) ) 

#define IUltraMonWindows_get_Count(This,pVal)	\
    ( (This)->lpVtbl -> get_Count(This,pVal) ) 

#define IUltraMonWindows_get__NewEnum(This,pVal)	\
    ( (This)->lpVtbl -> get__NewEnum(This,pVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IUltraMonWindows_INTERFACE_DEFINED__ */


#ifndef __IUltraMonSystem2_INTERFACE_DEFINED__
#define __IUltraMonSystem2_INTERFACE_DEFINED__

/* interface IUltraMonSystem2 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IUltraMonSystem2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("4EC0850A-E469-4CAB-8214-9A10FEEE62F7")
    IUltraMonSystem2 : public IUltraMonSystem
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CursorPosX( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CursorPosY( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetCursorPos( 
            /* [in] */ long x,
            /* [in] */ long y) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IUltraMonSystem2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IUltraMonSystem2 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IUltraMonSystem2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IUltraMonSystem2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IUltraMonSystem2 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IUltraMonSystem2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IUltraMonSystem2 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IUltraMonSystem2 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ApplyMonitorChanges )( 
            IUltraMonSystem2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SavePositions )( 
            IUltraMonSystem2 * This,
            /* [in] */ long flags);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RestorePositions )( 
            IUltraMonSystem2 * This,
            /* [in] */ long flags);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SecondaryDisable )( 
            IUltraMonSystem2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SecondaryEnable )( 
            IUltraMonSystem2 * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MultimonOS )( 
            IUltraMonSystem2 * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MultimonSystem )( 
            IUltraMonSystem2 * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_NumActiveMonitors )( 
            IUltraMonSystem2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DesktopLeft )( 
            IUltraMonSystem2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DesktopTop )( 
            IUltraMonSystem2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DesktopWidth )( 
            IUltraMonSystem2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DesktopHeight )( 
            IUltraMonSystem2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Monitors )( 
            IUltraMonSystem2 * This,
            /* [retval][out] */ IUltraMonMonitors **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DirectDrawAcceleration )( 
            IUltraMonSystem2 * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_DirectDrawAcceleration )( 
            IUltraMonSystem2 * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_IgnoredMonitors )( 
            IUltraMonSystem2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetIgnoredMonitors )( 
            IUltraMonSystem2 * This,
            long ignoredMonitors,
            VARIANT_BOOL save);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DockedAppBars )( 
            IUltraMonSystem2 * This,
            /* [retval][out] */ IUltraMonAppBars **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_WorkspaceLeft )( 
            IUltraMonSystem2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_WorkspaceTop )( 
            IUltraMonSystem2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_WorkspaceWidth )( 
            IUltraMonSystem2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_WorkspaceHeight )( 
            IUltraMonSystem2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CursorPosX )( 
            IUltraMonSystem2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CursorPosY )( 
            IUltraMonSystem2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetCursorPos )( 
            IUltraMonSystem2 * This,
            /* [in] */ long x,
            /* [in] */ long y);
        
        END_INTERFACE
    } IUltraMonSystem2Vtbl;

    interface IUltraMonSystem2
    {
        CONST_VTBL struct IUltraMonSystem2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IUltraMonSystem2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IUltraMonSystem2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IUltraMonSystem2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IUltraMonSystem2_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IUltraMonSystem2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IUltraMonSystem2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IUltraMonSystem2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IUltraMonSystem2_ApplyMonitorChanges(This)	\
    ( (This)->lpVtbl -> ApplyMonitorChanges(This) ) 

#define IUltraMonSystem2_SavePositions(This,flags)	\
    ( (This)->lpVtbl -> SavePositions(This,flags) ) 

#define IUltraMonSystem2_RestorePositions(This,flags)	\
    ( (This)->lpVtbl -> RestorePositions(This,flags) ) 

#define IUltraMonSystem2_SecondaryDisable(This)	\
    ( (This)->lpVtbl -> SecondaryDisable(This) ) 

#define IUltraMonSystem2_SecondaryEnable(This)	\
    ( (This)->lpVtbl -> SecondaryEnable(This) ) 

#define IUltraMonSystem2_get_MultimonOS(This,pVal)	\
    ( (This)->lpVtbl -> get_MultimonOS(This,pVal) ) 

#define IUltraMonSystem2_get_MultimonSystem(This,pVal)	\
    ( (This)->lpVtbl -> get_MultimonSystem(This,pVal) ) 

#define IUltraMonSystem2_get_NumActiveMonitors(This,pVal)	\
    ( (This)->lpVtbl -> get_NumActiveMonitors(This,pVal) ) 

#define IUltraMonSystem2_get_DesktopLeft(This,pVal)	\
    ( (This)->lpVtbl -> get_DesktopLeft(This,pVal) ) 

#define IUltraMonSystem2_get_DesktopTop(This,pVal)	\
    ( (This)->lpVtbl -> get_DesktopTop(This,pVal) ) 

#define IUltraMonSystem2_get_DesktopWidth(This,pVal)	\
    ( (This)->lpVtbl -> get_DesktopWidth(This,pVal) ) 

#define IUltraMonSystem2_get_DesktopHeight(This,pVal)	\
    ( (This)->lpVtbl -> get_DesktopHeight(This,pVal) ) 

#define IUltraMonSystem2_get_Monitors(This,pVal)	\
    ( (This)->lpVtbl -> get_Monitors(This,pVal) ) 

#define IUltraMonSystem2_get_DirectDrawAcceleration(This,pVal)	\
    ( (This)->lpVtbl -> get_DirectDrawAcceleration(This,pVal) ) 

#define IUltraMonSystem2_put_DirectDrawAcceleration(This,newVal)	\
    ( (This)->lpVtbl -> put_DirectDrawAcceleration(This,newVal) ) 

#define IUltraMonSystem2_get_IgnoredMonitors(This,pVal)	\
    ( (This)->lpVtbl -> get_IgnoredMonitors(This,pVal) ) 

#define IUltraMonSystem2_SetIgnoredMonitors(This,ignoredMonitors,save)	\
    ( (This)->lpVtbl -> SetIgnoredMonitors(This,ignoredMonitors,save) ) 

#define IUltraMonSystem2_get_DockedAppBars(This,pVal)	\
    ( (This)->lpVtbl -> get_DockedAppBars(This,pVal) ) 

#define IUltraMonSystem2_get_WorkspaceLeft(This,pVal)	\
    ( (This)->lpVtbl -> get_WorkspaceLeft(This,pVal) ) 

#define IUltraMonSystem2_get_WorkspaceTop(This,pVal)	\
    ( (This)->lpVtbl -> get_WorkspaceTop(This,pVal) ) 

#define IUltraMonSystem2_get_WorkspaceWidth(This,pVal)	\
    ( (This)->lpVtbl -> get_WorkspaceWidth(This,pVal) ) 

#define IUltraMonSystem2_get_WorkspaceHeight(This,pVal)	\
    ( (This)->lpVtbl -> get_WorkspaceHeight(This,pVal) ) 


#define IUltraMonSystem2_get_CursorPosX(This,pVal)	\
    ( (This)->lpVtbl -> get_CursorPosX(This,pVal) ) 

#define IUltraMonSystem2_get_CursorPosY(This,pVal)	\
    ( (This)->lpVtbl -> get_CursorPosY(This,pVal) ) 

#define IUltraMonSystem2_SetCursorPos(This,x,y)	\
    ( (This)->lpVtbl -> SetCursorPos(This,x,y) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IUltraMonSystem2_INTERFACE_DEFINED__ */


#ifndef __IUltraMonMonitor2_INTERFACE_DEFINED__
#define __IUltraMonMonitor2_INTERFACE_DEFINED__

/* interface IUltraMonMonitor2 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IUltraMonMonitor2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6CC32319-2DA3-4b16-9F18-3583DB804311")
    IUltraMonMonitor2 : public IUltraMonMonitor
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Orientation( 
            /* [retval][out] */ ORIENTATION *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Orientation( 
            /* [in] */ ORIENTATION newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CanChangeOrientation( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Interlaced( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Interlaced( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SerialNumber( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_TargetId( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetID( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_PreferredWidth( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_PreferredHeight( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DisableMons( 
            /* [retval][out] */ VARIANT *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_RealDispModes( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CcdSupport( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ConnectorType( 
            /* [retval][out] */ CONNECTOR_TYPE *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IUltraMonMonitor2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IUltraMonMonitor2 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IUltraMonMonitor2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IUltraMonMonitor2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IUltraMonMonitor2 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IUltraMonMonitor2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IUltraMonMonitor2 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IUltraMonMonitor2 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ID )( 
            IUltraMonMonitor2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IUltraMonMonitor2 * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DeviceName )( 
            IUltraMonMonitor2 * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_AdapterName )( 
            IUltraMonMonitor2 * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_AdapterDeviceName )( 
            IUltraMonMonitor2 * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Primary )( 
            IUltraMonMonitor2 * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Primary )( 
            IUltraMonMonitor2 * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Enabled )( 
            IUltraMonMonitor2 * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Enabled )( 
            IUltraMonMonitor2 * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Left )( 
            IUltraMonMonitor2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Left )( 
            IUltraMonMonitor2 * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Top )( 
            IUltraMonMonitor2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Top )( 
            IUltraMonMonitor2 * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Width )( 
            IUltraMonMonitor2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Width )( 
            IUltraMonMonitor2 * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Height )( 
            IUltraMonMonitor2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Height )( 
            IUltraMonMonitor2 * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_WorkLeft )( 
            IUltraMonMonitor2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_WorkTop )( 
            IUltraMonMonitor2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_WorkWidth )( 
            IUltraMonMonitor2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_WorkHeight )( 
            IUltraMonMonitor2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Colordepth )( 
            IUltraMonMonitor2 * This,
            /* [retval][out] */ short *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Colordepth )( 
            IUltraMonMonitor2 * This,
            /* [in] */ short newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RefreshRate )( 
            IUltraMonMonitor2 * This,
            /* [retval][out] */ short *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_RefreshRate )( 
            IUltraMonMonitor2 * This,
            /* [in] */ short newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HMonitor )( 
            IUltraMonMonitor2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DeviceRegKey )( 
            IUltraMonMonitor2 * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DisplayModes )( 
            IUltraMonMonitor2 * This,
            /* [retval][out] */ IUltraMonDisplayModes **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HwAccel )( 
            IUltraMonMonitor2 * This,
            /* [retval][out] */ HWACCEL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_HwAccel )( 
            IUltraMonMonitor2 * This,
            /* [in] */ HWACCEL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Removable )( 
            IUltraMonMonitor2 * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Detached )( 
            IUltraMonMonitor2 * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_WindowsID )( 
            IUltraMonMonitor2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Orientation )( 
            IUltraMonMonitor2 * This,
            /* [retval][out] */ ORIENTATION *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Orientation )( 
            IUltraMonMonitor2 * This,
            /* [in] */ ORIENTATION newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CanChangeOrientation )( 
            IUltraMonMonitor2 * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Interlaced )( 
            IUltraMonMonitor2 * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Interlaced )( 
            IUltraMonMonitor2 * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SerialNumber )( 
            IUltraMonMonitor2 * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TargetId )( 
            IUltraMonMonitor2 * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetID )( 
            IUltraMonMonitor2 * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PreferredWidth )( 
            IUltraMonMonitor2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PreferredHeight )( 
            IUltraMonMonitor2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DisableMons )( 
            IUltraMonMonitor2 * This,
            /* [retval][out] */ VARIANT *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RealDispModes )( 
            IUltraMonMonitor2 * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CcdSupport )( 
            IUltraMonMonitor2 * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ConnectorType )( 
            IUltraMonMonitor2 * This,
            /* [retval][out] */ CONNECTOR_TYPE *pVal);
        
        END_INTERFACE
    } IUltraMonMonitor2Vtbl;

    interface IUltraMonMonitor2
    {
        CONST_VTBL struct IUltraMonMonitor2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IUltraMonMonitor2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IUltraMonMonitor2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IUltraMonMonitor2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IUltraMonMonitor2_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IUltraMonMonitor2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IUltraMonMonitor2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IUltraMonMonitor2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IUltraMonMonitor2_get_ID(This,pVal)	\
    ( (This)->lpVtbl -> get_ID(This,pVal) ) 

#define IUltraMonMonitor2_get_Name(This,pVal)	\
    ( (This)->lpVtbl -> get_Name(This,pVal) ) 

#define IUltraMonMonitor2_get_DeviceName(This,pVal)	\
    ( (This)->lpVtbl -> get_DeviceName(This,pVal) ) 

#define IUltraMonMonitor2_get_AdapterName(This,pVal)	\
    ( (This)->lpVtbl -> get_AdapterName(This,pVal) ) 

#define IUltraMonMonitor2_get_AdapterDeviceName(This,pVal)	\
    ( (This)->lpVtbl -> get_AdapterDeviceName(This,pVal) ) 

#define IUltraMonMonitor2_get_Primary(This,pVal)	\
    ( (This)->lpVtbl -> get_Primary(This,pVal) ) 

#define IUltraMonMonitor2_put_Primary(This,newVal)	\
    ( (This)->lpVtbl -> put_Primary(This,newVal) ) 

#define IUltraMonMonitor2_get_Enabled(This,pVal)	\
    ( (This)->lpVtbl -> get_Enabled(This,pVal) ) 

#define IUltraMonMonitor2_put_Enabled(This,newVal)	\
    ( (This)->lpVtbl -> put_Enabled(This,newVal) ) 

#define IUltraMonMonitor2_get_Left(This,pVal)	\
    ( (This)->lpVtbl -> get_Left(This,pVal) ) 

#define IUltraMonMonitor2_put_Left(This,newVal)	\
    ( (This)->lpVtbl -> put_Left(This,newVal) ) 

#define IUltraMonMonitor2_get_Top(This,pVal)	\
    ( (This)->lpVtbl -> get_Top(This,pVal) ) 

#define IUltraMonMonitor2_put_Top(This,newVal)	\
    ( (This)->lpVtbl -> put_Top(This,newVal) ) 

#define IUltraMonMonitor2_get_Width(This,pVal)	\
    ( (This)->lpVtbl -> get_Width(This,pVal) ) 

#define IUltraMonMonitor2_put_Width(This,newVal)	\
    ( (This)->lpVtbl -> put_Width(This,newVal) ) 

#define IUltraMonMonitor2_get_Height(This,pVal)	\
    ( (This)->lpVtbl -> get_Height(This,pVal) ) 

#define IUltraMonMonitor2_put_Height(This,newVal)	\
    ( (This)->lpVtbl -> put_Height(This,newVal) ) 

#define IUltraMonMonitor2_get_WorkLeft(This,pVal)	\
    ( (This)->lpVtbl -> get_WorkLeft(This,pVal) ) 

#define IUltraMonMonitor2_get_WorkTop(This,pVal)	\
    ( (This)->lpVtbl -> get_WorkTop(This,pVal) ) 

#define IUltraMonMonitor2_get_WorkWidth(This,pVal)	\
    ( (This)->lpVtbl -> get_WorkWidth(This,pVal) ) 

#define IUltraMonMonitor2_get_WorkHeight(This,pVal)	\
    ( (This)->lpVtbl -> get_WorkHeight(This,pVal) ) 

#define IUltraMonMonitor2_get_Colordepth(This,pVal)	\
    ( (This)->lpVtbl -> get_Colordepth(This,pVal) ) 

#define IUltraMonMonitor2_put_Colordepth(This,newVal)	\
    ( (This)->lpVtbl -> put_Colordepth(This,newVal) ) 

#define IUltraMonMonitor2_get_RefreshRate(This,pVal)	\
    ( (This)->lpVtbl -> get_RefreshRate(This,pVal) ) 

#define IUltraMonMonitor2_put_RefreshRate(This,newVal)	\
    ( (This)->lpVtbl -> put_RefreshRate(This,newVal) ) 

#define IUltraMonMonitor2_get_HMonitor(This,pVal)	\
    ( (This)->lpVtbl -> get_HMonitor(This,pVal) ) 

#define IUltraMonMonitor2_get_DeviceRegKey(This,pVal)	\
    ( (This)->lpVtbl -> get_DeviceRegKey(This,pVal) ) 

#define IUltraMonMonitor2_get_DisplayModes(This,pVal)	\
    ( (This)->lpVtbl -> get_DisplayModes(This,pVal) ) 

#define IUltraMonMonitor2_get_HwAccel(This,pVal)	\
    ( (This)->lpVtbl -> get_HwAccel(This,pVal) ) 

#define IUltraMonMonitor2_put_HwAccel(This,newVal)	\
    ( (This)->lpVtbl -> put_HwAccel(This,newVal) ) 

#define IUltraMonMonitor2_get_Removable(This,pVal)	\
    ( (This)->lpVtbl -> get_Removable(This,pVal) ) 

#define IUltraMonMonitor2_get_Detached(This,pVal)	\
    ( (This)->lpVtbl -> get_Detached(This,pVal) ) 

#define IUltraMonMonitor2_get_WindowsID(This,pVal)	\
    ( (This)->lpVtbl -> get_WindowsID(This,pVal) ) 


#define IUltraMonMonitor2_get_Orientation(This,pVal)	\
    ( (This)->lpVtbl -> get_Orientation(This,pVal) ) 

#define IUltraMonMonitor2_put_Orientation(This,newVal)	\
    ( (This)->lpVtbl -> put_Orientation(This,newVal) ) 

#define IUltraMonMonitor2_get_CanChangeOrientation(This,pVal)	\
    ( (This)->lpVtbl -> get_CanChangeOrientation(This,pVal) ) 

#define IUltraMonMonitor2_get_Interlaced(This,pVal)	\
    ( (This)->lpVtbl -> get_Interlaced(This,pVal) ) 

#define IUltraMonMonitor2_put_Interlaced(This,newVal)	\
    ( (This)->lpVtbl -> put_Interlaced(This,newVal) ) 

#define IUltraMonMonitor2_get_SerialNumber(This,pVal)	\
    ( (This)->lpVtbl -> get_SerialNumber(This,pVal) ) 

#define IUltraMonMonitor2_get_TargetId(This,pVal)	\
    ( (This)->lpVtbl -> get_TargetId(This,pVal) ) 

#define IUltraMonMonitor2_SetID(This,newVal)	\
    ( (This)->lpVtbl -> SetID(This,newVal) ) 

#define IUltraMonMonitor2_get_PreferredWidth(This,pVal)	\
    ( (This)->lpVtbl -> get_PreferredWidth(This,pVal) ) 

#define IUltraMonMonitor2_get_PreferredHeight(This,pVal)	\
    ( (This)->lpVtbl -> get_PreferredHeight(This,pVal) ) 

#define IUltraMonMonitor2_get_DisableMons(This,pVal)	\
    ( (This)->lpVtbl -> get_DisableMons(This,pVal) ) 

#define IUltraMonMonitor2_get_RealDispModes(This,pVal)	\
    ( (This)->lpVtbl -> get_RealDispModes(This,pVal) ) 

#define IUltraMonMonitor2_get_CcdSupport(This,pVal)	\
    ( (This)->lpVtbl -> get_CcdSupport(This,pVal) ) 

#define IUltraMonMonitor2_get_ConnectorType(This,pVal)	\
    ( (This)->lpVtbl -> get_ConnectorType(This,pVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IUltraMonMonitor2_INTERFACE_DEFINED__ */


#ifndef __IUltraMonWindow_INTERFACE_DEFINED__
#define __IUltraMonWindow_INTERFACE_DEFINED__

/* interface IUltraMonWindow */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IUltraMonWindow;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3426502C-1507-411D-B84F-A04772841ECF")
    IUltraMonWindow : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_HWnd( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_HWnd( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Monitor( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Monitor( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_HMonitor( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_HMonitor( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Left( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Left( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Top( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Top( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Width( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Width( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Height( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Height( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ShowState( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ShowState( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Title( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Title( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Class( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_HWndParent( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Centered( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Centered( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CenteredWorkspace( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_CenteredWorkspace( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Visible( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Visible( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ApplyChanges( 
            /* [in] */ long flags,
            /* [retval][out] */ VARIANT_BOOL *pSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetForegroundWindow( 
            /* [retval][out] */ VARIANT_BOOL *pSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Find( 
            /* [in] */ BSTR wndTitle,
            /* [in] */ BSTR wndClass,
            /* [in] */ long flags,
            /* [in] */ long ownerProcessId,
            /* [in] */ long timeout,
            /* [retval][out] */ VARIANT_BOOL *pSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetAppMainWindow( 
            /* [in] */ long procId,
            /* [defaultvalue][in] */ long timeout,
            /* [retval][out] */ VARIANT_BOOL *pSuccess) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ProcessId( 
            /* [retval][out] */ long *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IUltraMonWindowVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IUltraMonWindow * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IUltraMonWindow * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IUltraMonWindow * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IUltraMonWindow * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IUltraMonWindow * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IUltraMonWindow * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IUltraMonWindow * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HWnd )( 
            IUltraMonWindow * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_HWnd )( 
            IUltraMonWindow * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Monitor )( 
            IUltraMonWindow * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Monitor )( 
            IUltraMonWindow * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HMonitor )( 
            IUltraMonWindow * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_HMonitor )( 
            IUltraMonWindow * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Left )( 
            IUltraMonWindow * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Left )( 
            IUltraMonWindow * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Top )( 
            IUltraMonWindow * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Top )( 
            IUltraMonWindow * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Width )( 
            IUltraMonWindow * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Width )( 
            IUltraMonWindow * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Height )( 
            IUltraMonWindow * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Height )( 
            IUltraMonWindow * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ShowState )( 
            IUltraMonWindow * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ShowState )( 
            IUltraMonWindow * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Title )( 
            IUltraMonWindow * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Title )( 
            IUltraMonWindow * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Class )( 
            IUltraMonWindow * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HWndParent )( 
            IUltraMonWindow * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Centered )( 
            IUltraMonWindow * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Centered )( 
            IUltraMonWindow * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CenteredWorkspace )( 
            IUltraMonWindow * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_CenteredWorkspace )( 
            IUltraMonWindow * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Visible )( 
            IUltraMonWindow * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Visible )( 
            IUltraMonWindow * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ApplyChanges )( 
            IUltraMonWindow * This,
            /* [in] */ long flags,
            /* [retval][out] */ VARIANT_BOOL *pSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetForegroundWindow )( 
            IUltraMonWindow * This,
            /* [retval][out] */ VARIANT_BOOL *pSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Find )( 
            IUltraMonWindow * This,
            /* [in] */ BSTR wndTitle,
            /* [in] */ BSTR wndClass,
            /* [in] */ long flags,
            /* [in] */ long ownerProcessId,
            /* [in] */ long timeout,
            /* [retval][out] */ VARIANT_BOOL *pSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetAppMainWindow )( 
            IUltraMonWindow * This,
            /* [in] */ long procId,
            /* [defaultvalue][in] */ long timeout,
            /* [retval][out] */ VARIANT_BOOL *pSuccess);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ProcessId )( 
            IUltraMonWindow * This,
            /* [retval][out] */ long *pVal);
        
        END_INTERFACE
    } IUltraMonWindowVtbl;

    interface IUltraMonWindow
    {
        CONST_VTBL struct IUltraMonWindowVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IUltraMonWindow_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IUltraMonWindow_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IUltraMonWindow_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IUltraMonWindow_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IUltraMonWindow_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IUltraMonWindow_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IUltraMonWindow_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IUltraMonWindow_get_HWnd(This,pVal)	\
    ( (This)->lpVtbl -> get_HWnd(This,pVal) ) 

#define IUltraMonWindow_put_HWnd(This,newVal)	\
    ( (This)->lpVtbl -> put_HWnd(This,newVal) ) 

#define IUltraMonWindow_get_Monitor(This,pVal)	\
    ( (This)->lpVtbl -> get_Monitor(This,pVal) ) 

#define IUltraMonWindow_put_Monitor(This,newVal)	\
    ( (This)->lpVtbl -> put_Monitor(This,newVal) ) 

#define IUltraMonWindow_get_HMonitor(This,pVal)	\
    ( (This)->lpVtbl -> get_HMonitor(This,pVal) ) 

#define IUltraMonWindow_put_HMonitor(This,newVal)	\
    ( (This)->lpVtbl -> put_HMonitor(This,newVal) ) 

#define IUltraMonWindow_get_Left(This,pVal)	\
    ( (This)->lpVtbl -> get_Left(This,pVal) ) 

#define IUltraMonWindow_put_Left(This,newVal)	\
    ( (This)->lpVtbl -> put_Left(This,newVal) ) 

#define IUltraMonWindow_get_Top(This,pVal)	\
    ( (This)->lpVtbl -> get_Top(This,pVal) ) 

#define IUltraMonWindow_put_Top(This,newVal)	\
    ( (This)->lpVtbl -> put_Top(This,newVal) ) 

#define IUltraMonWindow_get_Width(This,pVal)	\
    ( (This)->lpVtbl -> get_Width(This,pVal) ) 

#define IUltraMonWindow_put_Width(This,newVal)	\
    ( (This)->lpVtbl -> put_Width(This,newVal) ) 

#define IUltraMonWindow_get_Height(This,pVal)	\
    ( (This)->lpVtbl -> get_Height(This,pVal) ) 

#define IUltraMonWindow_put_Height(This,newVal)	\
    ( (This)->lpVtbl -> put_Height(This,newVal) ) 

#define IUltraMonWindow_get_ShowState(This,pVal)	\
    ( (This)->lpVtbl -> get_ShowState(This,pVal) ) 

#define IUltraMonWindow_put_ShowState(This,newVal)	\
    ( (This)->lpVtbl -> put_ShowState(This,newVal) ) 

#define IUltraMonWindow_get_Title(This,pVal)	\
    ( (This)->lpVtbl -> get_Title(This,pVal) ) 

#define IUltraMonWindow_put_Title(This,newVal)	\
    ( (This)->lpVtbl -> put_Title(This,newVal) ) 

#define IUltraMonWindow_get_Class(This,pVal)	\
    ( (This)->lpVtbl -> get_Class(This,pVal) ) 

#define IUltraMonWindow_get_HWndParent(This,pVal)	\
    ( (This)->lpVtbl -> get_HWndParent(This,pVal) ) 

#define IUltraMonWindow_get_Centered(This,pVal)	\
    ( (This)->lpVtbl -> get_Centered(This,pVal) ) 

#define IUltraMonWindow_put_Centered(This,newVal)	\
    ( (This)->lpVtbl -> put_Centered(This,newVal) ) 

#define IUltraMonWindow_get_CenteredWorkspace(This,pVal)	\
    ( (This)->lpVtbl -> get_CenteredWorkspace(This,pVal) ) 

#define IUltraMonWindow_put_CenteredWorkspace(This,newVal)	\
    ( (This)->lpVtbl -> put_CenteredWorkspace(This,newVal) ) 

#define IUltraMonWindow_get_Visible(This,pVal)	\
    ( (This)->lpVtbl -> get_Visible(This,pVal) ) 

#define IUltraMonWindow_put_Visible(This,newVal)	\
    ( (This)->lpVtbl -> put_Visible(This,newVal) ) 

#define IUltraMonWindow_ApplyChanges(This,flags,pSuccess)	\
    ( (This)->lpVtbl -> ApplyChanges(This,flags,pSuccess) ) 

#define IUltraMonWindow_GetForegroundWindow(This,pSuccess)	\
    ( (This)->lpVtbl -> GetForegroundWindow(This,pSuccess) ) 

#define IUltraMonWindow_Find(This,wndTitle,wndClass,flags,ownerProcessId,timeout,pSuccess)	\
    ( (This)->lpVtbl -> Find(This,wndTitle,wndClass,flags,ownerProcessId,timeout,pSuccess) ) 

#define IUltraMonWindow_GetAppMainWindow(This,procId,timeout,pSuccess)	\
    ( (This)->lpVtbl -> GetAppMainWindow(This,procId,timeout,pSuccess) ) 

#define IUltraMonWindow_get_ProcessId(This,pVal)	\
    ( (This)->lpVtbl -> get_ProcessId(This,pVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IUltraMonWindow_INTERFACE_DEFINED__ */


#ifndef __IUltraMonWindow2_INTERFACE_DEFINED__
#define __IUltraMonWindow2_INTERFACE_DEFINED__

/* interface IUltraMonWindow2 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IUltraMonWindow2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("20A81455-62E7-4fb6-B924-53DBC656ADBE")
    IUltraMonWindow2 : public IUltraMonWindow
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ProcessExe( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Close( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetAllWindows( 
            /* [in] */ VARIANT_BOOL includeHidden,
            /* [retval][out] */ IUltraMonWindows **pWnds) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetAppWindows( 
            /* [in] */ VARIANT_BOOL includeDlgs,
            /* [retval][out] */ IUltraMonWindows **pWnds) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Style( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ExStyle( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_AlwaysOnTop( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_AlwaysOnTop( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Active( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Activate( 
            /* [retval][out] */ VARIANT_BOOL *pSuccess) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IUltraMonWindow2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IUltraMonWindow2 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IUltraMonWindow2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IUltraMonWindow2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IUltraMonWindow2 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IUltraMonWindow2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IUltraMonWindow2 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IUltraMonWindow2 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HWnd )( 
            IUltraMonWindow2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_HWnd )( 
            IUltraMonWindow2 * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Monitor )( 
            IUltraMonWindow2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Monitor )( 
            IUltraMonWindow2 * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HMonitor )( 
            IUltraMonWindow2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_HMonitor )( 
            IUltraMonWindow2 * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Left )( 
            IUltraMonWindow2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Left )( 
            IUltraMonWindow2 * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Top )( 
            IUltraMonWindow2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Top )( 
            IUltraMonWindow2 * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Width )( 
            IUltraMonWindow2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Width )( 
            IUltraMonWindow2 * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Height )( 
            IUltraMonWindow2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Height )( 
            IUltraMonWindow2 * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ShowState )( 
            IUltraMonWindow2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ShowState )( 
            IUltraMonWindow2 * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Title )( 
            IUltraMonWindow2 * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Title )( 
            IUltraMonWindow2 * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Class )( 
            IUltraMonWindow2 * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HWndParent )( 
            IUltraMonWindow2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Centered )( 
            IUltraMonWindow2 * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Centered )( 
            IUltraMonWindow2 * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CenteredWorkspace )( 
            IUltraMonWindow2 * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_CenteredWorkspace )( 
            IUltraMonWindow2 * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Visible )( 
            IUltraMonWindow2 * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Visible )( 
            IUltraMonWindow2 * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ApplyChanges )( 
            IUltraMonWindow2 * This,
            /* [in] */ long flags,
            /* [retval][out] */ VARIANT_BOOL *pSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetForegroundWindow )( 
            IUltraMonWindow2 * This,
            /* [retval][out] */ VARIANT_BOOL *pSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Find )( 
            IUltraMonWindow2 * This,
            /* [in] */ BSTR wndTitle,
            /* [in] */ BSTR wndClass,
            /* [in] */ long flags,
            /* [in] */ long ownerProcessId,
            /* [in] */ long timeout,
            /* [retval][out] */ VARIANT_BOOL *pSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetAppMainWindow )( 
            IUltraMonWindow2 * This,
            /* [in] */ long procId,
            /* [defaultvalue][in] */ long timeout,
            /* [retval][out] */ VARIANT_BOOL *pSuccess);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ProcessId )( 
            IUltraMonWindow2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ProcessExe )( 
            IUltraMonWindow2 * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Close )( 
            IUltraMonWindow2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetAllWindows )( 
            IUltraMonWindow2 * This,
            /* [in] */ VARIANT_BOOL includeHidden,
            /* [retval][out] */ IUltraMonWindows **pWnds);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetAppWindows )( 
            IUltraMonWindow2 * This,
            /* [in] */ VARIANT_BOOL includeDlgs,
            /* [retval][out] */ IUltraMonWindows **pWnds);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Style )( 
            IUltraMonWindow2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ExStyle )( 
            IUltraMonWindow2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_AlwaysOnTop )( 
            IUltraMonWindow2 * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_AlwaysOnTop )( 
            IUltraMonWindow2 * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Active )( 
            IUltraMonWindow2 * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Activate )( 
            IUltraMonWindow2 * This,
            /* [retval][out] */ VARIANT_BOOL *pSuccess);
        
        END_INTERFACE
    } IUltraMonWindow2Vtbl;

    interface IUltraMonWindow2
    {
        CONST_VTBL struct IUltraMonWindow2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IUltraMonWindow2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IUltraMonWindow2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IUltraMonWindow2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IUltraMonWindow2_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IUltraMonWindow2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IUltraMonWindow2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IUltraMonWindow2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IUltraMonWindow2_get_HWnd(This,pVal)	\
    ( (This)->lpVtbl -> get_HWnd(This,pVal) ) 

#define IUltraMonWindow2_put_HWnd(This,newVal)	\
    ( (This)->lpVtbl -> put_HWnd(This,newVal) ) 

#define IUltraMonWindow2_get_Monitor(This,pVal)	\
    ( (This)->lpVtbl -> get_Monitor(This,pVal) ) 

#define IUltraMonWindow2_put_Monitor(This,newVal)	\
    ( (This)->lpVtbl -> put_Monitor(This,newVal) ) 

#define IUltraMonWindow2_get_HMonitor(This,pVal)	\
    ( (This)->lpVtbl -> get_HMonitor(This,pVal) ) 

#define IUltraMonWindow2_put_HMonitor(This,newVal)	\
    ( (This)->lpVtbl -> put_HMonitor(This,newVal) ) 

#define IUltraMonWindow2_get_Left(This,pVal)	\
    ( (This)->lpVtbl -> get_Left(This,pVal) ) 

#define IUltraMonWindow2_put_Left(This,newVal)	\
    ( (This)->lpVtbl -> put_Left(This,newVal) ) 

#define IUltraMonWindow2_get_Top(This,pVal)	\
    ( (This)->lpVtbl -> get_Top(This,pVal) ) 

#define IUltraMonWindow2_put_Top(This,newVal)	\
    ( (This)->lpVtbl -> put_Top(This,newVal) ) 

#define IUltraMonWindow2_get_Width(This,pVal)	\
    ( (This)->lpVtbl -> get_Width(This,pVal) ) 

#define IUltraMonWindow2_put_Width(This,newVal)	\
    ( (This)->lpVtbl -> put_Width(This,newVal) ) 

#define IUltraMonWindow2_get_Height(This,pVal)	\
    ( (This)->lpVtbl -> get_Height(This,pVal) ) 

#define IUltraMonWindow2_put_Height(This,newVal)	\
    ( (This)->lpVtbl -> put_Height(This,newVal) ) 

#define IUltraMonWindow2_get_ShowState(This,pVal)	\
    ( (This)->lpVtbl -> get_ShowState(This,pVal) ) 

#define IUltraMonWindow2_put_ShowState(This,newVal)	\
    ( (This)->lpVtbl -> put_ShowState(This,newVal) ) 

#define IUltraMonWindow2_get_Title(This,pVal)	\
    ( (This)->lpVtbl -> get_Title(This,pVal) ) 

#define IUltraMonWindow2_put_Title(This,newVal)	\
    ( (This)->lpVtbl -> put_Title(This,newVal) ) 

#define IUltraMonWindow2_get_Class(This,pVal)	\
    ( (This)->lpVtbl -> get_Class(This,pVal) ) 

#define IUltraMonWindow2_get_HWndParent(This,pVal)	\
    ( (This)->lpVtbl -> get_HWndParent(This,pVal) ) 

#define IUltraMonWindow2_get_Centered(This,pVal)	\
    ( (This)->lpVtbl -> get_Centered(This,pVal) ) 

#define IUltraMonWindow2_put_Centered(This,newVal)	\
    ( (This)->lpVtbl -> put_Centered(This,newVal) ) 

#define IUltraMonWindow2_get_CenteredWorkspace(This,pVal)	\
    ( (This)->lpVtbl -> get_CenteredWorkspace(This,pVal) ) 

#define IUltraMonWindow2_put_CenteredWorkspace(This,newVal)	\
    ( (This)->lpVtbl -> put_CenteredWorkspace(This,newVal) ) 

#define IUltraMonWindow2_get_Visible(This,pVal)	\
    ( (This)->lpVtbl -> get_Visible(This,pVal) ) 

#define IUltraMonWindow2_put_Visible(This,newVal)	\
    ( (This)->lpVtbl -> put_Visible(This,newVal) ) 

#define IUltraMonWindow2_ApplyChanges(This,flags,pSuccess)	\
    ( (This)->lpVtbl -> ApplyChanges(This,flags,pSuccess) ) 

#define IUltraMonWindow2_GetForegroundWindow(This,pSuccess)	\
    ( (This)->lpVtbl -> GetForegroundWindow(This,pSuccess) ) 

#define IUltraMonWindow2_Find(This,wndTitle,wndClass,flags,ownerProcessId,timeout,pSuccess)	\
    ( (This)->lpVtbl -> Find(This,wndTitle,wndClass,flags,ownerProcessId,timeout,pSuccess) ) 

#define IUltraMonWindow2_GetAppMainWindow(This,procId,timeout,pSuccess)	\
    ( (This)->lpVtbl -> GetAppMainWindow(This,procId,timeout,pSuccess) ) 

#define IUltraMonWindow2_get_ProcessId(This,pVal)	\
    ( (This)->lpVtbl -> get_ProcessId(This,pVal) ) 


#define IUltraMonWindow2_get_ProcessExe(This,pVal)	\
    ( (This)->lpVtbl -> get_ProcessExe(This,pVal) ) 

#define IUltraMonWindow2_Close(This)	\
    ( (This)->lpVtbl -> Close(This) ) 

#define IUltraMonWindow2_GetAllWindows(This,includeHidden,pWnds)	\
    ( (This)->lpVtbl -> GetAllWindows(This,includeHidden,pWnds) ) 

#define IUltraMonWindow2_GetAppWindows(This,includeDlgs,pWnds)	\
    ( (This)->lpVtbl -> GetAppWindows(This,includeDlgs,pWnds) ) 

#define IUltraMonWindow2_get_Style(This,pVal)	\
    ( (This)->lpVtbl -> get_Style(This,pVal) ) 

#define IUltraMonWindow2_get_ExStyle(This,pVal)	\
    ( (This)->lpVtbl -> get_ExStyle(This,pVal) ) 

#define IUltraMonWindow2_get_AlwaysOnTop(This,pVal)	\
    ( (This)->lpVtbl -> get_AlwaysOnTop(This,pVal) ) 

#define IUltraMonWindow2_put_AlwaysOnTop(This,newVal)	\
    ( (This)->lpVtbl -> put_AlwaysOnTop(This,newVal) ) 

#define IUltraMonWindow2_get_Active(This,pVal)	\
    ( (This)->lpVtbl -> get_Active(This,pVal) ) 

#define IUltraMonWindow2_Activate(This,pSuccess)	\
    ( (This)->lpVtbl -> Activate(This,pSuccess) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IUltraMonWindow2_INTERFACE_DEFINED__ */


#ifndef __IUltraMonUtility_INTERFACE_DEFINED__
#define __IUltraMonUtility_INTERFACE_DEFINED__

/* interface IUltraMonUtility */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IUltraMonUtility;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0587576A-E123-47DE-8CBF-2555FF6D152D")
    IUltraMonUtility : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Run( 
            /* [in] */ BSTR cmd,
            /* [defaultvalue][in] */ SHOWSTATE showState,
            /* [defaultvalue][in] */ VARIANT_BOOL suspended,
            /* [defaultvalue][in] */ BSTR verb,
            /* [retval][out] */ VARIANT_BOOL *pSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Sleep( 
            /* [in] */ long mSecs) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WaitForProcess( 
            /* [in] */ long hProc,
            /* [in] */ long mSecs,
            /* [retval][out] */ VARIANT_BOOL *pResult) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ProcessId( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_HProcess( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_HardwareProfile( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Docked( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ThreadId( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ResumeProcess( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IUltraMonUtilityVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IUltraMonUtility * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IUltraMonUtility * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IUltraMonUtility * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IUltraMonUtility * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IUltraMonUtility * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IUltraMonUtility * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IUltraMonUtility * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Run )( 
            IUltraMonUtility * This,
            /* [in] */ BSTR cmd,
            /* [defaultvalue][in] */ SHOWSTATE showState,
            /* [defaultvalue][in] */ VARIANT_BOOL suspended,
            /* [defaultvalue][in] */ BSTR verb,
            /* [retval][out] */ VARIANT_BOOL *pSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Sleep )( 
            IUltraMonUtility * This,
            /* [in] */ long mSecs);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *WaitForProcess )( 
            IUltraMonUtility * This,
            /* [in] */ long hProc,
            /* [in] */ long mSecs,
            /* [retval][out] */ VARIANT_BOOL *pResult);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ProcessId )( 
            IUltraMonUtility * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HProcess )( 
            IUltraMonUtility * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HardwareProfile )( 
            IUltraMonUtility * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Docked )( 
            IUltraMonUtility * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ThreadId )( 
            IUltraMonUtility * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ResumeProcess )( 
            IUltraMonUtility * This);
        
        END_INTERFACE
    } IUltraMonUtilityVtbl;

    interface IUltraMonUtility
    {
        CONST_VTBL struct IUltraMonUtilityVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IUltraMonUtility_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IUltraMonUtility_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IUltraMonUtility_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IUltraMonUtility_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IUltraMonUtility_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IUltraMonUtility_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IUltraMonUtility_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IUltraMonUtility_Run(This,cmd,showState,suspended,verb,pSuccess)	\
    ( (This)->lpVtbl -> Run(This,cmd,showState,suspended,verb,pSuccess) ) 

#define IUltraMonUtility_Sleep(This,mSecs)	\
    ( (This)->lpVtbl -> Sleep(This,mSecs) ) 

#define IUltraMonUtility_WaitForProcess(This,hProc,mSecs,pResult)	\
    ( (This)->lpVtbl -> WaitForProcess(This,hProc,mSecs,pResult) ) 

#define IUltraMonUtility_get_ProcessId(This,pVal)	\
    ( (This)->lpVtbl -> get_ProcessId(This,pVal) ) 

#define IUltraMonUtility_get_HProcess(This,pVal)	\
    ( (This)->lpVtbl -> get_HProcess(This,pVal) ) 

#define IUltraMonUtility_get_HardwareProfile(This,pVal)	\
    ( (This)->lpVtbl -> get_HardwareProfile(This,pVal) ) 

#define IUltraMonUtility_get_Docked(This,pVal)	\
    ( (This)->lpVtbl -> get_Docked(This,pVal) ) 

#define IUltraMonUtility_get_ThreadId(This,pVal)	\
    ( (This)->lpVtbl -> get_ThreadId(This,pVal) ) 

#define IUltraMonUtility_ResumeProcess(This)	\
    ( (This)->lpVtbl -> ResumeProcess(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IUltraMonUtility_INTERFACE_DEFINED__ */


#ifndef __IUltraMonUtility2_INTERFACE_DEFINED__
#define __IUltraMonUtility2_INTERFACE_DEFINED__

/* interface IUltraMonUtility2 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IUltraMonUtility2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("40F1835A-892E-4E5E-A666-0D2702066D8E")
    IUltraMonUtility2 : public IUltraMonUtility
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Run2( 
            /* [in] */ BSTR file,
            /* [in] */ BSTR commandLine,
            /* [defaultvalue][in] */ SHOWSTATE showState,
            /* [defaultvalue][in] */ VARIANT_BOOL suspended,
            /* [defaultvalue][in] */ BSTR verb,
            /* [defaultvalue][in] */ BSTR curDir,
            /* [retval][out] */ VARIANT_BOOL *pSuccess) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IUltraMonUtility2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IUltraMonUtility2 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IUltraMonUtility2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IUltraMonUtility2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IUltraMonUtility2 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IUltraMonUtility2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IUltraMonUtility2 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IUltraMonUtility2 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Run )( 
            IUltraMonUtility2 * This,
            /* [in] */ BSTR cmd,
            /* [defaultvalue][in] */ SHOWSTATE showState,
            /* [defaultvalue][in] */ VARIANT_BOOL suspended,
            /* [defaultvalue][in] */ BSTR verb,
            /* [retval][out] */ VARIANT_BOOL *pSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Sleep )( 
            IUltraMonUtility2 * This,
            /* [in] */ long mSecs);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *WaitForProcess )( 
            IUltraMonUtility2 * This,
            /* [in] */ long hProc,
            /* [in] */ long mSecs,
            /* [retval][out] */ VARIANT_BOOL *pResult);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ProcessId )( 
            IUltraMonUtility2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HProcess )( 
            IUltraMonUtility2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HardwareProfile )( 
            IUltraMonUtility2 * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Docked )( 
            IUltraMonUtility2 * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ThreadId )( 
            IUltraMonUtility2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ResumeProcess )( 
            IUltraMonUtility2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Run2 )( 
            IUltraMonUtility2 * This,
            /* [in] */ BSTR file,
            /* [in] */ BSTR commandLine,
            /* [defaultvalue][in] */ SHOWSTATE showState,
            /* [defaultvalue][in] */ VARIANT_BOOL suspended,
            /* [defaultvalue][in] */ BSTR verb,
            /* [defaultvalue][in] */ BSTR curDir,
            /* [retval][out] */ VARIANT_BOOL *pSuccess);
        
        END_INTERFACE
    } IUltraMonUtility2Vtbl;

    interface IUltraMonUtility2
    {
        CONST_VTBL struct IUltraMonUtility2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IUltraMonUtility2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IUltraMonUtility2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IUltraMonUtility2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IUltraMonUtility2_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IUltraMonUtility2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IUltraMonUtility2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IUltraMonUtility2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IUltraMonUtility2_Run(This,cmd,showState,suspended,verb,pSuccess)	\
    ( (This)->lpVtbl -> Run(This,cmd,showState,suspended,verb,pSuccess) ) 

#define IUltraMonUtility2_Sleep(This,mSecs)	\
    ( (This)->lpVtbl -> Sleep(This,mSecs) ) 

#define IUltraMonUtility2_WaitForProcess(This,hProc,mSecs,pResult)	\
    ( (This)->lpVtbl -> WaitForProcess(This,hProc,mSecs,pResult) ) 

#define IUltraMonUtility2_get_ProcessId(This,pVal)	\
    ( (This)->lpVtbl -> get_ProcessId(This,pVal) ) 

#define IUltraMonUtility2_get_HProcess(This,pVal)	\
    ( (This)->lpVtbl -> get_HProcess(This,pVal) ) 

#define IUltraMonUtility2_get_HardwareProfile(This,pVal)	\
    ( (This)->lpVtbl -> get_HardwareProfile(This,pVal) ) 

#define IUltraMonUtility2_get_Docked(This,pVal)	\
    ( (This)->lpVtbl -> get_Docked(This,pVal) ) 

#define IUltraMonUtility2_get_ThreadId(This,pVal)	\
    ( (This)->lpVtbl -> get_ThreadId(This,pVal) ) 

#define IUltraMonUtility2_ResumeProcess(This)	\
    ( (This)->lpVtbl -> ResumeProcess(This) ) 


#define IUltraMonUtility2_Run2(This,file,commandLine,showState,suspended,verb,curDir,pSuccess)	\
    ( (This)->lpVtbl -> Run2(This,file,commandLine,showState,suspended,verb,curDir,pSuccess) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IUltraMonUtility2_INTERFACE_DEFINED__ */


#ifndef __IUltraMonMonCtlEvents_INTERFACE_DEFINED__
#define __IUltraMonMonCtlEvents_INTERFACE_DEFINED__

/* interface IUltraMonMonCtlEvents */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IUltraMonMonCtlEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6E558C02-FEC9-48A0-9D18-22672BE3123A")
    IUltraMonMonCtlEvents : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnDraw( 
            /* [in] */ IUltraMonMonCtl *sender,
            /* [in] */ IUltraMonMonitorMonCtl *monitor,
            /* [in] */ long hWnd) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnSelChange( 
            /* [in] */ IUltraMonMonCtl *sender,
            /* [unique][in] */ IUltraMonMonitorMonCtl *newSel,
            /* [unique][in] */ IUltraMonMonitorMonCtl *prevSel,
            /* [out] */ VARIANT_BOOL *cancel) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnBeginMove( 
            /* [in] */ IUltraMonMonCtl *sender,
            /* [in] */ IUltraMonMonitorMonCtl *monitor,
            /* [in] */ long x,
            /* [in] */ long y) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnMoving( 
            /* [in] */ IUltraMonMonCtl *sender,
            /* [in] */ IUltraMonMonitorMonCtl *monitor,
            /* [in] */ long x,
            /* [in] */ long y) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnEndMove( 
            /* [in] */ IUltraMonMonCtl *sender,
            /* [in] */ IUltraMonMonitorMonCtl *monitor,
            /* [in] */ long x,
            /* [in] */ long y,
            /* [in] */ VARIANT_BOOL canceled) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IUltraMonMonCtlEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IUltraMonMonCtlEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IUltraMonMonCtlEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IUltraMonMonCtlEvents * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnDraw )( 
            IUltraMonMonCtlEvents * This,
            /* [in] */ IUltraMonMonCtl *sender,
            /* [in] */ IUltraMonMonitorMonCtl *monitor,
            /* [in] */ long hWnd);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnSelChange )( 
            IUltraMonMonCtlEvents * This,
            /* [in] */ IUltraMonMonCtl *sender,
            /* [unique][in] */ IUltraMonMonitorMonCtl *newSel,
            /* [unique][in] */ IUltraMonMonitorMonCtl *prevSel,
            /* [out] */ VARIANT_BOOL *cancel);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnBeginMove )( 
            IUltraMonMonCtlEvents * This,
            /* [in] */ IUltraMonMonCtl *sender,
            /* [in] */ IUltraMonMonitorMonCtl *monitor,
            /* [in] */ long x,
            /* [in] */ long y);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnMoving )( 
            IUltraMonMonCtlEvents * This,
            /* [in] */ IUltraMonMonCtl *sender,
            /* [in] */ IUltraMonMonitorMonCtl *monitor,
            /* [in] */ long x,
            /* [in] */ long y);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnEndMove )( 
            IUltraMonMonCtlEvents * This,
            /* [in] */ IUltraMonMonCtl *sender,
            /* [in] */ IUltraMonMonitorMonCtl *monitor,
            /* [in] */ long x,
            /* [in] */ long y,
            /* [in] */ VARIANT_BOOL canceled);
        
        END_INTERFACE
    } IUltraMonMonCtlEventsVtbl;

    interface IUltraMonMonCtlEvents
    {
        CONST_VTBL struct IUltraMonMonCtlEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IUltraMonMonCtlEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IUltraMonMonCtlEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IUltraMonMonCtlEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IUltraMonMonCtlEvents_OnDraw(This,sender,monitor,hWnd)	\
    ( (This)->lpVtbl -> OnDraw(This,sender,monitor,hWnd) ) 

#define IUltraMonMonCtlEvents_OnSelChange(This,sender,newSel,prevSel,cancel)	\
    ( (This)->lpVtbl -> OnSelChange(This,sender,newSel,prevSel,cancel) ) 

#define IUltraMonMonCtlEvents_OnBeginMove(This,sender,monitor,x,y)	\
    ( (This)->lpVtbl -> OnBeginMove(This,sender,monitor,x,y) ) 

#define IUltraMonMonCtlEvents_OnMoving(This,sender,monitor,x,y)	\
    ( (This)->lpVtbl -> OnMoving(This,sender,monitor,x,y) ) 

#define IUltraMonMonCtlEvents_OnEndMove(This,sender,monitor,x,y,canceled)	\
    ( (This)->lpVtbl -> OnEndMove(This,sender,monitor,x,y,canceled) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IUltraMonMonCtlEvents_INTERFACE_DEFINED__ */


#ifndef __IUltraMonMonitorsMonCtl_INTERFACE_DEFINED__
#define __IUltraMonMonitorsMonCtl_INTERFACE_DEFINED__

/* interface IUltraMonMonitorsMonCtl */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IUltraMonMonitorsMonCtl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5E4D9345-4BB2-4144-9EE1-A5F19BBAA530")
    IUltraMonMonitorsMonCtl : public IDispatch
    {
    public:
        virtual /* [custom][helpstring][id] */ HRESULT STDMETHODCALLTYPE Item( 
            /* [in] */ VARIANT index,
            /* [retval][out] */ IUltraMonMonitorMonCtl **ppMonitor) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][restricted][propget] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown **pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IUltraMonMonitorsMonCtlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IUltraMonMonitorsMonCtl * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IUltraMonMonitorsMonCtl * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IUltraMonMonitorsMonCtl * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IUltraMonMonitorsMonCtl * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IUltraMonMonitorsMonCtl * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IUltraMonMonitorsMonCtl * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IUltraMonMonitorsMonCtl * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [custom][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Item )( 
            IUltraMonMonitorsMonCtl * This,
            /* [in] */ VARIANT index,
            /* [retval][out] */ IUltraMonMonitorMonCtl **ppMonitor);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IUltraMonMonitorsMonCtl * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][restricted][propget] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            IUltraMonMonitorsMonCtl * This,
            /* [retval][out] */ IUnknown **pVal);
        
        END_INTERFACE
    } IUltraMonMonitorsMonCtlVtbl;

    interface IUltraMonMonitorsMonCtl
    {
        CONST_VTBL struct IUltraMonMonitorsMonCtlVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IUltraMonMonitorsMonCtl_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IUltraMonMonitorsMonCtl_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IUltraMonMonitorsMonCtl_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IUltraMonMonitorsMonCtl_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IUltraMonMonitorsMonCtl_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IUltraMonMonitorsMonCtl_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IUltraMonMonitorsMonCtl_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IUltraMonMonitorsMonCtl_Item(This,index,ppMonitor)	\
    ( (This)->lpVtbl -> Item(This,index,ppMonitor) ) 

#define IUltraMonMonitorsMonCtl_get_Count(This,pVal)	\
    ( (This)->lpVtbl -> get_Count(This,pVal) ) 

#define IUltraMonMonitorsMonCtl_get__NewEnum(This,pVal)	\
    ( (This)->lpVtbl -> get__NewEnum(This,pVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IUltraMonMonitorsMonCtl_INTERFACE_DEFINED__ */


#ifndef __IUltraMonMonCtl_INTERFACE_DEFINED__
#define __IUltraMonMonCtl_INTERFACE_DEFINED__

/* interface IUltraMonMonCtl */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IUltraMonMonCtl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("FEB58CD7-0422-46F9-AD51-0327D296393E")
    IUltraMonMonCtl : public IUnknown
    {
    public:
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_ParentWnd( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_ParentWnd( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Left( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_Left( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_DrawMode( 
            /* [retval][out] */ DRAWMODE *pVal) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_DrawMode( 
            /* [in] */ DRAWMODE newVal) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Top( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_Top( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Width( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_Width( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Height( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_Height( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_AutoRefresh( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_AutoRefresh( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_ShowMonitorStatus( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_ShowMonitorStatus( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_SelDisabled( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_SelDisabled( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Selection( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_Selection( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Visible( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_Visible( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE RefreshMonitorLayout( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Redraw( 
            /* [in] */ REDRAW what) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Create( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Destroy( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE MonCtlAdvise( 
            /* [in] */ IUltraMonMonCtlEvents *pEvents,
            /* [in] */ long events,
            /* [retval][out] */ long *pCookie) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE MonCtlUnadvise( 
            /* [in] */ long cookie) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CalcDesktopDimensions( 
            /* [out] */ long *left,
            /* [out] */ long *top,
            /* [out] */ long *width,
            /* [out] */ long *height) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Style( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_Style( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Monitors( 
            /* [retval][out] */ IUltraMonMonitorsMonCtl **pVal) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_AllowMonitorMoving( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_AllowMonitorMoving( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_ShowIgnoredMonitors( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_ShowIgnoredMonitors( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Enabled( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_Enabled( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IUltraMonMonCtlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IUltraMonMonCtl * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IUltraMonMonCtl * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IUltraMonMonCtl * This);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ParentWnd )( 
            IUltraMonMonCtl * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ParentWnd )( 
            IUltraMonMonCtl * This,
            /* [in] */ long newVal);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Left )( 
            IUltraMonMonCtl * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Left )( 
            IUltraMonMonCtl * This,
            /* [in] */ long newVal);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DrawMode )( 
            IUltraMonMonCtl * This,
            /* [retval][out] */ DRAWMODE *pVal);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_DrawMode )( 
            IUltraMonMonCtl * This,
            /* [in] */ DRAWMODE newVal);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Top )( 
            IUltraMonMonCtl * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Top )( 
            IUltraMonMonCtl * This,
            /* [in] */ long newVal);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Width )( 
            IUltraMonMonCtl * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Width )( 
            IUltraMonMonCtl * This,
            /* [in] */ long newVal);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Height )( 
            IUltraMonMonCtl * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Height )( 
            IUltraMonMonCtl * This,
            /* [in] */ long newVal);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_AutoRefresh )( 
            IUltraMonMonCtl * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_AutoRefresh )( 
            IUltraMonMonCtl * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ShowMonitorStatus )( 
            IUltraMonMonCtl * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ShowMonitorStatus )( 
            IUltraMonMonCtl * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SelDisabled )( 
            IUltraMonMonCtl * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_SelDisabled )( 
            IUltraMonMonCtl * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Selection )( 
            IUltraMonMonCtl * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Selection )( 
            IUltraMonMonCtl * This,
            /* [in] */ long newVal);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Visible )( 
            IUltraMonMonCtl * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Visible )( 
            IUltraMonMonCtl * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *RefreshMonitorLayout )( 
            IUltraMonMonCtl * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Redraw )( 
            IUltraMonMonCtl * This,
            /* [in] */ REDRAW what);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Create )( 
            IUltraMonMonCtl * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Destroy )( 
            IUltraMonMonCtl * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *MonCtlAdvise )( 
            IUltraMonMonCtl * This,
            /* [in] */ IUltraMonMonCtlEvents *pEvents,
            /* [in] */ long events,
            /* [retval][out] */ long *pCookie);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *MonCtlUnadvise )( 
            IUltraMonMonCtl * This,
            /* [in] */ long cookie);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *CalcDesktopDimensions )( 
            IUltraMonMonCtl * This,
            /* [out] */ long *left,
            /* [out] */ long *top,
            /* [out] */ long *width,
            /* [out] */ long *height);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Style )( 
            IUltraMonMonCtl * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Style )( 
            IUltraMonMonCtl * This,
            /* [in] */ long newVal);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Monitors )( 
            IUltraMonMonCtl * This,
            /* [retval][out] */ IUltraMonMonitorsMonCtl **pVal);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_AllowMonitorMoving )( 
            IUltraMonMonCtl * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_AllowMonitorMoving )( 
            IUltraMonMonCtl * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ShowIgnoredMonitors )( 
            IUltraMonMonCtl * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ShowIgnoredMonitors )( 
            IUltraMonMonCtl * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Enabled )( 
            IUltraMonMonCtl * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Enabled )( 
            IUltraMonMonCtl * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        END_INTERFACE
    } IUltraMonMonCtlVtbl;

    interface IUltraMonMonCtl
    {
        CONST_VTBL struct IUltraMonMonCtlVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IUltraMonMonCtl_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IUltraMonMonCtl_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IUltraMonMonCtl_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IUltraMonMonCtl_get_ParentWnd(This,pVal)	\
    ( (This)->lpVtbl -> get_ParentWnd(This,pVal) ) 

#define IUltraMonMonCtl_put_ParentWnd(This,newVal)	\
    ( (This)->lpVtbl -> put_ParentWnd(This,newVal) ) 

#define IUltraMonMonCtl_get_Left(This,pVal)	\
    ( (This)->lpVtbl -> get_Left(This,pVal) ) 

#define IUltraMonMonCtl_put_Left(This,newVal)	\
    ( (This)->lpVtbl -> put_Left(This,newVal) ) 

#define IUltraMonMonCtl_get_DrawMode(This,pVal)	\
    ( (This)->lpVtbl -> get_DrawMode(This,pVal) ) 

#define IUltraMonMonCtl_put_DrawMode(This,newVal)	\
    ( (This)->lpVtbl -> put_DrawMode(This,newVal) ) 

#define IUltraMonMonCtl_get_Top(This,pVal)	\
    ( (This)->lpVtbl -> get_Top(This,pVal) ) 

#define IUltraMonMonCtl_put_Top(This,newVal)	\
    ( (This)->lpVtbl -> put_Top(This,newVal) ) 

#define IUltraMonMonCtl_get_Width(This,pVal)	\
    ( (This)->lpVtbl -> get_Width(This,pVal) ) 

#define IUltraMonMonCtl_put_Width(This,newVal)	\
    ( (This)->lpVtbl -> put_Width(This,newVal) ) 

#define IUltraMonMonCtl_get_Height(This,pVal)	\
    ( (This)->lpVtbl -> get_Height(This,pVal) ) 

#define IUltraMonMonCtl_put_Height(This,newVal)	\
    ( (This)->lpVtbl -> put_Height(This,newVal) ) 

#define IUltraMonMonCtl_get_AutoRefresh(This,pVal)	\
    ( (This)->lpVtbl -> get_AutoRefresh(This,pVal) ) 

#define IUltraMonMonCtl_put_AutoRefresh(This,newVal)	\
    ( (This)->lpVtbl -> put_AutoRefresh(This,newVal) ) 

#define IUltraMonMonCtl_get_ShowMonitorStatus(This,pVal)	\
    ( (This)->lpVtbl -> get_ShowMonitorStatus(This,pVal) ) 

#define IUltraMonMonCtl_put_ShowMonitorStatus(This,newVal)	\
    ( (This)->lpVtbl -> put_ShowMonitorStatus(This,newVal) ) 

#define IUltraMonMonCtl_get_SelDisabled(This,pVal)	\
    ( (This)->lpVtbl -> get_SelDisabled(This,pVal) ) 

#define IUltraMonMonCtl_put_SelDisabled(This,newVal)	\
    ( (This)->lpVtbl -> put_SelDisabled(This,newVal) ) 

#define IUltraMonMonCtl_get_Selection(This,pVal)	\
    ( (This)->lpVtbl -> get_Selection(This,pVal) ) 

#define IUltraMonMonCtl_put_Selection(This,newVal)	\
    ( (This)->lpVtbl -> put_Selection(This,newVal) ) 

#define IUltraMonMonCtl_get_Visible(This,pVal)	\
    ( (This)->lpVtbl -> get_Visible(This,pVal) ) 

#define IUltraMonMonCtl_put_Visible(This,newVal)	\
    ( (This)->lpVtbl -> put_Visible(This,newVal) ) 

#define IUltraMonMonCtl_RefreshMonitorLayout(This)	\
    ( (This)->lpVtbl -> RefreshMonitorLayout(This) ) 

#define IUltraMonMonCtl_Redraw(This,what)	\
    ( (This)->lpVtbl -> Redraw(This,what) ) 

#define IUltraMonMonCtl_Create(This)	\
    ( (This)->lpVtbl -> Create(This) ) 

#define IUltraMonMonCtl_Destroy(This)	\
    ( (This)->lpVtbl -> Destroy(This) ) 

#define IUltraMonMonCtl_MonCtlAdvise(This,pEvents,events,pCookie)	\
    ( (This)->lpVtbl -> MonCtlAdvise(This,pEvents,events,pCookie) ) 

#define IUltraMonMonCtl_MonCtlUnadvise(This,cookie)	\
    ( (This)->lpVtbl -> MonCtlUnadvise(This,cookie) ) 

#define IUltraMonMonCtl_CalcDesktopDimensions(This,left,top,width,height)	\
    ( (This)->lpVtbl -> CalcDesktopDimensions(This,left,top,width,height) ) 

#define IUltraMonMonCtl_get_Style(This,pVal)	\
    ( (This)->lpVtbl -> get_Style(This,pVal) ) 

#define IUltraMonMonCtl_put_Style(This,newVal)	\
    ( (This)->lpVtbl -> put_Style(This,newVal) ) 

#define IUltraMonMonCtl_get_Monitors(This,pVal)	\
    ( (This)->lpVtbl -> get_Monitors(This,pVal) ) 

#define IUltraMonMonCtl_get_AllowMonitorMoving(This,pVal)	\
    ( (This)->lpVtbl -> get_AllowMonitorMoving(This,pVal) ) 

#define IUltraMonMonCtl_put_AllowMonitorMoving(This,newVal)	\
    ( (This)->lpVtbl -> put_AllowMonitorMoving(This,newVal) ) 

#define IUltraMonMonCtl_get_ShowIgnoredMonitors(This,pVal)	\
    ( (This)->lpVtbl -> get_ShowIgnoredMonitors(This,pVal) ) 

#define IUltraMonMonCtl_put_ShowIgnoredMonitors(This,newVal)	\
    ( (This)->lpVtbl -> put_ShowIgnoredMonitors(This,newVal) ) 

#define IUltraMonMonCtl_get_Enabled(This,pVal)	\
    ( (This)->lpVtbl -> get_Enabled(This,pVal) ) 

#define IUltraMonMonCtl_put_Enabled(This,newVal)	\
    ( (This)->lpVtbl -> put_Enabled(This,newVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IUltraMonMonCtl_INTERFACE_DEFINED__ */


#ifndef __IUltraMonMonCtl2_INTERFACE_DEFINED__
#define __IUltraMonMonCtl2_INTERFACE_DEFINED__

/* interface IUltraMonMonCtl2 */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IUltraMonMonCtl2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("05EEE67D-95FA-4C09-B456-D87A6364A9CD")
    IUltraMonMonCtl2 : public IUltraMonMonCtl
    {
    public:
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_TooltipShowDelay( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_TooltipShowDelay( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_TooltipShowDuration( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_TooltipShowDuration( 
            /* [in] */ long newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IUltraMonMonCtl2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IUltraMonMonCtl2 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IUltraMonMonCtl2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IUltraMonMonCtl2 * This);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ParentWnd )( 
            IUltraMonMonCtl2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ParentWnd )( 
            IUltraMonMonCtl2 * This,
            /* [in] */ long newVal);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Left )( 
            IUltraMonMonCtl2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Left )( 
            IUltraMonMonCtl2 * This,
            /* [in] */ long newVal);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DrawMode )( 
            IUltraMonMonCtl2 * This,
            /* [retval][out] */ DRAWMODE *pVal);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_DrawMode )( 
            IUltraMonMonCtl2 * This,
            /* [in] */ DRAWMODE newVal);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Top )( 
            IUltraMonMonCtl2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Top )( 
            IUltraMonMonCtl2 * This,
            /* [in] */ long newVal);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Width )( 
            IUltraMonMonCtl2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Width )( 
            IUltraMonMonCtl2 * This,
            /* [in] */ long newVal);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Height )( 
            IUltraMonMonCtl2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Height )( 
            IUltraMonMonCtl2 * This,
            /* [in] */ long newVal);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_AutoRefresh )( 
            IUltraMonMonCtl2 * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_AutoRefresh )( 
            IUltraMonMonCtl2 * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ShowMonitorStatus )( 
            IUltraMonMonCtl2 * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ShowMonitorStatus )( 
            IUltraMonMonCtl2 * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SelDisabled )( 
            IUltraMonMonCtl2 * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_SelDisabled )( 
            IUltraMonMonCtl2 * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Selection )( 
            IUltraMonMonCtl2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Selection )( 
            IUltraMonMonCtl2 * This,
            /* [in] */ long newVal);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Visible )( 
            IUltraMonMonCtl2 * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Visible )( 
            IUltraMonMonCtl2 * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *RefreshMonitorLayout )( 
            IUltraMonMonCtl2 * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Redraw )( 
            IUltraMonMonCtl2 * This,
            /* [in] */ REDRAW what);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Create )( 
            IUltraMonMonCtl2 * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Destroy )( 
            IUltraMonMonCtl2 * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *MonCtlAdvise )( 
            IUltraMonMonCtl2 * This,
            /* [in] */ IUltraMonMonCtlEvents *pEvents,
            /* [in] */ long events,
            /* [retval][out] */ long *pCookie);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *MonCtlUnadvise )( 
            IUltraMonMonCtl2 * This,
            /* [in] */ long cookie);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *CalcDesktopDimensions )( 
            IUltraMonMonCtl2 * This,
            /* [out] */ long *left,
            /* [out] */ long *top,
            /* [out] */ long *width,
            /* [out] */ long *height);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Style )( 
            IUltraMonMonCtl2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Style )( 
            IUltraMonMonCtl2 * This,
            /* [in] */ long newVal);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Monitors )( 
            IUltraMonMonCtl2 * This,
            /* [retval][out] */ IUltraMonMonitorsMonCtl **pVal);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_AllowMonitorMoving )( 
            IUltraMonMonCtl2 * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_AllowMonitorMoving )( 
            IUltraMonMonCtl2 * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ShowIgnoredMonitors )( 
            IUltraMonMonCtl2 * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ShowIgnoredMonitors )( 
            IUltraMonMonCtl2 * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Enabled )( 
            IUltraMonMonCtl2 * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Enabled )( 
            IUltraMonMonCtl2 * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TooltipShowDelay )( 
            IUltraMonMonCtl2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_TooltipShowDelay )( 
            IUltraMonMonCtl2 * This,
            /* [in] */ long newVal);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TooltipShowDuration )( 
            IUltraMonMonCtl2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_TooltipShowDuration )( 
            IUltraMonMonCtl2 * This,
            /* [in] */ long newVal);
        
        END_INTERFACE
    } IUltraMonMonCtl2Vtbl;

    interface IUltraMonMonCtl2
    {
        CONST_VTBL struct IUltraMonMonCtl2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IUltraMonMonCtl2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IUltraMonMonCtl2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IUltraMonMonCtl2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IUltraMonMonCtl2_get_ParentWnd(This,pVal)	\
    ( (This)->lpVtbl -> get_ParentWnd(This,pVal) ) 

#define IUltraMonMonCtl2_put_ParentWnd(This,newVal)	\
    ( (This)->lpVtbl -> put_ParentWnd(This,newVal) ) 

#define IUltraMonMonCtl2_get_Left(This,pVal)	\
    ( (This)->lpVtbl -> get_Left(This,pVal) ) 

#define IUltraMonMonCtl2_put_Left(This,newVal)	\
    ( (This)->lpVtbl -> put_Left(This,newVal) ) 

#define IUltraMonMonCtl2_get_DrawMode(This,pVal)	\
    ( (This)->lpVtbl -> get_DrawMode(This,pVal) ) 

#define IUltraMonMonCtl2_put_DrawMode(This,newVal)	\
    ( (This)->lpVtbl -> put_DrawMode(This,newVal) ) 

#define IUltraMonMonCtl2_get_Top(This,pVal)	\
    ( (This)->lpVtbl -> get_Top(This,pVal) ) 

#define IUltraMonMonCtl2_put_Top(This,newVal)	\
    ( (This)->lpVtbl -> put_Top(This,newVal) ) 

#define IUltraMonMonCtl2_get_Width(This,pVal)	\
    ( (This)->lpVtbl -> get_Width(This,pVal) ) 

#define IUltraMonMonCtl2_put_Width(This,newVal)	\
    ( (This)->lpVtbl -> put_Width(This,newVal) ) 

#define IUltraMonMonCtl2_get_Height(This,pVal)	\
    ( (This)->lpVtbl -> get_Height(This,pVal) ) 

#define IUltraMonMonCtl2_put_Height(This,newVal)	\
    ( (This)->lpVtbl -> put_Height(This,newVal) ) 

#define IUltraMonMonCtl2_get_AutoRefresh(This,pVal)	\
    ( (This)->lpVtbl -> get_AutoRefresh(This,pVal) ) 

#define IUltraMonMonCtl2_put_AutoRefresh(This,newVal)	\
    ( (This)->lpVtbl -> put_AutoRefresh(This,newVal) ) 

#define IUltraMonMonCtl2_get_ShowMonitorStatus(This,pVal)	\
    ( (This)->lpVtbl -> get_ShowMonitorStatus(This,pVal) ) 

#define IUltraMonMonCtl2_put_ShowMonitorStatus(This,newVal)	\
    ( (This)->lpVtbl -> put_ShowMonitorStatus(This,newVal) ) 

#define IUltraMonMonCtl2_get_SelDisabled(This,pVal)	\
    ( (This)->lpVtbl -> get_SelDisabled(This,pVal) ) 

#define IUltraMonMonCtl2_put_SelDisabled(This,newVal)	\
    ( (This)->lpVtbl -> put_SelDisabled(This,newVal) ) 

#define IUltraMonMonCtl2_get_Selection(This,pVal)	\
    ( (This)->lpVtbl -> get_Selection(This,pVal) ) 

#define IUltraMonMonCtl2_put_Selection(This,newVal)	\
    ( (This)->lpVtbl -> put_Selection(This,newVal) ) 

#define IUltraMonMonCtl2_get_Visible(This,pVal)	\
    ( (This)->lpVtbl -> get_Visible(This,pVal) ) 

#define IUltraMonMonCtl2_put_Visible(This,newVal)	\
    ( (This)->lpVtbl -> put_Visible(This,newVal) ) 

#define IUltraMonMonCtl2_RefreshMonitorLayout(This)	\
    ( (This)->lpVtbl -> RefreshMonitorLayout(This) ) 

#define IUltraMonMonCtl2_Redraw(This,what)	\
    ( (This)->lpVtbl -> Redraw(This,what) ) 

#define IUltraMonMonCtl2_Create(This)	\
    ( (This)->lpVtbl -> Create(This) ) 

#define IUltraMonMonCtl2_Destroy(This)	\
    ( (This)->lpVtbl -> Destroy(This) ) 

#define IUltraMonMonCtl2_MonCtlAdvise(This,pEvents,events,pCookie)	\
    ( (This)->lpVtbl -> MonCtlAdvise(This,pEvents,events,pCookie) ) 

#define IUltraMonMonCtl2_MonCtlUnadvise(This,cookie)	\
    ( (This)->lpVtbl -> MonCtlUnadvise(This,cookie) ) 

#define IUltraMonMonCtl2_CalcDesktopDimensions(This,left,top,width,height)	\
    ( (This)->lpVtbl -> CalcDesktopDimensions(This,left,top,width,height) ) 

#define IUltraMonMonCtl2_get_Style(This,pVal)	\
    ( (This)->lpVtbl -> get_Style(This,pVal) ) 

#define IUltraMonMonCtl2_put_Style(This,newVal)	\
    ( (This)->lpVtbl -> put_Style(This,newVal) ) 

#define IUltraMonMonCtl2_get_Monitors(This,pVal)	\
    ( (This)->lpVtbl -> get_Monitors(This,pVal) ) 

#define IUltraMonMonCtl2_get_AllowMonitorMoving(This,pVal)	\
    ( (This)->lpVtbl -> get_AllowMonitorMoving(This,pVal) ) 

#define IUltraMonMonCtl2_put_AllowMonitorMoving(This,newVal)	\
    ( (This)->lpVtbl -> put_AllowMonitorMoving(This,newVal) ) 

#define IUltraMonMonCtl2_get_ShowIgnoredMonitors(This,pVal)	\
    ( (This)->lpVtbl -> get_ShowIgnoredMonitors(This,pVal) ) 

#define IUltraMonMonCtl2_put_ShowIgnoredMonitors(This,newVal)	\
    ( (This)->lpVtbl -> put_ShowIgnoredMonitors(This,newVal) ) 

#define IUltraMonMonCtl2_get_Enabled(This,pVal)	\
    ( (This)->lpVtbl -> get_Enabled(This,pVal) ) 

#define IUltraMonMonCtl2_put_Enabled(This,newVal)	\
    ( (This)->lpVtbl -> put_Enabled(This,newVal) ) 


#define IUltraMonMonCtl2_get_TooltipShowDelay(This,pVal)	\
    ( (This)->lpVtbl -> get_TooltipShowDelay(This,pVal) ) 

#define IUltraMonMonCtl2_put_TooltipShowDelay(This,newVal)	\
    ( (This)->lpVtbl -> put_TooltipShowDelay(This,newVal) ) 

#define IUltraMonMonCtl2_get_TooltipShowDuration(This,pVal)	\
    ( (This)->lpVtbl -> get_TooltipShowDuration(This,pVal) ) 

#define IUltraMonMonCtl2_put_TooltipShowDuration(This,newVal)	\
    ( (This)->lpVtbl -> put_TooltipShowDuration(This,newVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IUltraMonMonCtl2_INTERFACE_DEFINED__ */


#ifndef __IUltraMonMonitorMonCtl_INTERFACE_DEFINED__
#define __IUltraMonMonitorMonCtl_INTERFACE_DEFINED__

/* interface IUltraMonMonitorMonCtl */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IUltraMonMonitorMonCtl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F7006EDB-449D-4E3B-A3E4-CAA58BC1AA62")
    IUltraMonMonitorMonCtl : public IUltraMonMonitor
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ToolTip( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ToolTip( 
            /* [in] */ BSTR newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IUltraMonMonitorMonCtlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IUltraMonMonitorMonCtl * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IUltraMonMonitorMonCtl * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IUltraMonMonitorMonCtl * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IUltraMonMonitorMonCtl * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IUltraMonMonitorMonCtl * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IUltraMonMonitorMonCtl * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IUltraMonMonitorMonCtl * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ID )( 
            IUltraMonMonitorMonCtl * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IUltraMonMonitorMonCtl * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DeviceName )( 
            IUltraMonMonitorMonCtl * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_AdapterName )( 
            IUltraMonMonitorMonCtl * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_AdapterDeviceName )( 
            IUltraMonMonitorMonCtl * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Primary )( 
            IUltraMonMonitorMonCtl * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Primary )( 
            IUltraMonMonitorMonCtl * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Enabled )( 
            IUltraMonMonitorMonCtl * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Enabled )( 
            IUltraMonMonitorMonCtl * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Left )( 
            IUltraMonMonitorMonCtl * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Left )( 
            IUltraMonMonitorMonCtl * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Top )( 
            IUltraMonMonitorMonCtl * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Top )( 
            IUltraMonMonitorMonCtl * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Width )( 
            IUltraMonMonitorMonCtl * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Width )( 
            IUltraMonMonitorMonCtl * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Height )( 
            IUltraMonMonitorMonCtl * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Height )( 
            IUltraMonMonitorMonCtl * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_WorkLeft )( 
            IUltraMonMonitorMonCtl * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_WorkTop )( 
            IUltraMonMonitorMonCtl * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_WorkWidth )( 
            IUltraMonMonitorMonCtl * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_WorkHeight )( 
            IUltraMonMonitorMonCtl * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Colordepth )( 
            IUltraMonMonitorMonCtl * This,
            /* [retval][out] */ short *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Colordepth )( 
            IUltraMonMonitorMonCtl * This,
            /* [in] */ short newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RefreshRate )( 
            IUltraMonMonitorMonCtl * This,
            /* [retval][out] */ short *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_RefreshRate )( 
            IUltraMonMonitorMonCtl * This,
            /* [in] */ short newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HMonitor )( 
            IUltraMonMonitorMonCtl * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DeviceRegKey )( 
            IUltraMonMonitorMonCtl * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DisplayModes )( 
            IUltraMonMonitorMonCtl * This,
            /* [retval][out] */ IUltraMonDisplayModes **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HwAccel )( 
            IUltraMonMonitorMonCtl * This,
            /* [retval][out] */ HWACCEL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_HwAccel )( 
            IUltraMonMonitorMonCtl * This,
            /* [in] */ HWACCEL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Removable )( 
            IUltraMonMonitorMonCtl * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Detached )( 
            IUltraMonMonitorMonCtl * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_WindowsID )( 
            IUltraMonMonitorMonCtl * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ToolTip )( 
            IUltraMonMonitorMonCtl * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ToolTip )( 
            IUltraMonMonitorMonCtl * This,
            /* [in] */ BSTR newVal);
        
        END_INTERFACE
    } IUltraMonMonitorMonCtlVtbl;

    interface IUltraMonMonitorMonCtl
    {
        CONST_VTBL struct IUltraMonMonitorMonCtlVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IUltraMonMonitorMonCtl_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IUltraMonMonitorMonCtl_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IUltraMonMonitorMonCtl_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IUltraMonMonitorMonCtl_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IUltraMonMonitorMonCtl_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IUltraMonMonitorMonCtl_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IUltraMonMonitorMonCtl_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IUltraMonMonitorMonCtl_get_ID(This,pVal)	\
    ( (This)->lpVtbl -> get_ID(This,pVal) ) 

#define IUltraMonMonitorMonCtl_get_Name(This,pVal)	\
    ( (This)->lpVtbl -> get_Name(This,pVal) ) 

#define IUltraMonMonitorMonCtl_get_DeviceName(This,pVal)	\
    ( (This)->lpVtbl -> get_DeviceName(This,pVal) ) 

#define IUltraMonMonitorMonCtl_get_AdapterName(This,pVal)	\
    ( (This)->lpVtbl -> get_AdapterName(This,pVal) ) 

#define IUltraMonMonitorMonCtl_get_AdapterDeviceName(This,pVal)	\
    ( (This)->lpVtbl -> get_AdapterDeviceName(This,pVal) ) 

#define IUltraMonMonitorMonCtl_get_Primary(This,pVal)	\
    ( (This)->lpVtbl -> get_Primary(This,pVal) ) 

#define IUltraMonMonitorMonCtl_put_Primary(This,newVal)	\
    ( (This)->lpVtbl -> put_Primary(This,newVal) ) 

#define IUltraMonMonitorMonCtl_get_Enabled(This,pVal)	\
    ( (This)->lpVtbl -> get_Enabled(This,pVal) ) 

#define IUltraMonMonitorMonCtl_put_Enabled(This,newVal)	\
    ( (This)->lpVtbl -> put_Enabled(This,newVal) ) 

#define IUltraMonMonitorMonCtl_get_Left(This,pVal)	\
    ( (This)->lpVtbl -> get_Left(This,pVal) ) 

#define IUltraMonMonitorMonCtl_put_Left(This,newVal)	\
    ( (This)->lpVtbl -> put_Left(This,newVal) ) 

#define IUltraMonMonitorMonCtl_get_Top(This,pVal)	\
    ( (This)->lpVtbl -> get_Top(This,pVal) ) 

#define IUltraMonMonitorMonCtl_put_Top(This,newVal)	\
    ( (This)->lpVtbl -> put_Top(This,newVal) ) 

#define IUltraMonMonitorMonCtl_get_Width(This,pVal)	\
    ( (This)->lpVtbl -> get_Width(This,pVal) ) 

#define IUltraMonMonitorMonCtl_put_Width(This,newVal)	\
    ( (This)->lpVtbl -> put_Width(This,newVal) ) 

#define IUltraMonMonitorMonCtl_get_Height(This,pVal)	\
    ( (This)->lpVtbl -> get_Height(This,pVal) ) 

#define IUltraMonMonitorMonCtl_put_Height(This,newVal)	\
    ( (This)->lpVtbl -> put_Height(This,newVal) ) 

#define IUltraMonMonitorMonCtl_get_WorkLeft(This,pVal)	\
    ( (This)->lpVtbl -> get_WorkLeft(This,pVal) ) 

#define IUltraMonMonitorMonCtl_get_WorkTop(This,pVal)	\
    ( (This)->lpVtbl -> get_WorkTop(This,pVal) ) 

#define IUltraMonMonitorMonCtl_get_WorkWidth(This,pVal)	\
    ( (This)->lpVtbl -> get_WorkWidth(This,pVal) ) 

#define IUltraMonMonitorMonCtl_get_WorkHeight(This,pVal)	\
    ( (This)->lpVtbl -> get_WorkHeight(This,pVal) ) 

#define IUltraMonMonitorMonCtl_get_Colordepth(This,pVal)	\
    ( (This)->lpVtbl -> get_Colordepth(This,pVal) ) 

#define IUltraMonMonitorMonCtl_put_Colordepth(This,newVal)	\
    ( (This)->lpVtbl -> put_Colordepth(This,newVal) ) 

#define IUltraMonMonitorMonCtl_get_RefreshRate(This,pVal)	\
    ( (This)->lpVtbl -> get_RefreshRate(This,pVal) ) 

#define IUltraMonMonitorMonCtl_put_RefreshRate(This,newVal)	\
    ( (This)->lpVtbl -> put_RefreshRate(This,newVal) ) 

#define IUltraMonMonitorMonCtl_get_HMonitor(This,pVal)	\
    ( (This)->lpVtbl -> get_HMonitor(This,pVal) ) 

#define IUltraMonMonitorMonCtl_get_DeviceRegKey(This,pVal)	\
    ( (This)->lpVtbl -> get_DeviceRegKey(This,pVal) ) 

#define IUltraMonMonitorMonCtl_get_DisplayModes(This,pVal)	\
    ( (This)->lpVtbl -> get_DisplayModes(This,pVal) ) 

#define IUltraMonMonitorMonCtl_get_HwAccel(This,pVal)	\
    ( (This)->lpVtbl -> get_HwAccel(This,pVal) ) 

#define IUltraMonMonitorMonCtl_put_HwAccel(This,newVal)	\
    ( (This)->lpVtbl -> put_HwAccel(This,newVal) ) 

#define IUltraMonMonitorMonCtl_get_Removable(This,pVal)	\
    ( (This)->lpVtbl -> get_Removable(This,pVal) ) 

#define IUltraMonMonitorMonCtl_get_Detached(This,pVal)	\
    ( (This)->lpVtbl -> get_Detached(This,pVal) ) 

#define IUltraMonMonitorMonCtl_get_WindowsID(This,pVal)	\
    ( (This)->lpVtbl -> get_WindowsID(This,pVal) ) 


#define IUltraMonMonitorMonCtl_get_ToolTip(This,pVal)	\
    ( (This)->lpVtbl -> get_ToolTip(This,pVal) ) 

#define IUltraMonMonitorMonCtl_put_ToolTip(This,newVal)	\
    ( (This)->lpVtbl -> put_ToolTip(This,newVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IUltraMonMonitorMonCtl_INTERFACE_DEFINED__ */


#ifndef __IUltraMonMonCtlEvents2_INTERFACE_DEFINED__
#define __IUltraMonMonCtlEvents2_INTERFACE_DEFINED__

/* interface IUltraMonMonCtlEvents2 */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IUltraMonMonCtlEvents2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("18BD075F-7E07-4340-87DC-E25C64913FD9")
    IUltraMonMonCtlEvents2 : public IUltraMonMonCtlEvents
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnMouseClick( 
            /* [in] */ IUltraMonMonCtl *sender,
            /* [in] */ MOUSE_BTN btn,
            /* [in] */ VARIANT_BOOL btnUp,
            /* [in] */ long modifierKeys,
            /* [in] */ VARIANT_BOOL doubleClick,
            /* [in] */ long screenX,
            /* [in] */ long screenY,
            /* [unique][in] */ IUltraMonMonitorMonCtl *monitor) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IUltraMonMonCtlEvents2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IUltraMonMonCtlEvents2 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IUltraMonMonCtlEvents2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IUltraMonMonCtlEvents2 * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnDraw )( 
            IUltraMonMonCtlEvents2 * This,
            /* [in] */ IUltraMonMonCtl *sender,
            /* [in] */ IUltraMonMonitorMonCtl *monitor,
            /* [in] */ long hWnd);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnSelChange )( 
            IUltraMonMonCtlEvents2 * This,
            /* [in] */ IUltraMonMonCtl *sender,
            /* [unique][in] */ IUltraMonMonitorMonCtl *newSel,
            /* [unique][in] */ IUltraMonMonitorMonCtl *prevSel,
            /* [out] */ VARIANT_BOOL *cancel);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnBeginMove )( 
            IUltraMonMonCtlEvents2 * This,
            /* [in] */ IUltraMonMonCtl *sender,
            /* [in] */ IUltraMonMonitorMonCtl *monitor,
            /* [in] */ long x,
            /* [in] */ long y);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnMoving )( 
            IUltraMonMonCtlEvents2 * This,
            /* [in] */ IUltraMonMonCtl *sender,
            /* [in] */ IUltraMonMonitorMonCtl *monitor,
            /* [in] */ long x,
            /* [in] */ long y);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnEndMove )( 
            IUltraMonMonCtlEvents2 * This,
            /* [in] */ IUltraMonMonCtl *sender,
            /* [in] */ IUltraMonMonitorMonCtl *monitor,
            /* [in] */ long x,
            /* [in] */ long y,
            /* [in] */ VARIANT_BOOL canceled);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnMouseClick )( 
            IUltraMonMonCtlEvents2 * This,
            /* [in] */ IUltraMonMonCtl *sender,
            /* [in] */ MOUSE_BTN btn,
            /* [in] */ VARIANT_BOOL btnUp,
            /* [in] */ long modifierKeys,
            /* [in] */ VARIANT_BOOL doubleClick,
            /* [in] */ long screenX,
            /* [in] */ long screenY,
            /* [unique][in] */ IUltraMonMonitorMonCtl *monitor);
        
        END_INTERFACE
    } IUltraMonMonCtlEvents2Vtbl;

    interface IUltraMonMonCtlEvents2
    {
        CONST_VTBL struct IUltraMonMonCtlEvents2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IUltraMonMonCtlEvents2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IUltraMonMonCtlEvents2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IUltraMonMonCtlEvents2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IUltraMonMonCtlEvents2_OnDraw(This,sender,monitor,hWnd)	\
    ( (This)->lpVtbl -> OnDraw(This,sender,monitor,hWnd) ) 

#define IUltraMonMonCtlEvents2_OnSelChange(This,sender,newSel,prevSel,cancel)	\
    ( (This)->lpVtbl -> OnSelChange(This,sender,newSel,prevSel,cancel) ) 

#define IUltraMonMonCtlEvents2_OnBeginMove(This,sender,monitor,x,y)	\
    ( (This)->lpVtbl -> OnBeginMove(This,sender,monitor,x,y) ) 

#define IUltraMonMonCtlEvents2_OnMoving(This,sender,monitor,x,y)	\
    ( (This)->lpVtbl -> OnMoving(This,sender,monitor,x,y) ) 

#define IUltraMonMonCtlEvents2_OnEndMove(This,sender,monitor,x,y,canceled)	\
    ( (This)->lpVtbl -> OnEndMove(This,sender,monitor,x,y,canceled) ) 


#define IUltraMonMonCtlEvents2_OnMouseClick(This,sender,btn,btnUp,modifierKeys,doubleClick,screenX,screenY,monitor)	\
    ( (This)->lpVtbl -> OnMouseClick(This,sender,btn,btnUp,modifierKeys,doubleClick,screenX,screenY,monitor) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IUltraMonMonCtlEvents2_INTERFACE_DEFINED__ */


#ifndef __IUltraMonDisplayMode2_INTERFACE_DEFINED__
#define __IUltraMonDisplayMode2_INTERFACE_DEFINED__

/* interface IUltraMonDisplayMode2 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IUltraMonDisplayMode2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("19A4331E-7BFD-4e5d-9A48-FEF0131E5320")
    IUltraMonDisplayMode2 : public IUltraMonDisplayMode
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Interlaced( 
            /* [retval][out] */ VARIANT *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IUltraMonDisplayMode2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IUltraMonDisplayMode2 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IUltraMonDisplayMode2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IUltraMonDisplayMode2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IUltraMonDisplayMode2 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IUltraMonDisplayMode2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IUltraMonDisplayMode2 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IUltraMonDisplayMode2 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Width )( 
            IUltraMonDisplayMode2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Height )( 
            IUltraMonDisplayMode2 * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ColorDepth )( 
            IUltraMonDisplayMode2 * This,
            /* [retval][out] */ VARIANT *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RefreshRate )( 
            IUltraMonDisplayMode2 * This,
            /* [retval][out] */ VARIANT *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Interlaced )( 
            IUltraMonDisplayMode2 * This,
            /* [retval][out] */ VARIANT *pVal);
        
        END_INTERFACE
    } IUltraMonDisplayMode2Vtbl;

    interface IUltraMonDisplayMode2
    {
        CONST_VTBL struct IUltraMonDisplayMode2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IUltraMonDisplayMode2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IUltraMonDisplayMode2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IUltraMonDisplayMode2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IUltraMonDisplayMode2_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IUltraMonDisplayMode2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IUltraMonDisplayMode2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IUltraMonDisplayMode2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IUltraMonDisplayMode2_get_Width(This,pVal)	\
    ( (This)->lpVtbl -> get_Width(This,pVal) ) 

#define IUltraMonDisplayMode2_get_Height(This,pVal)	\
    ( (This)->lpVtbl -> get_Height(This,pVal) ) 

#define IUltraMonDisplayMode2_get_ColorDepth(This,pVal)	\
    ( (This)->lpVtbl -> get_ColorDepth(This,pVal) ) 

#define IUltraMonDisplayMode2_get_RefreshRate(This,pVal)	\
    ( (This)->lpVtbl -> get_RefreshRate(This,pVal) ) 


#define IUltraMonDisplayMode2_get_Interlaced(This,pVal)	\
    ( (This)->lpVtbl -> get_Interlaced(This,pVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IUltraMonDisplayMode2_INTERFACE_DEFINED__ */


#ifndef __IUltraMonAppBar_INTERFACE_DEFINED__
#define __IUltraMonAppBar_INTERFACE_DEFINED__

/* interface IUltraMonAppBar */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IUltraMonAppBar;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("E779F392-46E9-4781-B2B4-1CF060E4B313")
    IUltraMonAppBar : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Monitor( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Edge( 
            /* [retval][out] */ SCREEN_EDGE *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_HWnd( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_AutoHidden( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Left( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Top( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Width( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Height( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Visible( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Visible( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Move( 
            /* [in] */ long monitor,
            /* [in] */ SCREEN_EDGE edge) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IUltraMonAppBarVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IUltraMonAppBar * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IUltraMonAppBar * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IUltraMonAppBar * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IUltraMonAppBar * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IUltraMonAppBar * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IUltraMonAppBar * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IUltraMonAppBar * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IUltraMonAppBar * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Monitor )( 
            IUltraMonAppBar * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Edge )( 
            IUltraMonAppBar * This,
            /* [retval][out] */ SCREEN_EDGE *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HWnd )( 
            IUltraMonAppBar * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_AutoHidden )( 
            IUltraMonAppBar * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Left )( 
            IUltraMonAppBar * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Top )( 
            IUltraMonAppBar * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Width )( 
            IUltraMonAppBar * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Height )( 
            IUltraMonAppBar * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Visible )( 
            IUltraMonAppBar * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Visible )( 
            IUltraMonAppBar * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Move )( 
            IUltraMonAppBar * This,
            /* [in] */ long monitor,
            /* [in] */ SCREEN_EDGE edge);
        
        END_INTERFACE
    } IUltraMonAppBarVtbl;

    interface IUltraMonAppBar
    {
        CONST_VTBL struct IUltraMonAppBarVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IUltraMonAppBar_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IUltraMonAppBar_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IUltraMonAppBar_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IUltraMonAppBar_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IUltraMonAppBar_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IUltraMonAppBar_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IUltraMonAppBar_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IUltraMonAppBar_get_Name(This,pVal)	\
    ( (This)->lpVtbl -> get_Name(This,pVal) ) 

#define IUltraMonAppBar_get_Monitor(This,pVal)	\
    ( (This)->lpVtbl -> get_Monitor(This,pVal) ) 

#define IUltraMonAppBar_get_Edge(This,pVal)	\
    ( (This)->lpVtbl -> get_Edge(This,pVal) ) 

#define IUltraMonAppBar_get_HWnd(This,pVal)	\
    ( (This)->lpVtbl -> get_HWnd(This,pVal) ) 

#define IUltraMonAppBar_get_AutoHidden(This,pVal)	\
    ( (This)->lpVtbl -> get_AutoHidden(This,pVal) ) 

#define IUltraMonAppBar_get_Left(This,pVal)	\
    ( (This)->lpVtbl -> get_Left(This,pVal) ) 

#define IUltraMonAppBar_get_Top(This,pVal)	\
    ( (This)->lpVtbl -> get_Top(This,pVal) ) 

#define IUltraMonAppBar_get_Width(This,pVal)	\
    ( (This)->lpVtbl -> get_Width(This,pVal) ) 

#define IUltraMonAppBar_get_Height(This,pVal)	\
    ( (This)->lpVtbl -> get_Height(This,pVal) ) 

#define IUltraMonAppBar_get_Visible(This,pVal)	\
    ( (This)->lpVtbl -> get_Visible(This,pVal) ) 

#define IUltraMonAppBar_put_Visible(This,newVal)	\
    ( (This)->lpVtbl -> put_Visible(This,newVal) ) 

#define IUltraMonAppBar_Move(This,monitor,edge)	\
    ( (This)->lpVtbl -> Move(This,monitor,edge) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IUltraMonAppBar_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_UltraMonSystem;

#ifdef __cplusplus

class DECLSPEC_UUID("1BC64DEE-F18D-42FF-8C10-E375F2F1E501")
UltraMonSystem;
#endif

EXTERN_C const CLSID CLSID_UltraMonWindow;

#ifdef __cplusplus

class DECLSPEC_UUID("9E6677A4-3C7A-4136-81DD-E2A4CF433259")
UltraMonWindow;
#endif

EXTERN_C const CLSID CLSID_UltraMonUtility;

#ifdef __cplusplus

class DECLSPEC_UUID("275DB82D-B1AB-40E2-A70A-B1ACBCD81B38")
UltraMonUtility;
#endif

EXTERN_C const CLSID CLSID_UltraMonMonCtl;

#ifdef __cplusplus

class DECLSPEC_UUID("282E7CC0-E359-4B7F-AE4C-21BC436A1D94")
UltraMonMonCtl;
#endif
#endif /* __ULTRAMONLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


