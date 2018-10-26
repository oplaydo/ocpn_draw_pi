/***************************************************************************
 * 
 * Project:  OpenCPN
 * Purpose:  ODPath Properties Support
 * Author:   Jon Gough
 *
 ***************************************************************************
 *   Copyright (C) 2010 by David S. Register                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.         *
 **************************************************************************/

#ifndef __ODPathPropertiesDialogImpl__
#define __ODPathPropertiesDialogImpl__

/**
@file
Subclass of ODPathPropertiesDialogDef, which is generated by wxFormBuilder.
*/

#include "ODPathPropertiesDialogDef.h"

//// end generated include

////@begin control identifiers
#define ID_PATHPROP                     9000
#define SYMBOL_PATHPROP_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_PATHPROP_TITLE wxT("Path Properties")
#define SYMBOL_PATHPROP_IDNAME ID_PATHPROP
#define SYMBOL_PATHPROP_SIZE wxSize(450, 300)
#define SYMBOL_PATHPROP_POSITION wxDefaultPosition
enum {
    ID_PATHPROP_TEXTCTRL = 9001,
    ID_PATHPROP_TEXTCTRL2,
    ID_PATHPROP_TEXTCTRL1,
    ID_PATHPROP_TEXTCTRL3,
    ID_PATHPROP_LISTCTRL,
    ID_PATHPROP_CANCEL,
    ID_PATHPROP_OK,
    ID_PATHPROP_SPLIT,
    ID_PATHPROP_EXTEND,
    ID_PATHPROP_COPYTXT,
    ID_PATHPROP_PRINT,
    ID_PATHPROP_TEXTCTRL4,
    ID_PATHPROP_TEXTCTRLDESC,
    ID_PATHPROP_STARTTIMECTL,
    ID_PATHPROP_ACTIVE,
    ID_PATHPROP_MENU_COPY_TEXT,
    ID_PATHPROP_MENU_EDIT_PROPERTIES,
    ID_PATHPROP_MENU_DELETE,
    ID_PATHPROP_MENU_REMOVE,
    ID_PILPROP_MENU_EDIT_PROPERTIES,
    ID_PILPROP_MENU_REMOVE

};

// Forward declaration
class   ODPath;
class   Boundary;
class   EBL;
class   DR;
class   GZ;
class   PIL;
class   ODPoint;

/** Implementing ODPathPropertiesDialogDef */
class ODPathPropertiesDialogImpl : public ODPathPropertiesDialogDef
{
	protected:
		// Handlers for ODPathPropertiesDialogDef events.
		void OnClose( wxCloseEvent& event );
        void OnLeftDoubleClick( wxMouseEvent& event );
        void OnLeftDoubleClickPIL( wxMouseEvent& event );
        void OnRightClick( wxMouseEvent& event );
        void OnRightClickPIL( wxMouseEvent& event );
        void OnOK( wxCommandEvent& event );
		void OnCancel( wxCommandEvent& event );
        
        virtual void InitializeList( void );
        virtual bool SaveChanges( void );
        virtual void SetPointsListHeadings();
        
        int         m_nSelected; // index of point selected in Properties dialog row
        ODPoint     *m_pEnroutePoint;
        bool        m_bStartNow;
        int         m_tz_selection;
        
	public:
		/** Constructor */
        ODPathPropertiesDialogImpl( void );
        ODPathPropertiesDialogImpl( wxWindow* parent );
        ODPathPropertiesDialogImpl( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style );
	//// end generated class members
        void SetPathAndUpdate( ODPath *pP, bool only_points = false );
        virtual void SetPath( ODPath *pP );
        virtual bool UpdateProperties( ODPath *pPath );
        virtual bool UpdateProperties( void );
        void SetDialogTitle( const wxString &sTitle );
        ODPath *GetPath(void) {return m_pPath;}
        
        ODPath      *m_pPath;
        Boundary    *m_pBoundary;
        EBL         *m_pEBL;
        DR          *m_pDR;
        GZ          *m_pGZ;
        PIL         *m_pPIL;
        
    private:
        void    SetViewableItems();
        void    OnPathPropMenuSelected( wxCommandEvent& event );
        void    SetDialogSize( void );
        
};

#endif // __ODPathPropertiesDialogImpl__
