/* Generated by wbuild from "Common.w"
** (generator version $Revision: 404 $ of $Date: 1999-09-28 09:56:30 -0600 (Tue, 28 Sep 1999) $)
*/
#ifndef _XfwfCommonP_H_
#define _XfwfCommonP_H_
#include <X11/CompositeP.h>
#include "Common.h"
_XFUNCPROTOBEGIN
#define max(a, b) ((a )>(b )?(a ):(b ))


#define min(a, b) ((a )<(b )?(a ):(b ))


#define abs(a) ((a )<0 ?-(a ):(a ))


typedef void (*compute_inside_Proc)(
#if NeedFunctionPrototypes
Widget,Position *,Position *,Dimension *,Dimension *
#endif
);
#define XtInherit_compute_inside ((compute_inside_Proc) _XtInherit)
typedef Dimension  (*total_frame_width_Proc)(
#if NeedFunctionPrototypes
Widget
#endif
);
#define XtInherit_total_frame_width ((total_frame_width_Proc) _XtInherit)
typedef void (*highlight_border_Proc)(
#if NeedFunctionPrototypes
Widget
#endif
);
#define XtInherit_highlight_border ((highlight_border_Proc) _XtInherit)
typedef void (*unhighlight_border_Proc)(
#if NeedFunctionPrototypes
Widget
#endif
);
#define XtInherit_unhighlight_border ((unhighlight_border_Proc) _XtInherit)
typedef Boolean  (*would_accept_focus_Proc)(
#if NeedFunctionPrototypes
Widget
#endif
);
#define XtInherit_would_accept_focus ((would_accept_focus_Proc) _XtInherit)
typedef void (*traverse_Proc)(
#if NeedFunctionPrototypes
Widget,TraversalDirection ,Widget ,Time *
#endif
);
#define XtInherit_traverse ((traverse_Proc) _XtInherit)
typedef Boolean  (*lighter_color_Proc)(
#if NeedFunctionPrototypes
Widget,Pixel ,Pixel *
#endif
);
#define XtInherit_lighter_color ((lighter_color_Proc) _XtInherit)
typedef Boolean  (*darker_color_Proc)(
#if NeedFunctionPrototypes
Widget,Pixel ,Pixel *
#endif
);
#define XtInherit_darker_color ((darker_color_Proc) _XtInherit)

typedef struct {
/* methods */
compute_inside_Proc compute_inside;
total_frame_width_Proc total_frame_width;
highlight_border_Proc highlight_border;
unhighlight_border_Proc unhighlight_border;
would_accept_focus_Proc would_accept_focus;
traverse_Proc traverse;
lighter_color_Proc lighter_color;
darker_color_Proc darker_color;
/* class variables */
XtTranslations  traversal_trans;
} XfwfCommonClassPart;

typedef struct _XfwfCommonClassRec {
CoreClassPart core_class;
CompositeClassPart composite_class;
XfwfCommonClassPart xfwfCommon_class;
} XfwfCommonClassRec;

typedef struct {
/* resources */
Boolean  useXCC;
Boolean  usePrivateColormap;
Boolean  useStandardColormaps;
Atom  standardColormap;
XCC  xcc;
Boolean  traversalOn;
Dimension  highlightThickness;
Color  highlightColor;
Color  background;
Pixmap  highlightPixmap;
XtCallbackList  nextTop;
XtPointer  userData;
/* private state */
Bool  traversal_focus;
GC  bordergc;
Bool  own_xcc;
} XfwfCommonPart;

typedef struct _XfwfCommonRec {
CorePart core;
CompositePart composite;
XfwfCommonPart xfwfCommon;
} XfwfCommonRec;

externalref XfwfCommonClassRec xfwfCommonClassRec;

_XFUNCPROTOEND
#endif /* _XfwfCommonP_H_ */
