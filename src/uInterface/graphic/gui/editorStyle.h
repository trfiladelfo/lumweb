/**
 * \addtogroup Graphic
 * @{
 *
 * \author Anziner, Hahn
 * \brief
 *
 */

#ifndef EDITORSTYLE_H_
#define EDITORSTYLE_H_

#include "grlib/grlib.h"
#include "grlib/checkbox.h"
#include "graphic/gui/displayStyle.h"

// SubmitButton
#define EDITOR_SUBMIT_BUTTON_TOP					208
#define EDITOR_SUBMIT_BUTTON_LEFT					161
#define EDITOR_SUBMIT_BUTTON_WIDTH					157
#define EDITOR_SUBMIT_BUTTON_HEIGHT					30
#define EDITOR_SUBMIT_BUTTON_COLOR 					ClrWhite
#define EDITOR_SUBMIT_BUTTON_OUTLINE_COLOR			ClrGreen
#define EDITOR_SUBMIT_BUTTON_BACKGROUND_COLOR		ClrDarkGreen
#define EDITOR_SUBMIT_BUTTON_PUSH_COLOR				ClrLightGreen
#define EDITOR_SUBMIT_BUTTON_FONT					&g_sFontCm20b
#define EDITOR_SUBMIT_BUTTON_STYLE					PB_STYLE_FILL | PB_STYLE_OUTLINE | PB_STYLE_TEXT
#define EDITOR_SUBMIT_BUTTON_TEXT					"OK"
#define EDITOR_SUBMIT_BUTTON_ACTION					vExitEditor

// CancelButton
#define EDITOR_CANCEL_BUTTON_TOP					208
#define EDITOR_CANCEL_BUTTON_LEFT					2
#define EDITOR_CANCEL_BUTTON_WIDTH					157
#define EDITOR_CANCEL_BUTTON_HEIGHT					30
#define EDITOR_CANCEL_BUTTON_COLOR 					ClrWhite
#define EDITOR_CANCEL_BUTTON_OUTLINE_COLOR			ClrGreen
#define EDITOR_CANCEL_BUTTON_BACKGROUND_COLOR		ClrDarkGreen
#define EDITOR_CANCEL_BUTTON_PUSH_COLOR				ClrLightGreen
#define EDITOR_CANCEL_BUTTON_FONT					&g_sFontCm20b
#define EDITOR_CANCEL_BUTTON_STYLE					PB_STYLE_FILL | PB_STYLE_OUTLINE | PB_STYLE_TEXT
#define EDITOR_CANCEL_BUTTON_TEXT					"X"
#define EDITOR_CANCEL_BUTTON_ACTION					vTouchEditorCancel

// Increase1Button
#define EDITOR_INC_1_BUTTON_TOP						150
#define EDITOR_INC_1_BUTTON_LEFT					44
#define EDITOR_INC_1_BUTTON_WIDTH					40
#define EDITOR_INC_1_BUTTON_HEIGHT					40
#define EDITOR_INC_1_BUTTON_COLOR 					ClrWhite
#define EDITOR_INC_1_BUTTON_OUTLINE_COLOR			ClrBlue
#define EDITOR_INC_1_BUTTON_BACKGROUND_COLOR		ClrDarkBlue
#define EDITOR_INC_1_BUTTON_PUSH_COLOR				ClrLightBlue
#define EDITOR_INC_1_BUTTON_FONT					&g_sFontCm24
#define EDITOR_INC_1_BUTTON_STYLE					(PB_STYLE_FILL | PB_STYLE_OUTLINE | PB_STYLE_TEXT)
#define EDITOR_INC_1_BUTTON_TEXT					"+"
#define EDITOR_INC_1_BUTTON_ACTION					vIncreaseValue

// Increase2Button
#define EDITOR_INC_2_BUTTON_TOP						150
#define EDITOR_INC_2_BUTTON_LEFT					174
#define EDITOR_INC_2_BUTTON_WIDTH					40
#define EDITOR_INC_2_BUTTON_HEIGHT					40
#define EDITOR_INC_2_BUTTON_COLOR 					ClrWhite
#define EDITOR_INC_2_BUTTON_OUTLINE_COLOR			ClrBlue
#define EDITOR_INC_2_BUTTON_BACKGROUND_COLOR		ClrDarkBlue
#define EDITOR_INC_2_BUTTON_PUSH_COLOR				ClrLightBlue
#define EDITOR_INC_2_BUTTON_FONT					&g_sFontCm24
#define EDITOR_INC_2_BUTTON_STYLE					(PB_STYLE_FILL | PB_STYLE_OUTLINE | PB_STYLE_TEXT)
#define EDITOR_INC_2_BUTTON_TEXT					"+"
#define EDITOR_INC_2_BUTTON_ACTION					vIncreaseValue

// Decrease1Button
#define EDITOR_DEC_1_BUTTON_TOP						150
#define EDITOR_DEC_1_BUTTON_LEFT					102
#define EDITOR_DEC_1_BUTTON_WIDTH					40
#define EDITOR_DEC_1_BUTTON_HEIGHT					40
#define EDITOR_DEC_1_BUTTON_COLOR 					ClrWhite
#define EDITOR_DEC_1_BUTTON_OUTLINE_COLOR			ClrBlue
#define EDITOR_DEC_1_BUTTON_BACKGROUND_COLOR		ClrDarkBlue
#define EDITOR_DEC_1_BUTTON_PUSH_COLOR				ClrLightBlue
#define EDITOR_DEC_1_BUTTON_FONT					&g_sFontCm24
#define EDITOR_DEC_1_BUTTON_STYLE					(PB_STYLE_FILL | PB_STYLE_OUTLINE | PB_STYLE_TEXT)
#define EDITOR_DEC_1_BUTTON_TEXT					"-"
#define EDITOR_DEC_1_BUTTON_ACTION					vDecreaseValue

// Decrease2Button
#define EDITOR_DEC_2_BUTTON_TOP						150
#define EDITOR_DEC_2_BUTTON_LEFT					236
#define EDITOR_DEC_2_BUTTON_WIDTH					40
#define EDITOR_DEC_2_BUTTON_HEIGHT					40
#define EDITOR_DEC_2_BUTTON_COLOR 					ClrWhite
#define EDITOR_DEC_2_BUTTON_OUTLINE_COLOR			ClrBlue
#define EDITOR_DEC_2_BUTTON_BACKGROUND_COLOR		ClrDarkBlue
#define EDITOR_DEC_2_BUTTON_PUSH_COLOR				ClrLightBlue
#define EDITOR_DEC_2_BUTTON_FONT					&g_sFontCm24
#define EDITOR_DEC_2_BUTTON_STYLE					(PB_STYLE_FILL | PB_STYLE_OUTLINE | PB_STYLE_TEXT)
#define EDITOR_DEC_2_BUTTON_TEXT					"-"
#define EDITOR_DEC_2_BUTTON_ACTION					vDecreaseValue

// Value
#define EDITOR_VALUE_TOP							100
#define EDITOR_VALUE_LEFT							0
#define EDITOR_VALUE_WIDTH							GrContextDpyWidthGet(&g_sContext)
#define EDITOR_VALUE_HEIGHT							30
#define EDITOR_VALUE_COLOR 							ClrBlack
#define EDITOR_VALUE_OUTLINE_COLOR					ClrBlack
#define EDITOR_VALUE_BACKGROUND_COLOR				ClrLightGrey
#define EDITOR_VALUE_FONT							&g_sFontCm20b
#define EDITOR_VALUE_STYLE							(CANVAS_STYLE_TEXT | CANVAS_STYLE_FILL)

#endif /* EDITORSTYLE_H_ */

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

