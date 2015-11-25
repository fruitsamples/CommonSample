// Quickdraw 3D sample code//// Nick Thompson, AppleLink: DEVSUPPORT (devsupport@applelink.apple.com)//// updated for 1.5.3 8/13/97//// �1994-7 Apple Computer Inc., All Rights Reserved#include <Dialogs.h>#include <Memory.h>#include <SegLoad.h>#include <TextUtils.h>#include "QD3D.h"#include "QD3DErrors.h"#include "MyErrorHandler.h"//---------------------------------------------------------------------------const int kErrorHandlerAlertID = 21032 ;			// ID of the alert resource//---------------------------------------------------------------------------// THIS ENUM MUST MATCH THE STRUCT BELOWenum {	kWarningString,	kErrorString,	kFirstErrorString,	kLastErrorString,	kSpaceString,	kOpenParen,	kCloseParen,	kUnknownErrorString} ;// use these to construct intelligable messagesstatic struct {	Str255		errorString ;} stringTable[] = {	"\pQuickDraw 3D warning.\r\r", 	"\pQuickDraw 3D Error.\r\r",	"\p\rFirst Error: ",	"\p\rLast Error: "	"\p ",	"\p(",	"\p)",	"\p\rUnknown Message"} ;//---------------------------------------------------------------------------/*	To generate these tables, copy/paste the error codes from QD3DErrors.h	and use this regexp:		Find:		kQ3Error([0-9A-z]*),	Replace:	kQ3Error\1, "\\pkQ3Error\1",		Find:		kQ3Warning([0-9A-z]*),	Replace:	kQ3Warning\1, "\\pkQ3Warning\1",		Find:		kQ3Notice([0-9A-z]*),	Replace:	kQ3Notice\1, "\\pkQ3Notice\1",	*//* define a table for all of the QuickDraw 3D warning codes */struct {	TQ3Warning	theWarning ;	Str255		theString ;} warningTable[] = {	kQ3WarningNone, 							"\pkQ3WarningNone",	/* General System */	kQ3WarningInternalException, 				"\pkQ3WarningInternalException",		/* Object Warnings */	kQ3WarningNoObjectSupportForDuplicateMethod, "\pkQ3WarningNoObjectSupportForDuplicateMethod",	kQ3WarningNoObjectSupportForDrawMethod, 	"\pkQ3WarningNoObjectSupportForDrawMethod",	kQ3WarningNoObjectSupportForWriteMethod, 	"\pkQ3WarningNoObjectSupportForWriteMethod",	kQ3WarningNoObjectSupportForReadMethod, 	"\pkQ3WarningNoObjectSupportForReadMethod",	kQ3WarningUnknownElementType, 				"\pkQ3WarningUnknownElementType",	kQ3WarningTypeAndMethodAlreadyDefined, 		"\pkQ3WarningTypeAndMethodAlreadyDefined",	kQ3WarningTypeIsOutOfRange, 				"\pkQ3WarningTypeIsOutOfRange",	kQ3WarningTypeHasNotBeenRegistered, 		"\pkQ3WarningTypeHasNotBeenRegistered",	/* Parameter Warnings */	kQ3WarningVector3DNotUnitLength, 			"\pkQ3WarningVector3DNotUnitLength",	/* IO Warnings */	kQ3WarningInvalidSubObjectForObject, 		"\pkQ3WarningInvalidSubObjectForObject",	kQ3WarningInvalidHexString, 				"\pkQ3WarningInvalidHexString",	kQ3WarningUnknownObject, 					"\pkQ3WarningUnknownObject",	kQ3WarningInvalidMetafileObject, 			"\pkQ3WarningInvalidMetafileObject",	kQ3WarningUnmatchedBeginGroup, 				"\pkQ3WarningUnmatchedBeginGroup",	kQ3WarningUnmatchedEndGroup, 				"\pkQ3WarningUnmatchedEndGroup",	kQ3WarningInvalidTableOfContents, 			"\pkQ3WarningInvalidTableOfContents",	kQ3WarningUnresolvableReference, 			"\pkQ3WarningUnresolvableReference",	kQ3WarningNoAttachMethod, 					"\pkQ3WarningNoAttachMethod",	kQ3WarningInconsistentData, 				"\pkQ3WarningInconsistentData",	kQ3WarningReadLessThanSize, 				"\pkQ3WarningReadLessThanSize",	kQ3WarningFilePointerResolutionFailed, 		"\pkQ3WarningFilePointerResolutionFailed",	kQ3WarningFilePointerRedefined, 			"\pkQ3WarningFilePointerRedefined",	kQ3WarningStringExceedsMaximumLength, 		"\pkQ3WarningStringExceedsMaximumLength",	/* Memory Warnings */	kQ3WarningLowMemory, 						"\pkQ3WarningLowMemory",	kQ3WarningPossibleMemoryLeak, 				"\pkQ3WarningPossibleMemoryLeak",	/* View Warnings */	kQ3WarningViewTraversalInProgress, 			"\pkQ3WarningViewTraversalInProgress",	kQ3WarningNonInvertibleMatrix, 				"\pkQ3WarningNonInvertibleMatrix",	/* Quaternion Warning */	kQ3WarningQuaternionEntriesAreZero, 		"\pkQ3WarningQuaternionEntriesAreZero",	/* Renderer Warning */	kQ3WarningFunctionalityNotSupported, 		"\pkQ3WarningFunctionalityNotSupported",	/* DrawContext Warning */	kQ3WarningInvalidPaneDimensions, 			"\pkQ3WarningInvalidPaneDimensions",	/* Pick Warning */	kQ3WarningPickParamOutside, 				"\pkQ3WarningPickParamOutside",	/* Scale Warnings */	kQ3WarningScaleEntriesAllZero, 				"\pkQ3WarningScaleEntriesAllZero",	kQ3WarningScaleContainsNegativeEntries, 	"\pkQ3WarningScaleContainsNegativeEntries",	/* Generic Warnings */	kQ3WarningParameterOutOfRange, 				"\pkQ3WarningParameterOutOfRange"} ;struct {	TQ3Error	theError ;	Str255		theString ;} errorTable[] = {	kQ3ErrorNone,							"\pkQ3ErrorNone",			/* Fatal Errors */	kQ3ErrorInternalError,					"\pkQ3ErrorInternalError",	kQ3ErrorNoRecovery,						"\pkQ3ErrorNoRecovery",	kQ3ErrorLastFatalError,					"\pkQ3ErrorLastFatalError",	/* System Errors */	kQ3ErrorNotInitialized,					"\pkQ3ErrorNotInitialized",	kQ3ErrorAlreadyInitialized,				"\pkQ3ErrorAlreadyInitialized",	kQ3ErrorUnimplemented,					"\pkQ3ErrorUnimplemented",	kQ3ErrorRegistrationFailed,				"\pkQ3ErrorRegistrationFailed",	/* OS Errors */	kQ3ErrorUnixError,						"\pkQ3ErrorUnixError",	kQ3ErrorMacintoshError,					"\pkQ3ErrorMacintoshError",	kQ3ErrorX11Error,						"\pkQ3ErrorX11Error",	/* Memory Errors */	kQ3ErrorMemoryLeak,						"\pkQ3ErrorMemoryLeak",	kQ3ErrorOutOfMemory,					"\pkQ3ErrorOutOfMemory",	/* Parameter errors */	kQ3ErrorNULLParameter,					"\pkQ3ErrorNULLParameter",	kQ3ErrorParameterOutOfRange,			"\pkQ3ErrorParameterOutOfRange",	kQ3ErrorInvalidParameter,				"\pkQ3ErrorInvalidParameter",				kQ3ErrorInvalidData,					"\pkQ3ErrorInvalidData",					kQ3ErrorAcceleratorAlreadySet,			"\pkQ3ErrorAcceleratorAlreadySet",			kQ3ErrorVector3DNotUnitLength,			"\pkQ3ErrorVector3DNotUnitLength",	kQ3ErrorVector3DZeroLength,				"\pkQ3ErrorVector3DZeroLength",	/* Object Errors */	kQ3ErrorInvalidObject,					"\pkQ3ErrorInvalidObject",	kQ3ErrorInvalidObjectClass,				"\pkQ3ErrorInvalidObjectClass",	kQ3ErrorInvalidObjectType,				"\pkQ3ErrorInvalidObjectType",	kQ3ErrorInvalidObjectName,				"\pkQ3ErrorInvalidObjectName",	kQ3ErrorObjectClassInUse,				"\pkQ3ErrorObjectClassInUse",				kQ3ErrorAccessRestricted,				"\pkQ3ErrorAccessRestricted",	kQ3ErrorMetaHandlerRequired,			"\pkQ3ErrorMetaHandlerRequired",	kQ3ErrorNeedRequiredMethods,			"\pkQ3ErrorNeedRequiredMethods",	kQ3ErrorNoSubClassType,					"\pkQ3ErrorNoSubClassType",	kQ3ErrorUnknownElementType,				"\pkQ3ErrorUnknownElementType",	kQ3ErrorNotSupported,					"\pkQ3ErrorNotSupported",	/* Extension Errors */	kQ3ErrorNoExtensionsFolder,				"\pkQ3ErrorNoExtensionsFolder",	kQ3ErrorExtensionError,					"\pkQ3ErrorExtensionError",	kQ3ErrorPrivateExtensionError,			"\pkQ3ErrorPrivateExtensionError",	/* Geometry Errors */	kQ3ErrorDegenerateGeometry,				"\pkQ3ErrorDegenerateGeometry",	kQ3ErrorGeometryInsufficientNumberOfPoints,	"\pkQ3ErrorGeometryInsufficientNumberOfPoints",	/* IO Errors */	kQ3ErrorNoStorageSetForFile,			"\pkQ3ErrorNoStorageSetForFile",	kQ3ErrorEndOfFile,						"\pkQ3ErrorEndOfFile",	kQ3ErrorFileCancelled,					"\pkQ3ErrorFileCancelled",	kQ3ErrorInvalidMetafile,				"\pkQ3ErrorInvalidMetafile", 	kQ3ErrorInvalidMetafilePrimitive,		"\pkQ3ErrorInvalidMetafilePrimitive", 	kQ3ErrorInvalidMetafileLabel,			"\pkQ3ErrorInvalidMetafileLabel", 	kQ3ErrorInvalidMetafileObject,			"\pkQ3ErrorInvalidMetafileObject", 	kQ3ErrorInvalidMetafileSubObject,		"\pkQ3ErrorInvalidMetafileSubObject",	kQ3ErrorInvalidSubObjectForObject,		"\pkQ3ErrorInvalidSubObjectForObject",	kQ3ErrorUnresolvableReference,			"\pkQ3ErrorUnresolvableReference",	kQ3ErrorUnknownObject,					"\pkQ3ErrorUnknownObject",	kQ3ErrorStorageInUse,					"\pkQ3ErrorStorageInUse",	kQ3ErrorStorageAlreadyOpen,				"\pkQ3ErrorStorageAlreadyOpen",	kQ3ErrorStorageNotOpen,					"\pkQ3ErrorStorageNotOpen",	kQ3ErrorStorageIsOpen,					"\pkQ3ErrorStorageIsOpen",	kQ3ErrorFileAlreadyOpen,				"\pkQ3ErrorFileAlreadyOpen",	kQ3ErrorFileNotOpen,					"\pkQ3ErrorFileNotOpen",	kQ3ErrorFileIsOpen,						"\pkQ3ErrorFileIsOpen",	kQ3ErrorBeginWriteAlreadyCalled,		"\pkQ3ErrorBeginWriteAlreadyCalled",	kQ3ErrorBeginWriteNotCalled,			"\pkQ3ErrorBeginWriteNotCalled",	kQ3ErrorEndWriteNotCalled,				"\pkQ3ErrorEndWriteNotCalled",	kQ3ErrorReadStateInactive,				"\pkQ3ErrorReadStateInactive",	kQ3ErrorStateUnavailable,				"\pkQ3ErrorStateUnavailable",	kQ3ErrorWriteStateInactive,				"\pkQ3ErrorWriteStateInactive",	kQ3ErrorSizeNotLongAligned,				"\pkQ3ErrorSizeNotLongAligned",	kQ3ErrorFileModeRestriction,			"\pkQ3ErrorFileModeRestriction",	kQ3ErrorInvalidHexString,				"\pkQ3ErrorInvalidHexString",	kQ3ErrorWroteMoreThanSize,				"\pkQ3ErrorWroteMoreThanSize",	kQ3ErrorWroteLessThanSize,				"\pkQ3ErrorWroteLessThanSize",	kQ3ErrorReadLessThanSize,				"\pkQ3ErrorReadLessThanSize",	kQ3ErrorReadMoreThanSize,				"\pkQ3ErrorReadMoreThanSize",	kQ3ErrorNoBeginGroup,					"\pkQ3ErrorNoBeginGroup",	kQ3ErrorSizeMismatch,					"\pkQ3ErrorSizeMismatch",	kQ3ErrorStringExceedsMaximumLength,		"\pkQ3ErrorStringExceedsMaximumLength",	kQ3ErrorValueExceedsMaximumSize,		"\pkQ3ErrorValueExceedsMaximumSize",	kQ3ErrorNonUniqueLabel,					"\pkQ3ErrorNonUniqueLabel",	kQ3ErrorEndOfContainer,					"\pkQ3ErrorEndOfContainer",	kQ3ErrorUnmatchedEndGroup,				"\pkQ3ErrorUnmatchedEndGroup",	kQ3ErrorFileVersionExists,				"\pkQ3ErrorFileVersionExists",	/* View errors */	kQ3ErrorViewNotStarted,					"\pkQ3ErrorViewNotStarted",	kQ3ErrorViewIsStarted,					"\pkQ3ErrorViewIsStarted",	kQ3ErrorRendererNotSet,					"\pkQ3ErrorRendererNotSet",	kQ3ErrorRenderingIsActive,				"\pkQ3ErrorRenderingIsActive",	kQ3ErrorImmediateModeUnderflow,			"\pkQ3ErrorImmediateModeUnderflow",	kQ3ErrorDisplayNotSet,					"\pkQ3ErrorDisplayNotSet",	kQ3ErrorCameraNotSet,					"\pkQ3ErrorCameraNotSet",	kQ3ErrorDrawContextNotSet,				"\pkQ3ErrorDrawContextNotSet",	kQ3ErrorNonInvertibleMatrix,			"\pkQ3ErrorNonInvertibleMatrix",	kQ3ErrorRenderingNotStarted,			"\pkQ3ErrorRenderingNotStarted",	kQ3ErrorPickingNotStarted,				"\pkQ3ErrorPickingNotStarted",	kQ3ErrorBoundsNotStarted,				"\pkQ3ErrorBoundsNotStarted",	kQ3ErrorDataNotAvailable,				"\pkQ3ErrorDataNotAvailable",	kQ3ErrorNothingToPop,					"\pkQ3ErrorNothingToPop",	/* Renderer Errors */	kQ3ErrorUnknownStudioType,				"\pkQ3ErrorUnknownStudioType",				kQ3ErrorAlreadyRendering,				"\pkQ3ErrorAlreadyRendering",	kQ3ErrorStartGroupRange,				"\pkQ3ErrorStartGroupRange",	kQ3ErrorUnsupportedGeometryType,		"\pkQ3ErrorUnsupportedGeometryType",	kQ3ErrorInvalidGeometryType,			"\pkQ3ErrorInvalidGeometryType",	kQ3ErrorUnsupportedFunctionality,		"\pkQ3ErrorUnsupportedFunctionality",	/* Group Errors */	kQ3ErrorInvalidPositionForGroup,		"\pkQ3ErrorInvalidPositionForGroup",	kQ3ErrorInvalidObjectForGroup,			"\pkQ3ErrorInvalidObjectForGroup",	kQ3ErrorInvalidObjectForPosition,		"\pkQ3ErrorInvalidObjectForPosition",	/* Transform Errors */	kQ3ErrorScaleOfZero,					"\pkQ3ErrorScaleOfZero",					/* String Errors */	kQ3ErrorBadStringType,					"\pkQ3ErrorBadStringType",					/* Attribute Errors */	kQ3ErrorAttributeNotContained,			"\pkQ3ErrorAttributeNotContained",			kQ3ErrorAttributeInvalidType,			"\pkQ3ErrorAttributeInvalidType",			/* Camera Errors */	kQ3ErrorInvalidCameraValues,			"\pkQ3ErrorInvalidCameraValues",			/* DrawContext Errors */	kQ3ErrorBadDrawContextType,				"\pkQ3ErrorBadDrawContextType",	kQ3ErrorBadDrawContextFlag,				"\pkQ3ErrorBadDrawContextFlag",	kQ3ErrorBadDrawContext,					"\pkQ3ErrorBadDrawContext",	kQ3ErrorUnsupportedPixelDepth,			"\pkQ3ErrorUnsupportedPixelDepth"} ;static void ConcatenateString( Str255 firstString, Str255 addThis ) ;static void CopyString( Str255 firstString, Str255 fromThis ) ;static void PresentErrMsgInDialog( Str255 theError ) ;//-------------------------------------------------------------------------------------------// MyErrorHandler - handle warnings from QuickDraw 3dvoid MyErrorHandler( TQ3Error sticky, TQ3Error latest, long refCon ){#ifndef DONT_WARN_ME	Str255 	myErrorStr ;	Str255	theWholeString ;	short	stickyIndex = sticky - kQ3ErrorInternalError ;	short	latestIndex = latest - kQ3ErrorInternalError ;		// if the error is out of range, then just tell us that something	// "unknown" happened. Otherwise look up the error from the table.	if( sticky != kQ3ErrorNone && ( sticky < kQ3ErrorInternalError || sticky > kQ3ErrorUnsupportedPixelDepth )) {		CopyString(	theWholeString, stringTable[kErrorString].errorString  ) ;		ConcatenateString( theWholeString, stringTable[ kUnknownErrorString ].errorString ) ;	}	else {						CopyString(	theWholeString, stringTable[ kErrorString ].errorString  ) ;				ConcatenateString( theWholeString, stringTable[ kFirstErrorString ].errorString ) ;		CopyString(	myErrorStr, errorTable[ stickyIndex ].theString) ;		ConcatenateString( theWholeString, myErrorStr) ;				ConcatenateString( theWholeString, stringTable[ kLastErrorString ].errorString ) ;		CopyString(	myErrorStr, errorTable[ stickyIndex ].theString) ;		ConcatenateString( theWholeString, myErrorStr) ;				// and display the error string 		PresentErrMsgInDialog( theWholeString ) ;	}		// clear down the error	latest = Q3Error_Get( &sticky );#endif	}//-------------------------------------------------------------------------------------------// MyErrorHandler - handle warnings from QuickDraw 3dvoid MyWarningHandler( TQ3Warning 	sticky, TQ3Warning 	latest, long refCon ){#ifndef DONT_WARN_ME	Str255 	myWarningStr ;	Str255	theWholeString ;	short	stickyIndex = sticky - kQ3WarningInternalException ;	short	latestIndex = latest - kQ3WarningInternalException ;		// if the error is out of range, then just tell us that something	// "unknown" happened. Otherwise look up the error from the table.	if( sticky != kQ3ErrorNone && ( sticky < kQ3WarningInternalException || sticky > kQ3WarningParameterOutOfRange )) {			CopyString(	theWholeString, stringTable[ kWarningString ].errorString  ) ;		ConcatenateString( theWholeString, stringTable[ kUnknownErrorString ].errorString ) ;			}	else {				CopyString(	theWholeString, stringTable[ kWarningString ].errorString  ) ;				ConcatenateString( theWholeString, stringTable[ kFirstErrorString ].errorString ) ;		CopyString(	myWarningStr, warningTable[ stickyIndex ].theString) ;		ConcatenateString( theWholeString, myWarningStr) ;				ConcatenateString( theWholeString, stringTable[ kLastErrorString ].errorString ) ;		CopyString(	myWarningStr, warningTable[ stickyIndex ].theString) ;		ConcatenateString( theWholeString, myWarningStr) ;				PresentErrMsgInDialog( theWholeString ) ;		}		// clear down the error	latest = Q3Warning_Get( &sticky );	#endif}//------------------------------------------------------------------------static void ConcatenateString( Str255 firstString, Str255 addThis ){	BlockMove( &addThis[1], &firstString[firstString[0]+1], addThis[0] ) ;	firstString[0] += addThis[0] ;}//------------------------------------------------------------------------static void CopyString( Str255 firstString, Str255 fromThis ){	BlockMove( &fromThis[1], &firstString[1], fromThis[0] ) ;	firstString[0] = fromThis[0] ;}//------------------------------------------------------------------------static void PresentErrMsgInDialog( Str255 theError ){		short	itemHit ;	ParamText( theError, 0L,  0L,  0L ) ;	itemHit = StopAlert( kErrorHandlerAlertID, nil ) ;		if( itemHit == 2 ) 		ExitToShell() ;	else if( itemHit == 3 ) 		Debugger() ;		}