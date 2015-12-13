/**************************************************************************
 * 
 * Project:  OpenCPN
 * Purpose:  Dead Reckoning Support
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

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "ODDRDialogImpl.h"
#include "DR.h"
#include "ocpn_draw_pi.h"
#include "ODConfig.h"
#include "ODSelect.h"
#include <wx/valnum.h>

extern ocpn_draw_pi *g_ocpn_draw_pi;
extern bool         g_bShowMag;
extern double       g_dVar;
extern double       g_dDRSOG;
extern int          g_iDRCOG;
extern double       g_dDRLength;
extern double       g_dDRPointInterval;
extern int          g_iDRLengthType;
extern int          g_iDRIntervalType;
extern int          g_iDRDistanceUnits;
extern int          g_iDRTimeUnits;
extern int          g_iDRPersistenceType;
extern bool         g_bDRPointShowRangeRings;
extern int          g_iDRPointRangeRingsNumber;
extern float        g_fDRPointRangeRingsStep;
extern int          g_iDRPointRangeRingsStepUnits;
extern wxColour     g_colourDRPointRangeRingsColour;
extern int          g_iDRPointRangeRingLineWidth;
extern int          g_iDRPointRangeRingLineStyle;

extern wxString        g_sDRPointIconName;

extern ODPlugIn_Position_Fix_Ex g_pfFix;
extern DRList                   *g_pDRList;
extern PathList                 *g_pPathList;
extern ODSelect                 *g_pODSelect;
extern ODConfig                 *g_pODConfig;

ODDRDialogImpl::ODDRDialogImpl( wxWindow* parent ) : ODDRDialogDef( parent )
{
    wxFloatingPointValidator<double> dSOGVal(3, &m_dSOGValidator, wxNUM_VAL_DEFAULT);
    wxFloatingPointValidator<double> dLengthVal(3, &m_dLengthValidator, wxNUM_VAL_DEFAULT);
    wxFloatingPointValidator<double> dIntervalVal(3, &m_dIntervalValidator, wxNUM_VAL_DEFAULT);
    wxIntegerValidator<int> iCOGVal(&m_iCOGValidator, wxNUM_VAL_DEFAULT);
    dSOGVal.SetMin(0);
    dLengthVal.SetMin(0);
    dIntervalVal.SetMin(0);
    iCOGVal.SetRange(0, 360);
    m_textCtrlSOG->SetValidator( dSOGVal );
    m_textCtrlCOG->SetValidator( iCOGVal );
    m_textCtrlLength->SetValidator( dLengthVal );
    m_textCtrlDRPointInterval->SetValidator( dIntervalVal );
    m_dSOGValidator = g_dDRSOG;
    m_iCOGValidator = g_iDRCOG;
    m_dLengthValidator = g_dDRLength;
    m_dIntervalValidator = g_dDRPointInterval;
}

void ODDRDialogImpl::UpdateDialog()
{
    wxString s;
    if(g_bShowMag && !wxIsNaN(g_dVar)) s.Printf( _T("Course over Ground %s"), _T("(M)") );
    else s.Printf( _T("Course over Ground %s"), _T("(T)") );
    m_staticTextCOG->SetLabel( s );
    m_dSOGValidator = g_dDRSOG;
    m_iCOGValidator = g_iDRCOG;
    m_dLengthValidator = g_dDRLength;
    m_dIntervalValidator = g_dDRPointInterval;
    
    this->Layout();
}

void ODDRDialogImpl::OnOK( wxCommandEvent& event )
{
    DR *l_pDR = new(DR);
    g_pDRList->Append( l_pDR );
    g_pPathList->Append( l_pDR );
    l_pDR->m_PathNameString << _("DR") << _T(" ") << g_pDRList->GetCount();
    
    ODPoint *beginPoint = new ODPoint( g_pfFix.Lat, g_pfFix.Lon, wxEmptyString, wxS("Start"), wxT("") );
    beginPoint->SetNameShown( false );
    beginPoint->SetTypeString( wxT("DR Point"));
    beginPoint->m_IconName = g_sDRPointIconName;
    beginPoint->m_bIsolatedMark = false;
    beginPoint->m_bShowODPointRangeRings = g_bDRPointShowRangeRings;
    beginPoint->m_iODPointRangeRingsNumber = g_iDRPointRangeRingsNumber;
    beginPoint->m_fODPointRangeRingsStep = g_fDRPointRangeRingsStep;
    beginPoint->m_iODPointRangeRingsStepUnits = g_iDRPointRangeRingsStepUnits;
    beginPoint->m_wxcODPointRangeRingsColour = g_colourDRPointRangeRingsColour;
    beginPoint->m_iRangeRingStyle = g_iDRPointRangeRingLineStyle;
    beginPoint->m_iRangeRingWidth = g_iDRPointRangeRingLineWidth;
    l_pDR->AddPoint( beginPoint, false );

    m_textCtrlSOG->GetValue().ToDouble( &l_pDR->m_dSoG );
    l_pDR->m_iCoG = g_ocpn_draw_pi->GetTrueOrMag( wxAtoi( m_textCtrlCOG->GetValue() ) );
    l_pDR->m_dMagCOG = g_dVar;
    m_textCtrlLength->GetValue().ToDouble( &l_pDR->m_dDRPathLength );
    m_textCtrlDRPointInterval->GetValue().ToDouble( &l_pDR->m_dDRPointInterval );
    
    l_pDR->m_iLengthType = m_radioBoxLengthType->GetSelection();
    l_pDR->m_iIntervalType = m_radioBoxIntervalType->GetSelection();
    l_pDR->m_iDistanceUnits = m_radioBoxDistanceUnits->GetSelection();
    l_pDR->m_iTimeUnits = m_radioBoxTimeUnits->GetSelection();
    
    switch ( m_radioBoxLengthType->GetSelection() ) {
        case ID_LT_TIME: {
            switch ( m_radioBoxTimeUnits->GetSelection() ) {
                case ID_TU_MINUTES:
                    l_pDR->m_dTotalLengthNM = ( l_pDR->m_dSoG / 60 ) * l_pDR->m_dDRPathLength;
                    break;
                case ID_TU_HOURS:
                    l_pDR->m_dTotalLengthNM = l_pDR->m_dSoG * l_pDR->m_dDRPathLength;
                    break;
                case ID_TU_DAYS:
                    l_pDR->m_dTotalLengthNM = ( l_pDR->m_dSoG * l_pDR->m_dDRPathLength ) * 24;
                    break;
            }
            break;
        }
        case ID_LT_DISTANCE: {
            if( m_radioBoxDistanceUnits->GetSelection() == ID_DU_KILOMETRES )
                l_pDR->m_dTotalLengthNM = l_pDR->m_dDRPathLength / 1.852;
            else
                l_pDR->m_dTotalLengthNM = l_pDR->m_dDRPathLength;
            break;
        }
    }
    
    switch ( m_radioBoxIntervalType->GetSelection() ) {
        case ID_IT_TIME: {
            switch ( m_radioBoxTimeUnits->GetSelection() ) {
                case ID_TU_MINUTES:
                    l_pDR->m_dDRPointIntervalNM = ( l_pDR->m_dSoG / 60 ) * l_pDR->m_dDRPointInterval;
                    break;
                case ID_TU_HOURS:
                    l_pDR->m_dDRPointIntervalNM = l_pDR->m_dSoG * l_pDR->m_dDRPointInterval;
                    break;
                case ID_TU_DAYS:
                    l_pDR->m_dDRPointIntervalNM = ( l_pDR->m_dSoG * l_pDR->m_dDRPointInterval ) * 24;
                    break;
            }
            break;
        }
        case ID_IT_DISTANCE: {
            if( m_radioBoxDistanceUnits->GetSelection() == ID_DU_KILOMETRES )
                l_pDR->m_dDRPointIntervalNM = l_pDR->m_dDRPointInterval / 1.852;
            else
                l_pDR->m_dDRPointIntervalNM = l_pDR->m_dDRPointInterval;
            break;
        }
    }
    
    double l_dStartLat = g_pfFix.Lat;
    double l_dStartLon = g_pfFix.Lon;
    double l_dEndLat;
    double l_dEndLon;

    PositionBearingDistanceMercator_Plugin( l_dStartLat, l_dStartLon, l_pDR->m_iCoG, l_pDR->m_dTotalLengthNM, &l_dEndLat, &l_dEndLon );
    
    int l_iNumWaypoints = floor( l_pDR->m_dTotalLengthNM / l_pDR->m_dDRPointIntervalNM );
    double l_cumLength = l_pDR->m_dDRPointIntervalNM;
    double l_dSaveLat = l_dStartLat;
    double l_dSaveLon = l_dStartLon;
    for( int i = 0; i < l_iNumWaypoints; i++ ) {
        double l_dLat, l_dLon;
        PositionBearingDistanceMercator_Plugin( l_dStartLat, l_dStartLon, l_pDR->m_iCoG, l_cumLength, &l_dLat, &l_dLon );
        ODPoint *l_NewPoint = new ODPoint( l_dLat, l_dLon, g_sDRPointIconName, wxT(""), wxT("") );
        l_NewPoint->SetNameShown( false );
        l_NewPoint->SetTypeString( wxS("DR Point") );
        l_NewPoint->m_bIsolatedMark = FALSE;
        l_NewPoint->m_bShowODPointRangeRings = g_bDRPointShowRangeRings;
        l_NewPoint->m_iODPointRangeRingsNumber = g_iDRPointRangeRingsNumber;
        l_NewPoint->m_fODPointRangeRingsStep = g_fDRPointRangeRingsStep;
        l_NewPoint->m_iODPointRangeRingsStepUnits = g_iDRPointRangeRingsStepUnits;
        l_NewPoint->m_wxcODPointRangeRingsColour = g_colourDRPointRangeRingsColour;
        l_NewPoint->m_iRangeRingStyle = g_iDRPointRangeRingLineStyle;
        l_NewPoint->m_iRangeRingWidth = g_iDRPointRangeRingLineWidth;
        l_pDR->AddPoint( l_NewPoint );
        g_pODSelect->AddSelectableODPoint( l_dLat, l_dLon, l_NewPoint );
        g_pODSelect->AddSelectablePathSegment( l_dLat, l_dLon, l_dSaveLat, l_dSaveLon, beginPoint, l_NewPoint, l_pDR );
        l_dSaveLat = l_dLat;
        l_dSaveLon = l_dLon;
        l_cumLength += l_pDR->m_dDRPointIntervalNM;
    }
    
    if( l_dEndLat != l_dSaveLat || l_dEndLon != l_dSaveLon ) {
        ODPoint *l_NewPoint = new ODPoint( l_dEndLat, l_dEndLon, g_sDRPointIconName, wxT(""), wxT("") );
        l_NewPoint->SetNameShown( false );
        l_NewPoint->SetTypeString( wxS("DR Point") );
        l_NewPoint->m_bIsolatedMark = FALSE;
        l_NewPoint->m_bShowODPointRangeRings = g_bDRPointShowRangeRings;
        l_NewPoint->m_iODPointRangeRingsNumber = g_iDRPointRangeRingsNumber;
        l_NewPoint->m_fODPointRangeRingsStep = g_fDRPointRangeRingsStep;
        l_NewPoint->m_iODPointRangeRingsStepUnits = g_iDRPointRangeRingsStepUnits;
        l_NewPoint->m_wxcODPointRangeRingsColour = g_colourDRPointRangeRingsColour;
        l_NewPoint->m_iRangeRingStyle = g_iDRPointRangeRingLineStyle;
        l_NewPoint->m_iRangeRingWidth = g_iDRPointRangeRingLineWidth;
        l_pDR->AddPoint( l_NewPoint );
        g_pODSelect->AddSelectableODPoint( l_dEndLat, l_dEndLon, l_NewPoint );
        g_pODSelect->AddSelectablePathSegment( l_dSaveLat, l_dSaveLon, l_dEndLat, l_dEndLon, beginPoint, l_NewPoint, l_pDR );
    }
    if(l_pDR->m_iPersistenceType == ID_DR_PERSISTENT || l_pDR->m_iPersistenceType == ID_DR_PERSISTENT_CRASH)
        g_pODConfig->AddNewPath( l_pDR, -1 );    // don't save over restart

    RequestRefresh( g_ocpn_draw_pi->m_parent_window );
    Show( false );
#ifdef __WXOSX__    
    EndModal(wxID_CANCEL);
#endif
}

void ODDRDialogImpl::OnCancel( wxCommandEvent& event )
{
    Show( false );
    #ifdef __WXOSX__    
    EndModal(wxID_CANCEL);
    #endif
    
    event.Skip();
}
